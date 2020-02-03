#pragma once
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <fstream>
#include <iomanip>
#include <vector>
#include "compress.h"

void CorrelationAnalysFull(HISTOGRAM* Data[100], wchar_t FileName[80], int Count);
void PrintTable(std::ofstream&, int, std::vector<int>, float**);