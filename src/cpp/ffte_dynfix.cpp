// Copyright (c) [2022-2026] Erdem Kose

#include "ffte_dynfix.hpp"
#include "ffte/subfuncs.hpp"

template <typename Tf, typename Ti>
void ffte_pp_fix(DynFix<Tf,Ti>* x_real, DynFix<Tf,Ti>* x_imag,
                 unsigned int N, bool only_real_input)
{
    if (N < 2) return;
    if ((N & (N - 1)) == 0)
        ffte_pp_cooleytukey_fix<Tf,Ti>(x_real, x_imag, N, only_real_input, false);
    else
        ffte_pp_bluestein_fix<Tf,Ti>(x_real, x_imag, N, only_real_input, false);
}

template void ffte_pp_fix<double, int64_t>(DynFix<double,int64_t>*, DynFix<double,int64_t>*, unsigned int, bool);
template void ffte_pp_fix<double, int32_t>(DynFix<double,int32_t>*, DynFix<double,int32_t>*, unsigned int, bool);
template void ffte_pp_fix<float,  int32_t>(DynFix<float, int32_t>*, DynFix<float, int32_t>*, unsigned int, bool);
template void ffte_pp_fix<float,  int16_t>(DynFix<float, int16_t>*, DynFix<float, int16_t>*, unsigned int, bool);

template <typename Tf, typename Ti>
void iffte_pp_fix(DynFix<Tf,Ti>* x_real, DynFix<Tf,Ti>* x_imag,
                  unsigned int N, bool only_real_input)
{
    if (N < 2) return;
    if ((N & (N - 1)) == 0)
        ffte_pp_cooleytukey_fix<Tf,Ti>(x_real, x_imag, N, only_real_input, true);
    else
        ffte_pp_bluestein_fix<Tf,Ti>(x_real, x_imag, N, only_real_input, true);
}

template void iffte_pp_fix<double, int64_t>(DynFix<double,int64_t>*, DynFix<double,int64_t>*, unsigned int, bool);
template void iffte_pp_fix<double, int32_t>(DynFix<double,int32_t>*, DynFix<double,int32_t>*, unsigned int, bool);
template void iffte_pp_fix<float,  int32_t>(DynFix<float, int32_t>*, DynFix<float, int32_t>*, unsigned int, bool);
template void iffte_pp_fix<float,  int16_t>(DynFix<float, int16_t>*, DynFix<float, int16_t>*, unsigned int, bool);
