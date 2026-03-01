// Copyright (c) [2022-2026] Erdem Kose

#include "tests.hpp"
#include <cmath>
#include <cstdio>
extern "C" {
    #include "../utils/plot.h"
    #include "../utils/exectime.h"
}
#include "../src/cpp/ffte.hpp"
#include "../src/cpp/ffte_dynfix.hpp"

// Convert DynFix array → double array (for plotting / comparison)
template <typename Tf, typename Ti>
static void dynfix_to_double(const DynFix<Tf,Ti>* src, double* dst, unsigned int N) {
    for (unsigned int i = 0; i < N; ++i)
        dst[i] = (double)src[i].toFloat();
}

// Peak bin index of |FFT| magnitude
static unsigned int peak_bin(const double* xr, const double* xi, unsigned int N) {
    double   best = -1.0;
    unsigned int k = 0;
    for (unsigned int i = 0; i < N; ++i) {
        double mag = xr[i]*xr[i] + xi[i]*xi[i];
        if (mag > best) { best = mag; k = i; }
    }
    return k;
}

// Shared test signal parameters
static constexpr double F_SIG  = 9.375;   // Hz
static constexpr double F_S    = 200.0;   // Hz
static constexpr uint8_t QM    = 8;       // Q(8.16): integer bits
static constexpr uint8_t QN    = 16;      // Q(8.16): fractional bits

static void test_ffte_fix_pow2(unsigned int N)
{
    // --- Build input: cosine, stored as Q(8.16) int32_t ---
    DynFix<double,int32_t> xr_fix[N], xi_fix[N];
    double xr_flt[N], xi_flt[N];
    double f_axis[N], t_axis[N];

    for (unsigned int i = 0; i < N; ++i) {
        double val  = cos(2.0 * M_PI * F_SIG / F_S * (double)i);
        xr_fix[i]  = DynFix<double,int32_t>(val, QM, QN);
        xi_fix[i]  = DynFix<double,int32_t>(0.0, QM, QN);
        xr_flt[i]  = val;
        xi_flt[i]  = 0.0;
        f_axis[i]  = (double)i * F_S / (double)N;
        t_axis[i]  = (double)i / F_S;
    }

    // --- Float FFT (reference) ---
    ffte_pp<double>(xr_flt, xi_flt, N, 1);

    // --- Fixed-point FFT ---
    double ffte_fix_time;
    tic();
    ffte_pp_fix<double,int32_t>(xr_fix, xi_fix, N, true);
    ffte_fix_time = toc();
    printf("\tFFTE(CPP-DynFix) takes %f secs \r\n", ffte_fix_time);

    // Convert to double for plots and comparison
    double xr_fix_d[N], xi_fix_d[N];
    dynfix_to_double(xr_fix, xr_fix_d, N);
    dynfix_to_double(xi_fix, xi_fix_d, N);

    plot("../tests/output/cpp_fix/ffte_cpp_fix_Xr_pow2.svg", f_axis, xr_fix_d, N, "f", "Xr_fix");
    plot("../tests/output/cpp_fix/ffte_cpp_fix_Xi_pow2.svg", f_axis, xi_fix_d, N, "f", "Xi_fix");

    // Both should peak at the same frequency bin
    unsigned int bin_flt = peak_bin(xr_flt, xi_flt, N);
    unsigned int bin_fix = peak_bin(xr_fix_d, xi_fix_d, N);
    EXPECT_EQ(bin_flt, bin_fix)
        << "Peak bin mismatch: float=" << bin_flt << " fix=" << bin_fix;

    // --- Fixed-point IFFT round-trip ---
    DynFix<double,int32_t> xr_rec[N], xi_rec[N];
    for (unsigned int i = 0; i < N; ++i) {
        xr_rec[i] = xr_fix[i];
        xi_rec[i] = xi_fix[i];
    }

    double iffte_fix_time;
    tic();
    iffte_pp_fix<double,int32_t>(xr_rec, xi_rec, N, false);
    iffte_fix_time = toc();
    printf("\tIFFTE(CPP-DynFix) takes %f secs \r\n", iffte_fix_time);

    double xr_rec_d[N];
    dynfix_to_double(xr_rec, xr_rec_d, N);
    plot("../tests/output/cpp_fix/ffte_cpp_fix_x_ifft_pow2.svg", t_axis, xr_rec_d, N, "t", "x_rec");

    // Recovered signal should match original (within fixed-point quantisation error)
    for (unsigned int i = 0; i < N; ++i) {
        double orig = cos(2.0 * M_PI * F_SIG / F_S * (double)i);
        EXPECT_NEAR(xr_rec_d[i], orig, 0.05)
            << "Round-trip error too large at sample " << i;
    }
}

static void test_ffte_fix_nonpow2(unsigned int N)
{
    DynFix<double,int32_t> xr_fix[N], xi_fix[N];
    double xr_flt[N], xi_flt[N];
    double f_axis[N], t_axis[N];

    for (unsigned int i = 0; i < N; ++i) {
        double val = cos(2.0 * M_PI * F_SIG / F_S * (double)i);
        xr_fix[i] = DynFix<double,int32_t>(val, QM, QN);
        xi_fix[i] = DynFix<double,int32_t>(0.0, QM, QN);
        xr_flt[i] = val;
        xi_flt[i] = 0.0;
        f_axis[i] = (double)i * F_S / (double)N;
        t_axis[i] = (double)i / F_S;
    }

    // Float reference
    ffte_pp<double>(xr_flt, xi_flt, N, 1);

    // Fixed-point FFT
    double ffte_fix_time;
    tic();
    ffte_pp_fix<double,int32_t>(xr_fix, xi_fix, N, true);
    ffte_fix_time = toc();
    printf("\tFFTE(CPP-DynFix) NonPow2 takes %f secs \r\n", ffte_fix_time);

    double xr_fix_d[N], xi_fix_d[N];
    dynfix_to_double(xr_fix, xr_fix_d, N);
    dynfix_to_double(xi_fix, xi_fix_d, N);

    plot("../tests/output/cpp_fix/ffte_cpp_fix_Xr_nonpow2.svg", f_axis, xr_fix_d, N, "f", "Xr_fix");
    plot("../tests/output/cpp_fix/ffte_cpp_fix_Xi_nonpow2.svg", f_axis, xi_fix_d, N, "f", "Xi_fix");

    unsigned int bin_flt = peak_bin(xr_flt, xi_flt, N);
    unsigned int bin_fix = peak_bin(xr_fix_d, xi_fix_d, N);
    // For real cosine input, conjugate symmetry gives |X[k]| == |X[N-k]|.
    // Quantisation noise can tip the balance, so accept either bin.
    EXPECT_TRUE(bin_fix == bin_flt || bin_fix == N - bin_flt)
        << "Peak bin " << bin_fix
        << " is neither expected bin " << bin_flt
        << " nor its conjugate " << (N - bin_flt);

    // Round-trip
    DynFix<double,int32_t> xr_rec[N], xi_rec[N];
    for (unsigned int i = 0; i < N; ++i) {
        xr_rec[i] = xr_fix[i];
        xi_rec[i] = xi_fix[i];
    }

    double iffte_fix_time;
    tic();
    iffte_pp_fix<double,int32_t>(xr_rec, xi_rec, N, false);
    iffte_fix_time = toc();
    printf("\tIFFTE(CPP-DynFix) NonPow2 takes %f secs \r\n", iffte_fix_time);

    double xr_rec_d[N];
    dynfix_to_double(xr_rec, xr_rec_d, N);
    plot("../tests/output/cpp_fix/ffte_cpp_fix_x_ifft_nonpow2.svg", t_axis, xr_rec_d, N, "t", "x_rec");

    for (unsigned int i = 0; i < N; ++i) {
        double orig = cos(2.0 * M_PI * F_SIG / F_S * (double)i);
        EXPECT_NEAR(xr_rec_d[i], orig, 0.05)
            << "Round-trip error too large at sample " << i;
    }
}

TEST(ffte_fix_test, pow2_fft_ifft) {
    printf("\n\rFixed-point FFT test — power-of-2 (N=128, Q(8.16) int32)\n");
    test_ffte_fix_pow2(8 * 16);   // N = 128
}

TEST(ffte_fix_test, nonpow2_fft_ifft) {
    printf("\n\rFixed-point FFT test — non-power-of-2 (N=120, Q(8.16) int32)\n");
    test_ffte_fix_nonpow2(8 * 15);  // N = 120
}
