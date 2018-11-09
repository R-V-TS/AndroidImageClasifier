#include <iostream>
#include <array>
#include <cmath>

#define MAX_COLL 512
#define MAX_ROW 512

using namespace std;

const int matrix_size = 8;
const int vector_size = 16;

double maximus[16] = {0.943, 0.0911, 2.87, 24.57, 0.33, 0.045, 2.769, 14.091, 0.2812, 6.727, 9.705, 204.841, 0.154, 6.6458, 18.811, 757.186};
double minimus[16] = {0.2367, 0.0024, -3.638, 1.38, 0.0492, 0.0018, -0.4913, 1.966, 0.00632, 0.0173, -0.5408, 1.8566, 0.0014, 0.0083, 0.1879, 2.4729};

//Power number
double pow(double number, int s)
{
    double ans = 1;
    for(int i = 0; i < s; i++)
        ans *= number;
    return ans;
}

//Return component (8x8) from image
void componentInImage(double *image, int x, int y, double *component, int width){
    for(int i = x, i_ans = 0; i < x+8; i++, i_ans++)
        for(int j = y, j_ans = 0; j < y+8; j++, j_ans++)
        {
            *(component+((i_ans*8)+j_ans)) = *(image+((i*width)+j));
        }
}

//Return transposing matrix
void transpon(double *matrix, double *transpon){
    for(int i = 0; i < matrix_size; i++)
        for(int j = 0; j < matrix_size; j++)
        {
            *(transpon+((i*matrix_size)+j)) = *(matrix+((j*matrix_size)+i));
        }
}

//Multiply matrix
void maxrixMultiply(double *matrix1, double *matrix2, double *ans){
    for(int i = 0; i < matrix_size; i++)
    {
        for(int j = 0; j < matrix_size; j++)
        {
            double summ = 0;
            for(int k = 0; k<matrix_size; k++)
            {
                summ += (*(matrix1+((i*matrix_size)+k)) * (*(matrix2+((k*matrix_size)+j))));
            }
            *(ans+((i*matrix_size)+j)) = summ;
        }
    }
}

//Square matrix element
void squareMatrix(double *matrix){
    for (int i = 0; i < matrix_size; ++i) {
        for (int j = 0; j < matrix_size; ++j) {
            *(matrix+((i*matrix_size)+j)) = pow(*(matrix+((i*matrix_size)+j)), 2);
        }
    }
}

//Sum matrix element
double matrixSumm(double* matrix) {
    double summ = 0;
    for(int i = 0; i < matrix_size; i++)
        for (int j = 0; j < matrix_size; ++j) {
            summ += *(matrix+((i*matrix_size)+j));
        }
    return summ;
}

//Main function for calculate
void dct2Dblind(double *image, int cols, int rows, double *coff)
{
    double flags[matrix_size][matrix_size] = {
            {0, 1, 1, 1, 1, 2, 2, 2},
            {1, 1, 1, 1, 2, 2, 2, 3},
            {1, 1, 1, 2, 2, 2, 3, 3},
            {1, 1, 2, 2, 2, 3, 3, 3},
            {1, 2, 2, 2, 3, 3, 3, 4},
            {2, 2, 2, 3, 3, 3, 4, 4},
            {2, 2, 3, 3, 3, 4, 4, 4},
            {2, 3, 3, 3, 4, 4, 4, 4}
    };

    double Trans2D[matrix_size][matrix_size] = {
            {0.3536, 0.3536, 0.3536, 0.3536, 0.3536, 0.3536, 0.3536, 0.3536},
            {0.4904, 0.4157, 0.2778, 0.0975, -0.0975, -0.2778, -0.4157, -0.4904},
            {0.4619, 0.1913, -0.1913, -0.4619, -0.4619, -0.1913, 0.1913, 0.4619},
            {0.4157, -0.0975, -0.4904, -0.2778, 0.2778, 0.4904, 0.0975, -0.4157},
            {0.3536, -0.3536, -0.3536, 0.3536, 0.3536, -0.3536, -0.3536, 0.3536},
            {0.2778, -0.4904, 0.0975, 0.4157, -0.4157, -0.0975, 0.4904, -0.2778},
            {0.1913, -0.4619, 0.4619, -0.1913, -0.1913, 0.4619, -0.4619, 0.1913},
            {0.0975, -0.2778, 0.4157, -0.4904, 0.4904, -0.4157, 0.2778, -0.0975}
    };

    double unic[4] = {1, 2, 3, 4};
    double EST[cols][rows][4];
    double transp[matrix_size][matrix_size];
    double dctx[matrix_size][matrix_size];
    double component[matrix_size][matrix_size];
    double temp[matrix_size][matrix_size];
    double slices[4];

    transpon(&Trans2D[0][0], &transp[0][0]);

    for(int i = 0; i < cols-8; i++)
    {
        for (int j = 0; j < rows-8; j++) {
            componentInImage(image, i, j, &component[0][0], rows);
            maxrixMultiply(&Trans2D[0][0], &component[0][0], &temp[0][0]);
            maxrixMultiply(&temp[0][0], &transp[0][0], &dctx[0][0]);
            squareMatrix(&dctx[0][0]);
            dctx[0][0] = 0;
            double fullDCT = matrixSumm(&dctx[0][0]);
            if(fullDCT != 0)
            {
                for(int k = 0; k < 4; k++) {
                    slices[k] = 0;
                    for (int s = 0; s < matrix_size; s++)
                        for (int l = 0; l < matrix_size; l++)
                            if (unic[k] == flags[s][l]) slices[k] += dctx[s][l];
                }
                for(int k = 0; k < 4; k++)
                    slices[k] /= fullDCT;
            }
            EST[i][j][0] = slices[0];
            EST[i][j][1] = slices[1];
            EST[i][j][2] = slices[2];
            EST[i][j][3] = slices[3];
        }
    }
    double summ = 0;
    int count = 0;
    for(int freq = 0; freq < 4; freq++)
    {
        summ = 0;
        count = 0;
        for(int i = 0; i < cols-8; i++)
        {
            for(int j = 0; j < rows-8; j++) {
                summ += EST[i][j][freq];
                count++;
            }
        }
        double mean = summ / count;
        summ = 0;
        double summ_for_skewness = 0;
        double summ_for_kurtosis = 0;
        for(int i = 0; i < cols-8; i++)
            for(int j = 0; j < rows-8; j++) {
                summ += pow((EST[i][j][freq] - mean), 2);
                summ_for_skewness += pow((EST[i][j][freq]- mean), 3);
                summ_for_kurtosis += pow((EST[i][j][freq] - mean), 4);
            }
        double variance = summ / (count-1);
        double skewness = (summ_for_skewness/count) / pow((sqrt(summ/count)), 3);
        double kurtosis = (summ_for_kurtosis/double(count)) / pow((summ/double(count)),2);
        *(coff + (freq*4)) = (((1 - (-1)) * ((mean) - minimus[freq*4])) / (maximus[freq*4] - minimus[freq*4])) + (-1);
        *(coff + (freq*4)+1) = (((1 - (-1)) * ((variance) - minimus[(freq*4)+1])) / (maximus[(freq*4)+1] - minimus[(freq*4)+1])) + (-1);;
        *(coff + (freq*4)+2) = (((1 - (-1)) * ((skewness) - minimus[(freq*4)+2])) / (maximus[(freq*4)+2] - minimus[(freq*4)+2])) + (-1);;
        *(coff + (freq*4)+3) = (((1 - (-1)) * ((kurtosis) - minimus[(freq*4)+3])) / (maximus[(freq*4)+3] - minimus[(freq*4)+3])) + (-1);;
    }
}