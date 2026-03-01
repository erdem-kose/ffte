// Copyright (c) [2022-2026] Erdem Kose

#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "ffte.hpp"
#include "ffte/subfuncs.hpp"

// FFT
template <typename T> void ffte_pp(T* x_real, T* x_imag, unsigned int N, bool only_real_input)
{
	if(N<2)
		return;
	else if((N & (N - 1)) == 0)
		ffte_pp_cooleytukey<T>(x_real, x_imag, N, only_real_input, 0);  // power-of-2: Cooley-Tukey
	else
		ffte_pp_bluestein<T>(x_real, x_imag, N, only_real_input, 0);    // non-power-of-2: Bluestein
}

template void ffte_pp<double>(double*, double*, unsigned int, bool);
template void ffte_pp<float>(float*, float*, unsigned int, bool);

// IFFT
template <typename T> void iffte_pp(T* x_real, T* x_imag, unsigned int N, bool only_real_input)
{
	if(N<2)
		return;
	else if((N & (N - 1)) == 0)
		ffte_pp_cooleytukey<T>(x_real, x_imag, N, only_real_input, 1);  // power-of-2: Cooley-Tukey
	else
		ffte_pp_bluestein<T>(x_real, x_imag, N, only_real_input, 1);    // non-power-of-2: Bluestein
}

template void iffte_pp<double>(double*, double*, unsigned int, bool);
template void iffte_pp<float>(float*, float*, unsigned int, bool);