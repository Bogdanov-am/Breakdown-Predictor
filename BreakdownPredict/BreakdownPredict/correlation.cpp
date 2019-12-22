#include "correlation.h"

int CorrelationAnalys(int size, float *X, float *Y, float R0)
{
	int i;
	float sum = 0;

	for (i = 0; i = size; i++)
		sum += X[i] * Y[i];

	R0 = sum / float(size);

	return 0;
}

void ArrayAlignment(int sizeX, int sizeY, int sizeOut, float *X, float *Y)
{

}