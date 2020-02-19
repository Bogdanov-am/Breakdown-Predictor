#pragma once
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <fstream>
#include <iomanip>
#include "compress.h" 

void Pack(wchar_t FullFileNameOut[256], HISTOGRAM* Data[100], char FileHeader[12], int Count);
void Depack(wchar_t FullFileNameIn[256], char *FileHeader, HISTOGRAM** Data, int& Count);
void RemoveProgramName(wchar_t* FullFileName);
void Path(wchar_t *FileName, wchar_t *FullFileNameIn, wchar_t *FullFileNameOut);
void PrintData(HISTOGRAM* Data[100], wchar_t* FileName, int Count);