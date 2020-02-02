#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <fstream>
#include <iomanip>

void Pack(wchar_t *FullFileNameOut, LONG Data[100], char FileHeader[12], int Count);
void Depack(wchar_t *FullFileNameIn, char FileHeader[12], LONG* Data, int& Count);
void RemoveProgramName(wchar_t* FullFileName);
void Path(wchar_t *FileName, wchar_t *FullFileNameIn, wchar_t *FullFileNameOut);
void PrintData(LONG* Data, wchar_t* FileName, int Count);