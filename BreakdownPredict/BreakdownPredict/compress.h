#include <cstdio>
#include <windows.h>
#include <malloc.h>

#include <iomanip>
#include <iostream>
#include <fstream>

typedef struct tagHISTOGRAM {
	int Type;
	char Name[128];
	char Comment[128];
	char Unit[128];
	SYSTEMTIME Time;
	int NChannels;
	double Tmin, Tmax;
	double Umin, Delta;
	LONG  Data[1];
}HISTOGRAM;

int DefineVersion(HANDLE hFile, char* FileHeader);
int HistogramDataSize(int NChannels, int Type);
void* CompressRLE(void*, int, int*);
void* DecompressRLE(void*, int, int*);
void* CompressHoffman(void*, int, int*);
void* DecompressHoffman(void*, int, int*);
void ChangeByteOrder(void*, int, int, bool);
HISTOGRAM* DecompressHist(void* H, int Size);
void* CompressHist(HISTOGRAM* H, int* Size);

