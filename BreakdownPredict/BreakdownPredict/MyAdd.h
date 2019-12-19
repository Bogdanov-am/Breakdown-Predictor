#include <math.h>
#include <windows.h>
#include <stdlib.h>


void Pack(wchar_t FullFileNameOut[256], LONG Data[100], char FileHeader[12], int Count);
void Depack(wchar_t FullFileNameIn[256], char FileHeader[12], LONG* Data, int& Count);
void RemoveProgramName(wchar_t* FullFileName);
void Path(wchar_t FileName[80], wchar_t FullFileNameIn[256], wchar_t FullFileNameOut[256]);