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
		ffte_pp_bluestein<T>(x_real, x_imag, N, only_real_input, 0);
	else
		ffte_pp_bluestein<T>(x_real, x_imag, N, only_real_input, 0);
}

template void ffte_pp<double>(double*, double*, unsigned int, bool);
template void ffte_pp<float>(float*, float*, unsigned int, bool);

// IFFT
template <typename T> void iffte_pp(T* x_real, T* x_imag, unsigned int N, bool only_real_input)
{
	if(N<2)
		return;
	else if((N & (N - 1)) == 0)
		ffte_pp_cooleytukey<T>(x_real, x_imag, N, only_real_input, 1);
	else
		ffte_pp_cooleytukey<T>(x_real, x_imag, N, only_real_input, 1);
}

template void iffte_pp<double>(double*, double*, unsigned int, bool);
template void iffte_pp<float>(float*, float*, unsigned int, bool);