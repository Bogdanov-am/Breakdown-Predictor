#pragma once
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

void CorrelationAnalysFull(HISTOGRAM* Data[100], wchar_t*, int);
void PrintTable(std::ofstream&, int, std::vector<int>, float**);