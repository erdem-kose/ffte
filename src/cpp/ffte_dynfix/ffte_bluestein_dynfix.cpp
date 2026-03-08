// Copyright (c) [2022-2026] Erdem Kose

#include <cmath>
#include <cstdint>

#include "subfuncs.hpp"

static uint64_t nextpow2_fix(uint64_t v) {
    v--;
    v |= v >> 1;  v |= v >> 2;  v |= v >> 4;
    v |= v >> 8;  v |= v >> 16; v |= v >> 32;
    return ++v;
}

template <typename Tf, typename Ti>
void ffte_cpp_bluestein_dynfix(DynFix<Tf,Ti>* x_real, DynFix<Tf,Ti>* x_imag,
                            unsigned int N, bool only_real_input, bool inverse)
{
    const uint8_t sig_m = x_real[0].getM();
    const uint8_t qn    = x_real[0].getN();

    const uint64_t M      = (2 * N) - 1;
    const uint64_t M_pow2 = nextpow2_fix(M);

    // wq calculation
    Tf PI_N = inverse ? (-(Tf)M_PI / (Tf)N) : ((Tf)M_PI / (Tf)N);

    DynFix<Tf,Ti> wq_r[M_pow2], wq_i[M_pow2];
    DynFix<Tf,Ti> wq_r_half[N], wq_i_half[N];

    int64_t  idx = -(int64_t)(N - 1);
    uint64_t j   = N - 1;

    for (uint64_t i = M_pow2 - N + 1; i < M_pow2; ++i) {
        Tf param   = PI_N * (Tf)idx * (Tf)idx;
        Tf cos_val = (Tf)cos(param);
        Tf sin_val = (Tf)sin(param);

        wq_r[i]      = DynFix<Tf,Ti>(cos_val, 1, qn);
        wq_r[j]      = wq_r[i];
        wq_r_half[j] = wq_r[i];
        wq_i[i]      = DynFix<Tf,Ti>(sin_val, 1, qn);
        wq_i[j]      = wq_i[i];
        wq_i_half[j] = wq_i[i];

        --j;
        ++idx;
    }

    wq_r[0]      = DynFix<Tf,Ti>((Tf)1.0, 1, qn);
    wq_r_half[0] = DynFix<Tf,Ti>((Tf)1.0, 1, qn);
    wq_i[0]      = DynFix<Tf,Ti>((Tf)0.0, 1, qn);
    wq_i_half[0] = DynFix<Tf,Ti>((Tf)0.0, 1, qn);

    DynFix<Tf,Ti> zero_tw((Tf)0.0, 1, qn);
    for (uint64_t i = N; i < M_pow2 - N + 1; ++i) {
        wq_r[i] = zero_tw;
        wq_i[i] = zero_tw;
    }

    // xq calculation
    DynFix<Tf,Ti>    xq_r[M_pow2], xq_i[M_pow2];
    DynFix<Tf,Ti>    zero_sig((Tf)0.0, sig_m, qn);
    DynFixCmplx<Tf,Ti> x1, x2, y;

    for (uint64_t i = 0; i < N; ++i) {
        if (only_real_input)
            x1.set(x_real[i], zero_sig);
        else
            x1.set(x_real[i], x_imag[i]);

        x2.set(wq_r_half[i], wq_i_half[i]);
        y       = x1 / x2;
        xq_r[i] = y.get_real();
        xq_i[i] = y.get_imag();
    }

    for (uint64_t i = N; i < M_pow2; ++i) {
        xq_r[i] = zero_sig;
        xq_i[i] = zero_sig;
    }

    // Convolution
    ffte_cpp_cooleytukey_dynfix<Tf,Ti>(xq_r, xq_i, (unsigned int)M_pow2, false, false);
    ffte_cpp_cooleytukey_dynfix<Tf,Ti>(wq_r, wq_i, (unsigned int)M_pow2, false, false);

    for (uint64_t i = 0; i < M_pow2; ++i) {
        x1.set(xq_r[i], xq_i[i]);
        x2.set(wq_r[i], wq_i[i]);
        y       = x1 * x2;
        xq_r[i] = y.get_real();
        xq_i[i] = y.get_imag();
    }

    ffte_cpp_cooleytukey_dynfix<Tf,Ti>(xq_r, xq_i, (unsigned int)M_pow2, false, true);

    // X calculation
    DynFix<Tf,Ti> inv_N((Tf)1.0 / (Tf)N, 1, qn);

    for (uint64_t i = 0; i < N; ++i) {
        if (inverse)
            x1.set(xq_r[i] * inv_N, xq_i[i] * inv_N);
        else
            x1.set(xq_r[i], xq_i[i]);

        x2.set(wq_r_half[i], wq_i_half[i]);
        y         = x1 / x2;
        x_real[i] = y.get_real();
        x_imag[i] = y.get_imag();
    }
}

template void ffte_cpp_bluestein_dynfix<double, int64_t>(DynFix<double,int64_t>*, DynFix<double,int64_t>*, unsigned int, bool, bool);
template void ffte_cpp_bluestein_dynfix<double, int32_t>(DynFix<double,int32_t>*, DynFix<double,int32_t>*, unsigned int, bool, bool);
template void ffte_cpp_bluestein_dynfix<float,  int32_t>(DynFix<float, int32_t>*, DynFix<float, int32_t>*, unsigned int, bool, bool);
template void ffte_cpp_bluestein_dynfix<float,  int16_t>(DynFix<float, int16_t>*, DynFix<float, int16_t>*, unsigned int, bool, bool);
