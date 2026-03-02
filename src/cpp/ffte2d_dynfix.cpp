// Copyright (c) [2022-2026] Erdem Kose

#include "ffte_dynfix.hpp"
#include "ffte_dynfix/subfuncs.hpp"

template <typename Tf, typename Ti>
static void ffte2d_cpp_dynfix_base(DynFix<Tf,Ti>** x_real, DynFix<Tf,Ti>** x_imag,
                                unsigned int M, unsigned int N, bool only_real_input, bool isinverse)
{
    unsigned int i, j;

    // Process rows (along N)
    for (i = 0; i < M; ++i)
    {
        if (N < 2) {}
        else if ((N & (N - 1)) == 0)
            ffte_cpp_cooleytukey_dynfix<Tf,Ti>(x_real[i], x_imag[i], N, only_real_input, isinverse);
        else
            ffte_cpp_bluestein_dynfix<Tf,Ti>(x_real[i], x_imag[i], N, only_real_input, isinverse);
    }

    // Process columns (along M)
    DynFix<Tf,Ti>* x_real_tmp = new DynFix<Tf,Ti>[M];
    DynFix<Tf,Ti>* x_imag_tmp = new DynFix<Tf,Ti>[M];

    for (j = 0; j < N; ++j)
    {
        for (i = 0; i < M; ++i)
        {
            x_real_tmp[i] = x_real[i][j];
            x_imag_tmp[i] = x_imag[i][j];
        }

        if (M < 2) {}
        else if ((M & (M - 1)) == 0)
            ffte_cpp_cooleytukey_dynfix<Tf,Ti>(x_real_tmp, x_imag_tmp, M, false, isinverse);
        else
            ffte_cpp_bluestein_dynfix<Tf,Ti>(x_real_tmp, x_imag_tmp, M, false, isinverse);

        for (i = 0; i < M; ++i)
        {
            x_real[i][j] = x_real_tmp[i];
            x_imag[i][j] = x_imag_tmp[i];
        }
    }

    delete[] x_real_tmp;
    delete[] x_imag_tmp;
}

template <typename Tf, typename Ti>
void ffte2d_cpp_dynfix(DynFix<Tf,Ti>** x_real, DynFix<Tf,Ti>** x_imag,
                   unsigned int M, unsigned int N, bool only_real_input)
{
    ffte2d_cpp_dynfix_base<Tf,Ti>(x_real, x_imag, M, N, only_real_input, false);
}

template void ffte2d_cpp_dynfix<double, int64_t>(DynFix<double,int64_t>**, DynFix<double,int64_t>**, unsigned int, unsigned int, bool);
template void ffte2d_cpp_dynfix<double, int32_t>(DynFix<double,int32_t>**, DynFix<double,int32_t>**, unsigned int, unsigned int, bool);
template void ffte2d_cpp_dynfix<float,  int32_t>(DynFix<float, int32_t>**, DynFix<float, int32_t>**, unsigned int, unsigned int, bool);
template void ffte2d_cpp_dynfix<float,  int16_t>(DynFix<float, int16_t>**, DynFix<float, int16_t>**, unsigned int, unsigned int, bool);

template <typename Tf, typename Ti>
void iffte2d_cpp_dynfix(DynFix<Tf,Ti>** x_real, DynFix<Tf,Ti>** x_imag,
                    unsigned int M, unsigned int N, bool only_real_input)
{
    ffte2d_cpp_dynfix_base<Tf,Ti>(x_real, x_imag, M, N, only_real_input, true);
}

template void iffte2d_cpp_dynfix<double, int64_t>(DynFix<double,int64_t>**, DynFix<double,int64_t>**, unsigned int, unsigned int, bool);
template void iffte2d_cpp_dynfix<double, int32_t>(DynFix<double,int32_t>**, DynFix<double,int32_t>**, unsigned int, unsigned int, bool);
template void iffte2d_cpp_dynfix<float,  int32_t>(DynFix<float, int32_t>**, DynFix<float, int32_t>**, unsigned int, unsigned int, bool);
template void iffte2d_cpp_dynfix<float,  int16_t>(DynFix<float, int16_t>**, DynFix<float, int16_t>**, unsigned int, unsigned int, bool);
