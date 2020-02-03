#pragma once
#include "correlation.h"
#include "MyAdd.h" 
#include <sstream>
#include "StreamTable.h"

void CorrelationAnalysFull(HISTOGRAM* Data[100], wchar_t FileName[80], int Count) //вывод в текстовый файл
{
	std::vector<int> SelectedHistogram;
	int NumAnal;

	wchar_t ListForAnalysis[256] = { 0 }, Correlations[256] = { 0 };
	wchar_t FileNameWithoutSHT[80] = { 0 };

	//Задание путей, для вывода данных
	wchar_t FullFileName[256] = { 0 };

	GetModuleFileNameW(NULL, FullFileName, sizeof(FullFileName) / sizeof(FullFileName[0]));
	RemoveProgramName(FullFileName);

	wcscpy_s(FileNameWithoutSHT, 80, FileName);
	int len = wcslen(FileNameWithoutSHT);
	for (int i = 0; i < 4; i++)
		FileNameWithoutSHT[len - i - 1] = L'\0';

	//Создание пути для входных данных
	wcscpy_s(ListForAnalysis, 256, FullFileName);
	wcscat_s(ListForAnalysis, 256, L"Selected for analysis\\");
	wcscat_s(ListForAnalysis, 256, L"Selected.txt");

	//Создание пути для выходных данных
	wcscpy_s(Correlations, 256, FullFileName);
	wcscat_s(Correlations, 256, L"Correlation\\");
	wcscat_s(Correlations, 256, FileNameWithoutSHT);
	wcscat_s(Correlations, 256, L"-corr.txt");

	std::ifstream InRead(ListForAnalysis, std::ios_base::in);
	std::ofstream OutWrite(Correlations, std::ios::trunc | std::ios_base::app);

	HISTOGRAM* H1;
	HISTOGRAM* H2;

	InRead >> NumAnal;
	
	std::string* NameHistogram = new std::string[NumAnal];

	for (int i = 0; i < NumAnal; i++) {
		std::getline(InRead, NameHistogram[i]);
	}

	for (int k = 0; k < Count; k++) {
		H1 = Data[k];
		for (int i = 0; i < NumAnal; i++)
			if (strcmp(NameHistogram[i].c_str(), H1->Name) == 0) 
				SelectedHistogram.push_back(k);	

	}

	for (const auto& k : SelectedHistogram) {
		H1 = Data[k];
		OutWrite << H1->Name << " in table as " << k << std::endl;
	}

	OutWrite << std::endl << std::endl;

	NumAnal = SelectedHistogram.size();

	// динамическое создание двумерного массива вещественных чисел на десять элементов
	if (NumAnal > 1) {
		float** TableCorrelations = new float* [NumAnal];
		for (int i = 0; i < 2; i++)
			TableCorrelations[i] = new float[NumAnal];



		for (int i = 0; i < NumAnal; i++) {
			H1 = Data[SelectedHistogram[i]];

			int Size0;
			HISTOGRAM* H0;
			//преобразование данных из вида, полученного при распаковке, к обычному.
			Size0 = sizeof(HISTOGRAM) - sizeof(LONG) + HistogramDataSize(H1->NChannels, H1->Type);
			H0 = (HISTOGRAM*)GlobalAlloc(GMEM_FIXED, Size0);
			memcpy(H0, H1, Size0);
			ChangeByteOrder(&H0->Data[0], HistogramDataSize(H1->NChannels, H1->Type), 4, true);
			GlobalFree(H0);

			for (int j = 0; j < i + 1; j++)
				TableCorrelations[i][j] = 0;

			for (int j = i + 1; j < NumAnal; j++) {

				H2 = Data[SelectedHistogram[j]];

				int Size0;
				HISTOGRAM* H0;
				//преобразование данных из вида, полученного при распаковке, к обычному.
				Size0 = sizeof(HISTOGRAM) - sizeof(LONG) + HistogramDataSize(H2->NChannels, H2->Type);
				H0 = (HISTOGRAM*)GlobalAlloc(GMEM_FIXED, Size0);
				memcpy(H0, H2, Size0);
				ChangeByteOrder(&H0->Data[0], HistogramDataSize(H2->NChannels, H2->Type), 4, true);
				GlobalFree(H0);

				if (H1->NChannels == H2->NChannels) {
					float A, B, C;
					A = 0;
					B = 0;
					C = 0;

					for (int count = 0; count < H2->NChannels; count++) {
						A += H1->Data[i] * H1->Data[i];
						B += H2->Data[i] * H2->Data[i];
						C += H1->Data[i] * H2->Data[i];
					}

					TableCorrelations[i][j] = C / (sqrt(abs(A)) * sqrt(abs(B)));
				}

				else {
					printf("Histogram sizes do not match! #%d...#%d\n", i, j);
				}

			}
		}

		PrintTable(OutWrite, NumAnal, SelectedHistogram, TableCorrelations);

		printf("\n");
		InRead.close();
		OutWrite.close();

		for (int i = 0; i < 2; i++)
			delete[] TableCorrelations[i];
		delete[] TableCorrelations;
	}
}


void PrintTable(std::ofstream& a, int N, std::vector<int> vec, float** Array) {
	StreamTable tb(a);
	tb.AddCol(5);

	for (int i = 0; i < N; i++)
		tb.AddCol(5);
	tb << "#";
	for (int i = 0; i < N; i++)
		tb << vec[i];

	for (int i = 0; i < N; i++) {
		tb << vec[i];
		for (int j = 0; j < N; j++) {
			tb << Array[i][j];
		}
	}
}