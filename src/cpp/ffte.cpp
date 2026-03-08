// Copyright (c) [2022-2026] Erdem Kose

#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "ffte.hpp"
#include "ffte/subfuncs.hpp"

// FFT
template <typename T> void ffte_cpp(T* x_real, T* x_imag, unsigned int N, bool only_real_input)
{
	if(N<2)
		return;
	else if((N & (N - 1)) == 0)
		ffte_cpp_cooleytukey<T>(x_real, x_imag, N, only_real_input, 0);  // power-of-2: Cooley-Tukey
	else
		ffte_cpp_bluestein<T>(x_real, x_imag, N, only_real_input, 0);    // non-power-of-2: Bluestein
}

template void ffte_cpp<double>(double*, double*, unsigned int, bool);
template void ffte_cpp<float>(float*, float*, unsigned int, bool);

// IFFT
template <typename T> void iffte_cpp(T* x_real, T* x_imag, unsigned int N, bool only_real_input)
{
	if(N<2)
		return;
	else if((N & (N - 1)) == 0)
		ffte_cpp_cooleytukey<T>(x_real, x_imag, N, only_real_input, 1);  // power-of-2: Cooley-Tukey
	else
		ffte_cpp_bluestein<T>(x_real, x_imag, N, only_real_input, 1);    // non-power-of-2: Bluestein
}

template void iffte_cpp<double>(double*, double*, unsigned int, bool);
template void iffte_cpp<float>(float*, float*, unsigned int, bool);