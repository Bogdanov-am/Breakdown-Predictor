#pragma once
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <map>
//#include "boost/filesystem/operations.hpp"
//#include "boost/filesystem/path.hpp"

void readTimeDisruption(wchar_t FileNameIn[256], std::map<int, float>);
void createDataSet(wchar_t DirNameIn[256], wchar_t FileNameIn[256], wchar_t FileNameOut[256]);
void writeData(HANDLE hFileOut, char* Data);