#include <iostream>
#include <array>
#include <cmath>
#include <vector>

using namespace std;

#define matrix_size 8
#define vector_size 16

double pow(double number, int s);

void componentInImage(double *image, int x, int y, double *component, int width);

void transpon(double *matrix, double *transpon);

void maxrixMultiply(double *matrix1, double *matrix2, double *ans);

void squareMatrix(double *matrix);

double matrixSumm(double* matrix);

void dct2Dblind(double *image, int cols, int rows, double *coff);