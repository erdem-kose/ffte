#include <cmath>
#include <cstdio>
#include <cstdint>
#include <cstdlib>

#include "subfuncs.hpp"
#include "../ffte.hpp"

static uint64_t nextpow2(uint64_t v);

template <typename T> void ffte_pp_bluestein(T* x_real, T* x_imag, unsigned int N, bool only_real_input, bool inverse)
{
	uint64_t M = (2 * N) - 1;
	uint64_t M_pow2 = nextpow2(M);

	Cmplx<T> y;
	Cmplx<T> x1;
	Cmplx<T> x2;

	// wq calculation
	T PI_N = (inverse != 0) ? ((T)-M_PI / (T)N) : ((T)M_PI / (T)N);

	int64_t n = -(int64_t)(N - 1);

	T trig_param;

	T wq_r[M_pow2], wq_i[M_pow2];
	T wq_r_half[N];
	T wq_i_half[N];

	uint64_t i;
	uint64_t j = N-1;
	for (i = M_pow2-N+1; i < M_pow2; ++i)
	{
		trig_param = PI_N * n * n;

		wq_r[i] = cos(trig_param);
		wq_r[j] = wq_r[i];
		wq_r_half[j] = wq_r[i];
		wq_i[i] = sin(trig_param);
		wq_i[j] = wq_i[i];
		wq_i_half[j] = wq_i[i];

		--j;
		++n;
	}
	wq_r[0] = 1;
	wq_r_half[0] = 1;
	wq_i[0] = 0;
	wq_i_half[0] = 0;

	for (i = N; i < M_pow2-N+1; ++i)
	{
		wq_r[i]=0;
		wq_i[i]=0;
	}

	// xq calculation
	T xq_r[M_pow2], xq_i[M_pow2];
	T r_tmp, i_tmp;

	for (i = 0; i < N; ++i)
	{
		if (only_real_input == 0)
			x1.set(x_real[i], x_imag[i]);
		else
			x1.set(x_real[i], 0);
		x2.set(wq_r_half[i], wq_i_half[i]);
		y=x1/x2;
		xq_r[i]=y.get_real();
		xq_i[i]=y.get_imag();
	}

	for (i = N; i < M_pow2; ++i)
	{
		xq_r[i]=0;
		xq_i[i]=0;
	}

	// Convolution
	ffte_pp_cooleytukey(xq_r, xq_i, M_pow2, 0, 0);
	ffte_pp_cooleytukey(wq_r, wq_i, M_pow2, 0, 0);

	for (i = 0; i < M_pow2; ++i)
	{
		x1.set(xq_r[i], xq_i[i]);
		x2.set(wq_r[i], wq_i[i]);
		y=x1*x2;
		xq_r[i]=y.get_real();
		xq_i[i]=y.get_imag();
	}

	ffte_pp_cooleytukey(xq_r, xq_i, M_pow2, 0, 1);

	// X calculation
	for (i = 0; i < N; ++i)
	{
		if(inverse!=0)
			x1.set(xq_r[i]/N, xq_i[i]/N);
		else
			x1.set(xq_r[i], xq_i[i]);
		x2.set(wq_r_half[i], wq_i_half[i]);
		y=x1/x2;
		x_real[i]=y.get_real();
		x_imag[i]=y.get_imag();
	}
}

inline uint64_t nextpow2(uint64_t v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v |= v >> 32;
	v++;
	return v;
}

template void ffte_pp_bluestein<double>(double*, double*, unsigned int, bool, bool);
template void ffte_pp_bluestein<float>(float*, float*, unsigned int, bool, bool);