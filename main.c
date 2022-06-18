#include <math.h>
#include <stdio.h>
#include "src/ffte.h"

int main()
{
    // Create data
    unsigned int N = 100;
    float x_r[N];
    float x_i[N];

    float f=250;
    float fs=1000;

    for(int i=0; i< N; ++i)
    {
        x_r[i]=cos(2*M_PI*f/fs*i);
    }
    
    // Print data
    printf("Input Signal:\n");
    for(int i=0; i< N; ++i)
    {
        printf("\nReal: %f",x_r[i]);
    }
    printf("\n");

    // Take FFT
    ffte(x_r,x_i,N,1,0);
    
    // Print output
    printf("FFT:\n");
    for(int i=0; i< N; ++i)
    {
        printf("\nf: %f, Abs: %f",i*fs/N, sqrt(x_r[i]*x_r[i] + x_i[i] * x_i[i])/N);
    }

    return 0;
}

