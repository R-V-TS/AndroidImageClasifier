//
// Created by rostislav on 06.11.18.
//


void rgb_to_y(int *array, double *array_y, int size)
{
    int R = 0;
    int G = 0;
    int B = 0;
    for(int i = 0; i < size; i++)
    {
        R = (*(array+i) >> 16) & 0xff;
        G = (*(array+i) >> 8) & 0xff;
        B = *(array+i) & 0xff;

        *(array_y+i) = 0.299*R + 0.587*G + 0.114*B;
    }
}


double maximus[16] = {0.943, 0.0911, 2.87, 24.57, 0.33, 0.045, 2.769, 14.091, 0.2812, 0.0173, 9.705, 204.841, 0.154, 0.0083, 18.811, 757.186};
double minimus[16] = {0.2367, 0.0024, -3.638, 1.38, 0.0492, 0.0018, -0.4913, 1.966, 0.00632, 6.727, -0.5408, 1.8566, 0.0014, 6.6458, 0.1879, 2.4729};

void normalize_coefficient(double *coff) {
    for (int i = 0; i < 16; i++) {
        *(coff+i) = (((1 - (-1)) * ((*(coff+i)) - minimus[i])) / (maximus[i] - minimus[i]) + (-1));
    }
}