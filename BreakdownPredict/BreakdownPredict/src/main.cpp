#include "compress.h" 
#include "MyAdd.h" 
#include "correlation.h"
#include <iostream>
#include <iomanip>
#include <OpenXLSX/OpenXLSX.h>

int main(int argc, wchar_t* argv[]) 
{
	wchar_t FullFileName[256] = { 0 }, FileName[80] = { 0 }, FullFileNameIn[256] = { 0 }, FullFileNameOut[256] = { 0 };
	char FileHeader[12] = { 0 };
	char s;
	int Count; 

	printf("Enter the file name: ");
	wscanf_s(L"%lS", FileName, (unsigned)_countof(FileName));

	Path(FileName, FullFileNameIn, FullFileNameOut);			//создание путей для файлов ввода и вывода 

	printf("\n");
	wprintf(L"Name of file was succesfully read: %lS", FullFileNameIn);

	//создается массив гистограмм
	HISTOGRAM** Data = new HISTOGRAM*[100];
	// количество гистограмм
	Count = 0;		

	Depack(FullFileNameIn, FileHeader, Data, Count);

	/*PrintData(Data, FileName, Count);*/
	CorrelationAnalysFull(Data, FileName, Count);


	/*Pack(FullFileNameOut, Data, FileHeader, Count);*/

	delete[] Data;

	s = getchar();
	s = getchar();


	return 0;
}


