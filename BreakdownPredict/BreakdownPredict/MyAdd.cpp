

#include "MyAdd.h" 

#define PI 3.14159265 


void Depack(wchar_t FullFileNameIn[256], char *FileHeader, HISTOGRAM** Data, int& Count) //распаковка .SHT в массив указателей Data
{
	bool validRead;
	HANDLE hFileIn;
	void* P;
	int Version, Size;
	DWORD NBytes;

	//открытие файла, определения, и всякие словечки в консоль

	hFileIn = CreateFileW(FullFileNameIn, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	printf("\n");

	if (hFileIn == INVALID_HANDLE_VALUE) {
		printf("Failed to create input file handle!\n");
		DWORD ErrIn = GetLastError();
		printf("The errorcode is %d", ErrIn);
	}
	else printf("Handle of input file is created.\n");
	printf("\n");


	Version = DefineVersion(hFileIn, FileHeader);									//считывание версии
	printf("File header: %s\n", FileHeader);

	printf("Number of version: %d\n", Version);

	//читаем количество гистограмм
	validRead = ReadFile(hFileIn, &Count, sizeof(int), &NBytes, NULL);				//чтение количества гистограмм
	HISTOGRAM* H;

	//сама суть - запускаем цикл по количеству, читаем, распаковываем

	if (NBytes == sizeof(int)) {
		printf("Count: %d\n", Count);
		for (int i = 0; i < Count; i++) {
			H = NULL;
			if (ReadFile(hFileIn, &Size, sizeof(int), &NBytes, NULL))				//считывание размера 1 гистограммы             
				if ((NBytes == sizeof(int)) && (Size > 0)) {
					printf("Depacking histogram #%d... Size #%d\n", i + 1, Size);
					P = GlobalAlloc(GMEM_FIXED, Size);

					if (ReadFile(hFileIn, P, Size, &NBytes, NULL))					//считывание самих данных                         
						if (NBytes == Size) {
							H = DecompressHist(P, Size);							//распаковка данных
							Data[i] = H;
						}
					
					GlobalFree(P);
				}
		}
	}

	printf("File openning complete.\n");
	printf("\n");
	printf("\n");
	CloseHandle(hFileIn);
}

void Pack(wchar_t FullFileNameOut[256], HISTOGRAM* Data[100], char FileHeader[12], int Count) // упаковка данных из Data в .SHT файл
{
	HISTOGRAM* H;
	int Size;
	void* P;
	DWORD NBytes;
	HANDLE hFileOut;

	//открытие файла, определения, и всякие словечки в консоль
	wprintf(L"Name of the output file: %s", FullFileNameOut);
	hFileOut = CreateFile(FullFileNameOut, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	printf("\n");

	if (hFileOut == INVALID_HANDLE_VALUE)
	{
		printf("Failed to create output file handle!\n");
		DWORD ErrIn = GetLastError();
		printf("The errorcode is %d", ErrIn);
	}
	else printf("Handle of output file is created.\n");

	char FileHeader2[] = "ANALIZER1.2";

	//запись версии файла
	WriteFile(hFileOut, FileHeader, sizeof(FileHeader2), &NBytes, NULL);
	//запись количества
	WriteFile(hFileOut, &Count, sizeof(int), &NBytes, NULL);

	//сама суть - запись гистограмм
	for (int i = 0; i < Count; i++) {
		H = Data[i];
		if (H) {
			printf("Packing histogram #%d...\n", i + 1);
			P = CompressHist(H, &Size);
			WriteFile(hFileOut, &Size, sizeof(int), &NBytes, NULL);
			WriteFile(hFileOut, P, Size, &NBytes, NULL);
			GlobalFree(P);
		}
	}
	CloseHandle(hFileOut);
	printf("\n");
	printf("Output file is ready.");
	printf("\n");
	printf("\n");
}

void RemoveProgramName(wchar_t* FullFileName)
{
	int i = wcslen(FullFileName) - 1;

	while (FullFileName[i] != L'\\') {
		FullFileName[i] = L'\0';
		i--;
	}
}

void Path(wchar_t *FileName, wchar_t *FullFileNameIn, wchar_t *FullFileNameOut)
{
	wchar_t FullFileName[256] = { 0 };
	GetModuleFileNameW(NULL, FullFileName, sizeof(FullFileName) / sizeof(FullFileName[0]));

	RemoveProgramName(FullFileName);

	//Создание пути для входных данных
	wcscpy_s(FullFileNameIn, 256, FullFileName);
	wcscat_s(FullFileNameIn, 256, L"Input\\");
	wcscat_s(FullFileNameIn, 256, FileName);

	//Создание пути для выходных данных
	wcscpy_s(FullFileNameOut, 256, FullFileName);
	wcscat_s(FullFileNameOut, 256, L"Output\\");
	wcscat_s(FullFileNameOut, 256, FileName);
}

void PrintData(HISTOGRAM* Data[100], wchar_t* FileName, int Count) //вывод в текстовый файл
{
	int i;
	wchar_t FullDepacked[256] = { 0 }, TestDots[256] = { 0 };
	wchar_t FileNameWithoutSHT[80] = { 0 };

	//Задание путей, для вывода данных
	wchar_t FullFileName[256] = { 0 };

	GetModuleFileNameW(NULL, FullFileName, sizeof(FullFileName) / sizeof(FullFileName[0]));
	RemoveProgramName(FullFileName);

	wcscpy_s(FileNameWithoutSHT, 80, FileName);
	int len = wcslen(FileNameWithoutSHT);
	for (i = 0; i < 4; i++)
		FileNameWithoutSHT[len - i - 1] = L'\0';

	//Создание пути для выходных данных
	wcscpy_s(FullDepacked, 256, FullFileName);
	wcscat_s(FullDepacked, 256, L"Input\\");
	wcscat_s(FullDepacked, 256, FileNameWithoutSHT);
	wcscat_s(FullDepacked, 256, L"-FullDepacked.txt");

	//Создание пути для выходных данных
	wcscpy_s(TestDots, 256, FullFileName);
	wcscat_s(TestDots, 256, L"Output\\");
	wcscat_s(TestDots, 256, FileNameWithoutSHT);
	wcscat_s(TestDots, 256, L"-TestDots.txt");

	std::ofstream T1(FullDepacked, std::ios::trunc);
	std::ofstream T2(TestDots, std::ios::trunc);
	T1.close();
	T2.close();

	std::ofstream F(FullDepacked, std::ios_base::app);
	std::ofstream FF(TestDots, std::ios_base::app);

	HISTOGRAM* H;
	for (int k = 0; k < Count; k++) {
		H = (HISTOGRAM*)Data[k];
		printf("Printing histogram #%d...\n", k + 1);

		// вывод информации из гистограммы
		F << "Type " << H->Type << std::endl;
		F << "Name " << H->Name << std::endl;
		F << "Comment " << H->Comment << std::endl;
		F << "Unit " << H->Unit << std::endl;
		F << "NChannels " << H->NChannels << std::endl;
		F << "Tmin, Tmax " << H->Tmin << " " << H->Tmax << std::endl;
		F << "Umin, Delta " << H->Umin << " " << H->Delta << std::endl;

		FF << "Type " << H->Type << std::endl;
		FF << "Name " << H->Name << std::endl;
		FF << "Comment " << H->Comment << std::endl;
		FF << "Unit " << H->Unit << std::endl;
		FF << "NChannels " << H->NChannels << std::endl;
		FF << "Tmin, Tmax " << H->Tmin << " " << H->Tmax << std::endl;
		FF << "Umin, Delta " << H->Umin << " " << H->Delta << std::endl;

		int Size0;
		HISTOGRAM* H0;

		//преобразование данных из вида, полученного при распаковке, к обычному.
		Size0 = sizeof(HISTOGRAM) - sizeof(LONG) + HistogramDataSize(H->NChannels, H->Type);
		H0 = (HISTOGRAM*)GlobalAlloc(GMEM_FIXED, Size0);
		memcpy(H0, H, Size0);
		ChangeByteOrder(&H0->Data[0], HistogramDataSize(H->NChannels, H->Type), 4, true);
		GlobalFree(H0);

		//Полный вывод точек и частичный вывод точек гистограмм (для оценки характера)
		int check = 0;
		int t = 0;
		for (int i = 0; i < H->NChannels; i++) {
			/*F << std::setw(5) << H->Data[i] << " ";*/
			t++;
			check++;

			if (check == 5000) {
				FF << std::setw(5) << H->Data[i] << " ";
				check = 0;
			}
			if (t == 10) {
				F << std::endl;
				t = 0;
			}
		}
		FF << std::endl;
		FF << std::endl;
		F << std::endl;
		F << std::endl;
	}

	printf("\n");
	F.close();
	FF.close();
}