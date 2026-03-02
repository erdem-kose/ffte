// Copyright (c) [2022-2026] Erdem Kose

#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "ffte.hpp"
#include "ffte/subfuncs.hpp"

template <typename T>
static void ffte2d_cpp_base(T** x_real, T** x_imag, unsigned int M, unsigned int N, bool only_real_input, bool isinverse)
{
    unsigned int i, j;

    // Process rows (along N)
    for (i = 0; i < M; ++i)
    {
        if (N < 2) {}
        else if ((N & (N - 1)) == 0)
            ffte_cpp_cooleytukey<T>(x_real[i], x_imag[i], N, only_real_input, isinverse);
        else
            ffte_cpp_bluestein<T>(x_real[i], x_imag[i], N, only_real_input, isinverse);
    }

    // Process columns (along M)
    T* x_real_tmp = new T[M];
    T* x_imag_tmp = new T[M];

    for (j = 0; j < N; ++j)
    {
        for (i = 0; i < M; ++i)
        {
            x_real_tmp[i] = x_real[i][j];
            x_imag_tmp[i] = x_imag[i][j];
        }

        if (M < 2) {}
        else if ((M & (M - 1)) == 0)
            ffte_cpp_cooleytukey<T>(x_real_tmp, x_imag_tmp, M, false, isinverse);
        else
            ffte_cpp_bluestein<T>(x_real_tmp, x_imag_tmp, M, false, isinverse);

        for (i = 0; i < M; ++i)
        {
            x_real[i][j] = x_real_tmp[i];
            x_imag[i][j] = x_imag_tmp[i];
        }
    }

    delete[] x_real_tmp;
    delete[] x_imag_tmp;
}

template <typename T>
void ffte2d_cpp(T** x_real, T** x_imag, unsigned int M, unsigned int N, bool only_real_input)
{
    ffte2d_cpp_base<T>(x_real, x_imag, M, N, only_real_input, false);
}

template void ffte2d_cpp<double>(double**, double**, unsigned int, unsigned int, bool);
template void ffte2d_cpp<float>(float**, float**, unsigned int, unsigned int, bool);

template <typename T>
void iffte2d_cpp(T** x_real, T** x_imag, unsigned int M, unsigned int N, bool only_real_input)
{
    ffte2d_cpp_base<T>(x_real, x_imag, M, N, only_real_input, true);
}

template void iffte2d_cpp<double>(double**, double**, unsigned int, unsigned int, bool);
template void iffte2d_cpp<float>(float**, float**, unsigned int, unsigned int, bool);
