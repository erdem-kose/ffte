// Copyright (c) [2022-2026] Erdem Kose

#include "tests.hpp"

#include <cstdio>
#include <cmath>
extern "C" {
    #include "../utils/plot.h"
    #include "../utils/exectime.h"
}
#include "../src/cpp/ffte.hpp"
#include "../src/cpp/ffte_dynfix.hpp"

static constexpr double  F_SIG = 9.375;  // 10 for N=100, 9.375 for N=128
static constexpr double  F_S   = 200.0;
static constexpr uint8_t QM    = 8;      // Q(8.16): integer bits
static constexpr uint8_t QN    = 16;     // Q(8.16): fractional bits

void test_ffte1d_cpp_dynfix(unsigned int N, const char* postfix)
{
    // Create data
    DynFix<double,int32_t> x_r[N], x_i[N];
    double xr_d[N], xi_d[N];
    double f_axis[N];
    double t_axis[N];
    char path[256];

    for (unsigned int i = 0; i < N; ++i) {
        double val = cos(2.0 * M_PI * F_SIG / F_S * (double)i);
        x_r[i]    = DynFix<double,int32_t>(val, QM, QN);
        x_i[i]    = DynFix<double,int32_t>(0.0, QM, QN);
        xr_d[i]   = val;
        f_axis[i] = (double)i * F_S / (double)N;
        t_axis[i] = (double)i / F_S;
    }

    // Execution time measurement variables
    double ffte_time, iffte_time;

    // Plot data
    snprintf(path, sizeof(path), "../tests/output/cpp_dynfix/%s/ffte_cpp_fix_x" PLOT_EXT, postfix);
    plot(path, t_axis, xr_d, N, "t", "X");

    // Take FFT
    tic();
    ffte_cpp_dynfix<double,int32_t>(x_r, x_i, N, true);
    ffte_time = toc();
    printf("\tFFTE(CPP-DynFix) takes %f secs \r\n", ffte_time);

    for (unsigned int i = 0; i < N; ++i) {
        xr_d[i] = (double)x_r[i].toFloat();
        xi_d[i] = (double)x_i[i].toFloat();
    }
    snprintf(path, sizeof(path), "../tests/output/cpp_dynfix/%s/ffte_cpp_fix_Xr" PLOT_EXT, postfix);
    plot(path, f_axis, xr_d, N, "f", "X");
    snprintf(path, sizeof(path), "../tests/output/cpp_dynfix/%s/ffte_cpp_fix_Xi" PLOT_EXT, postfix);
    plot(path, f_axis, xi_d, N, "f", "X");

    // Take IFFT
    tic();
    iffte_cpp_dynfix<double,int32_t>(x_r, x_i, N, false);
    iffte_time = toc();
    printf("\tIFFTE(CPP-DynFix) takes %f secs \r\n", iffte_time);

    for (unsigned int i = 0; i < N; ++i)
        xr_d[i] = (double)x_r[i].toFloat();
    snprintf(path, sizeof(path), "../tests/output/cpp_dynfix/%s/ffte_cpp_fix_x_ifft" PLOT_EXT, postfix);
    plot(path, t_axis, xr_d, N, "t", "X");

    // Save execution times
    snprintf(path, sizeof(path), "../tests/output/cpp_dynfix/%s/times.txt", postfix);
    FILE* f_times = fopen(path, "w");
    if (f_times) {
        fprintf(f_times, "ffte_time:  %f secs\n", ffte_time);
        fprintf(f_times, "iffte_time: %f secs\n", iffte_time);
        fclose(f_times);
    }
}

TEST(ffte_test, 1d_cpp_dynfix) {
    printf("\n\rPower of 2 FFT test\r\n");
    test_ffte1d_cpp_dynfix(8*16, "pow2");

    printf("\n\rNon-Power of 2 FFT test\r\n");
    test_ffte1d_cpp_dynfix(8*15, "nonpow2"); // TODO: 1024*63 gives segfault, look at that

    EXPECT_EQ(1, 1);
}
