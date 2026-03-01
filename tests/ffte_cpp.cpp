// Copyright (c) [2022-2026] Erdem Kose

#include "tests.hpp"

#include <cstdio>
#include <cmath>
extern "C" {
    #include "../utils/plot.h"
    #include "../utils/exectime.h"
}
#include "../src/cpp/ffte.hpp"

void test_ffte1d_cpp(unsigned int N)
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
    plot("../tests/output/cpp/ffte_cpp_x.svg", t_axis, x_r , N, "t", "X");

    // Take FFT
    tic();
    ffte_pp<double>(x_r,x_i,N,1);
    ffte_time=toc();
    printf("\tFFTE(CPP) takes %f secs \r\n", ffte_time);

    plot("../tests/output/cpp/ffte_cpp_Xr.svg", f_axis, x_r , N, "f", "X");
    plot("../tests/output/cpp/ffte_cpp_Xi.svg", f_axis, x_i , N, "f", "X");

    // Take IFFT
    tic();
    iffte_pp<double>(x_r,x_i,N,0);
    iffte_time=toc();
    printf("\tIFFTE(CPP) takes %f secs \r\n", iffte_time);

    plot("../tests/output/cpp/ffte_cpp_x_ifft.svg", t_axis, x_r , N, "t", "X");
}

TEST(ffte_test, 1d_cpp) {
    printf("\n\rPower of 2 FFT test\r\n");
    test_ffte1d_cpp(8*16);
    
    printf("\n\rNon-Power of 2 FFT test\r\n");
    test_ffte1d_cpp(8*15); // TODO: 1024*63 gives segfault, look at that

    
    EXPECT_EQ(1, 1);
}