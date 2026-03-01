// Copyright (c) [2022-2026] Erdem Kose

#include <cmath>
#include <cstdint>

#include "subfuncs.hpp"

template <typename Tf, typename Ti>
static void swap_fix(DynFix<Tf,Ti>* X, uint64_t i, uint64_t j) {
    DynFix<Tf,Ti> tmp = X[i];
    X[i] = X[j];
    X[j] = tmp;
}

template <typename Tf, typename Ti>
void ffte_pp_cooleytukey_fix(DynFix<Tf,Ti>* x_real, DynFix<Tf,Ti>* x_imag,
                              unsigned int N, bool only_real_input, bool inverse)
{
    const uint8_t sig_m = x_real[0].getM();
    const uint8_t qn    = x_real[0].getN();

    int64_t stages = (int64_t)(log((Tf)N) / log((Tf)2));

    // wq calculation
    Tf PI_N = inverse ? (-(Tf)M_PI / (Tf)N) : ((Tf)M_PI / (Tf)N);

    DynFix<Tf,Ti> wq_r[N], wq_i[N];
    for (int64_t i = 0; i < (int64_t)N; ++i) {
        Tf param = (Tf)i * PI_N;
        wq_r[i] = DynFix<Tf,Ti>((Tf)cos(param), 1, qn);
        wq_i[i] = DynFix<Tf,Ti>((Tf)sin(param), 1, qn);
    }

    // If only real input enabled, fill x_imag with zeros
    if (only_real_input) {
        DynFix<Tf,Ti> zero((Tf)0.0, sig_m, qn);
        for (int64_t i = 0; i < (int64_t)N; ++i)
            x_imag[i] = zero;
    }

    // Input reordering with bit reversal
    for (int64_t i = 0; i < (int64_t)N; ++i) {
        int64_t i_rev = 0;
        for (int64_t s = 0; s < stages; ++s)
            i_rev = (i_rev << 1) | (1 & (i >> s));
        if (i_rev < i) {
            swap_fix<Tf,Ti>(x_real, i, i_rev);
            swap_fix<Tf,Ti>(x_imag, i, i_rev);
        }
    }

    // log(N) stages
    DynFixCmplx<Tf,Ti> x1, x2, y;
    DynFix<Tf,Ti>      r_tmp, i_tmp;

    int64_t n1 = 1;
    int64_t n2 = 2;

    for (int64_t i = 0; i < stages; ++i) {
        int64_t k      = 0;
        int64_t stride = N / n1;

        // N stages
        while (k < (int64_t)N - 1) {
            // Sections
            for (int64_t j = 0; j < n1; ++j) {
                int64_t j1 = k + j;
                int64_t j2 = j1 + n1;
                int64_t j3 = j * stride;

                x1.set(x_real[j2], x_imag[j2]);
                x2.set(wq_r[j3],   wq_i[j3]);
                y     = x1 * x2;
                r_tmp = y.get_real();
                i_tmp = y.get_imag();

                x_real[j2] = x_real[j1] - r_tmp;
                x_imag[j2] = x_imag[j1] - i_tmp;
                x_real[j1] = x_real[j1] + r_tmp;
                x_imag[j1] = x_imag[j1] + i_tmp;
            }
            k += n2;
        }

        n1 = n2;
        n2 <<= 1;
    }

    if (inverse) {
        DynFix<Tf,Ti> inv_N((Tf)1.0 / (Tf)N, 1, qn);
        for (int64_t i = 0; i < (int64_t)N; ++i) {
            x_real[i] = x_real[i] * inv_N;
            x_imag[i] = x_imag[i] * inv_N;
        }
    }
}

template void ffte_pp_cooleytukey_fix<double, int64_t>(DynFix<double,int64_t>*, DynFix<double,int64_t>*, unsigned int, bool, bool);
template void ffte_pp_cooleytukey_fix<double, int32_t>(DynFix<double,int32_t>*, DynFix<double,int32_t>*, unsigned int, bool, bool);
template void ffte_pp_cooleytukey_fix<float,  int32_t>(DynFix<float, int32_t>*, DynFix<float, int32_t>*, unsigned int, bool, bool);
template void ffte_pp_cooleytukey_fix<float,  int16_t>(DynFix<float, int16_t>*, DynFix<float, int16_t>*, unsigned int, bool, bool);
