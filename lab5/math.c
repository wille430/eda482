

#include "math.h"

float pow(float n, int exp)
{
    float res = 1;
    for (;;)
    {
        if (exp & 1)
            res *= n;
        exp >>= 1;
        if (!exp)
            break;
        n *= n;
    }
    return res;
}

int fac(int n)
{
    int prod = 1;
    for (int i = 2; i <= n; i++)
    {
        prod *= i;
    }
    return prod;
}

float sin(float rad)
{
    float sum = 0;
    
    for (int i = 0; i < 10; i++)
    {
        int n = 1+(i*2);
        sum += pow(-1, i) * pow(rad, n) / ((float) fac(n));
    }
    
    return sum;
}

float cos(float rad)
{
    return sin(M_PI/2 - rad);
}

float tan(float rad)
{
    return sin(rad) / cos(rad);
}

int min(int a, int b) {
    return a < b ? a : b;
}