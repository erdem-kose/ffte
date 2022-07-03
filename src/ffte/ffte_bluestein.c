#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "subfuncs.h"
#include "../ffte.h"

static uint64_t nextpow2(uint64_t v);

void ffte_bluestein(double x_real[], double x_imag[], size_t N, unsigned char only_real_input, unsigned char inverse)
{
	uint64_t M = (2 * N) - 1;
	uint64_t M_pow2 = nextpow2(M);

	// wq calculation
	double PI_N = (inverse != 0) ? (-M_PI / (double)N) : (M_PI / (double)N);

	int64_t n = -(int64_t)(N - 1);

	double trig_param;

	double wq_r[M_pow2], wq_i[M_pow2];
	double wq_r_half[N];
	double wq_i_half[N];

	uint64_t j = N-1;
	for (uint64_t i = M_pow2-N+1; i < M_pow2; ++i)
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

	for (uint64_t i = N; i < M_pow2-N+1; ++i)
	{
		wq_r[i]=0;
		wq_i[i]=0;
	}

	// xq calculation
	double xq_r[M_pow2], xq_i[M_pow2];
	double r_tmp, i_tmp;

	if (only_real_input == 0)
	{
		for (uint64_t i = 0; i < N; ++i)
			cmplx_div(&xq_r[i], &xq_i[i], x_real[i], x_imag[i], wq_r_half[i], wq_i_half[i]);
	}
	else
	{
		for (uint64_t i = 0; i < N; ++i)
			cmplx_div(&xq_r[i], &xq_i[i], x_real[i], 0, wq_r_half[i], wq_i_half[i]);
	}

	for (uint64_t i = N; i < M_pow2; ++i)
	{
		xq_r[i]=0;
		xq_i[i]=0;
	}

	// Convolution
	ffte_cooleytukey(xq_r, xq_i, M_pow2, 0, 0);
	ffte_cooleytukey(wq_r, wq_i, M_pow2, 0, 0);

	for (uint64_t i = 0; i < M_pow2; ++i)
	{
		cmplx_mul(&xq_r[i], &xq_i[i], xq_r[i], xq_i[i], wq_r[i], wq_i[i]);
	}

	ffte_cooleytukey(xq_r, xq_i, M_pow2, 0, 1);

	// X calculation
	if(inverse!=0)
	{
		for (uint64_t i = 0; i < N; ++i)
		{
			cmplx_div(&x_real[i], &x_imag[i], xq_r[i]/N, xq_i[i]/N, wq_r_half[i], wq_i_half[i]);
		}
	}
	else
	{
		for (uint64_t i = 0; i < N; ++i)
		{
			cmplx_div(&x_real[i], &x_imag[i], xq_r[i], xq_i[i], wq_r_half[i], wq_i_half[i]);
		}
	}
}

static uint64_t nextpow2(uint64_t v)
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