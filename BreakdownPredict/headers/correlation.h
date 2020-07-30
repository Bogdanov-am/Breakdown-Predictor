#pragma once
#include <math.h>
#include <stdlib.h>
#include <iomanip>
#include <vector>
#include "compress.h"
#include "MyAdd.h" 
#include <sstream>
#include <iostream>

void CorrelationAnalysFull(HISTOGRAM* Data[100], wchar_t FileName[80], int Count);
void PrintTable(std::ofstream&, int, std::vector<int>, float**);