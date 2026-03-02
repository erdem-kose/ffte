
// Copyright (c) [2022-2026] Erdem Kose

#include "tests.hpp"

extern "C" {
    #include <stdio.h>
    #include <math.h>
    #include "../utils/plot.h"
    #include "../utils/exectime.h"
    #include "../src/c/ffte.h"

    void test_ffte1d(unsigned int N, const char* postfix)
    {
        // Create data
        double x_r[N];
        double x_i[N];
        double f_axis[N];
        double t_axis[N];
        double f=9.375;//10 for N=100 , 9.375 for N=128
        double fs=200;
        char path[256];

        for(int i=0; i< N; ++i)
        {
            x_r[i]=cos(2.0*M_PI*f/fs*((double)i));
            f_axis[i]=i*fs/N;
            t_axis[i]=i/fs;
        }

        // Execution time measurement variables
        double ffte_time, iffte_time;

        // Plot data
        snprintf(path, sizeof(path), "../tests/output/c/%s/ffte_x" PLOT_EXT, postfix);
        plot(path, t_axis, x_r , N, "t", "X");

        // Take FFT
        tic();
        ffte(x_r,x_i,N,1);
        ffte_time=toc();
        printf("\tFFTE(C) takes %f secs \r\n", ffte_time);

        snprintf(path, sizeof(path), "../tests/output/c/%s/ffte_Xr" PLOT_EXT, postfix);
        plot(path, f_axis, x_r , N, "f", "X");
        snprintf(path, sizeof(path), "../tests/output/c/%s/ffte_Xi" PLOT_EXT, postfix);
        plot(path, f_axis, x_i , N, "f", "X");

        // Take IFFT
        tic();
        iffte(x_r,x_i,N,0);
        iffte_time=toc();
        printf("\tIFFTE(C) takes %f secs \r\n", iffte_time);

        snprintf(path, sizeof(path), "../tests/output/c/%s/ffte_x_ifft" PLOT_EXT, postfix);
        plot(path, t_axis, x_r , N, "t", "X");

        // Save execution times
        snprintf(path, sizeof(path), "../tests/output/c/%s/times.txt", postfix);
        FILE* f_times = fopen(path, "w");
        if (f_times) {
            fprintf(f_times, "ffte_time:  %f secs\n", ffte_time);
            fprintf(f_times, "iffte_time: %f secs\n", iffte_time);
            fclose(f_times);
        }
    }
}

TEST(ffte_test, 1d_c) {
    printf("\n\rPower of 2 FFT test\r\n");
    test_ffte1d(8*16, "pow2");

    printf("\n\rNon-Power of 2 FFT test\r\n");
    test_ffte1d(8*15, "nonpow2"); // TODO: 1024*63 gives segfault, look at that


    EXPECT_EQ(1, 1);
}
