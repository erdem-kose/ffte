#include <math.h>
#include <stdio.h>
#include "utils/plot.h"
#include "utils/exectime.h"
#include "src/ffte.h"

void test_ffte1d(unsigned int N)
{
    // Create data
    double x_r[N];
    double x_i[N];
    double f_axis[N];
    double t_axis[N];
    double f=9.375;//10 for N=100 , 9.375 for N=128
    double fs=200;

    for(int i=0; i< N; ++i)
    {
        x_r[i]=cos(2.0*M_PI*f/fs*((double)i));
        f_axis[i]=i*fs/N;
        t_axis[i]=i/fs;
    }

    // Execution time measurement variables
    double ffte_time, iffte_time;

    // Plot data
    plot("../ffte_x.pdf", t_axis, x_r , N, "t", "X");

    // Take FFT
    tic();
    ffte(x_r,x_i,N,1);
    ffte_time=toc();
    printf("\tFFTE takes %f secs \r\n", ffte_time);

    plot("../ffte_Xr.pdf", f_axis, x_r , N, "f", "X");
    plot("../ffte_Xi.pdf", f_axis, x_i , N, "f", "X");

    // Take IFFT
    tic();
    iffte(x_r,x_i,N,0);
    iffte_time=toc();
    printf("\tIFFTE takes %f secs \r\n", iffte_time);

    plot("../ffte_x_ifft.pdf", t_axis, x_r , N, "t", "X");
}

int main(int argc, char **argv )
{
    printf("\n\rNon-Power of 2 FFT test\r\n");
    test_ffte1d(1024*15); // TODO: 1024*63 gives segfault, look at that
    printf("Power of 2 FFT test\r\n");
    test_ffte1d(1024*16);

    return 0;
}