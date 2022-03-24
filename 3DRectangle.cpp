#include "DirectTX.cpp"
#include <stdio.h>

const int LENGTH = 1000;
const int WIDTH  = 600;
double Znear = 19;

const int A = 4;

double Res[A][A] = {};
double Matrix_WL[WIDTH][LENGTH] = {};

double Lerp       (double  x1, double  x2, double  t);

double Lerp (double x1, double x2, double t)
{
    x1 = x1 + t * (x2 - x1);

    return x1;
}
