#include <math.h>
#include <stdio.h>
#include "src/ffte.h"


int main()
{
    // Create data
    unsigned int N = 100;
    double x_r[N];
    double x_i[N];

    double f=10;//10 for N=100 , 9.375 for N=128
    double fs=200;

    for(int i=0; i< N; ++i)
    {
        x_r[i]=cos(2.0*M_PI*f/fs*((double)i));
    }
    
    // Print data
    printf("\nInput Signal:\n");
    for(int i=0; i< N; ++i)
    {
        printf("\nReal: %f",x_r[i]);
    }
    printf("\n");

    // Take FFT
    ffte(x_r,x_i,N,1);
    
    // Print output
    printf("\nFFT:\n");
    for(int i=0; i< N; ++i)
    {
        printf("\nf: %f, Abs: %f",i*fs/N, sqrt(x_r[i]*x_r[i] + x_i[i] * x_i[i]));
    }
    printf("\n");

    // Take IFFT
    iffte(x_r,x_i,N,0);
    
    // Print output
    printf("\nOutput Signal:\n");
    for(int i=0; i< N; ++i)
    {
        printf("\nReal: %f",x_r[i]);
    }
    printf("\n");

    return 0;
}

