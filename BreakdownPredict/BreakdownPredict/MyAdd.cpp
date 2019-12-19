
#include "compress.h" 
#include "MyAdd.h" 

#define PI 3.14159265 


void Depack(wchar_t FullFileNameIn[256], char FileHeader[12], LONG* Data, int& Count) //распаковка .SHT в массив указателей Data
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
							Data[i] = (LONG)H;
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

void Pack(wchar_t FullFileNameOut[256], LONG Data[100], char FileHeader[12], int Count) // упаковка данных из Data в .SHT файл
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
		H = (HISTOGRAM*)Data[i];
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

void Path(wchar_t FileName[80], wchar_t FullFileNameIn[256], wchar_t FullFileNameOut[256])
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