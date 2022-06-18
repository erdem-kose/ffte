#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "ffte.h"

// Prototypes
void cmplx_mul(float *z_r, float *z_i, float x_r, float x_i, float y_r, float y_i);
void cmplx_div(float *z_r, float *z_i, float x_r, float x_i, float y_r, float y_i);
void ffte_bluestein(float x_real[], float x_imag[], size_t N, unsigned char real_input, unsigned char inverse);

// Main function
void ffte(float x_real[], float x_imag[], size_t N, unsigned char real_input, unsigned char inverse)
{
	if(N<2)
		return;
	else if((N & (N - 1)) == 0)
		ffte_bluestein(x_real, x_imag, N, real_input, inverse);// will be replaced with Radix type algorithm
	else
		ffte_bluestein(x_real, x_imag, N, real_input, inverse);
}

/*
* Bluestein nonpowerof2 
*/
inline void ffte_bluestein(float x_real[], float x_imag[], size_t N, unsigned char real_input, unsigned char inverse)
{
	// n axis creation
	uint64_t M = (2 * N) - 1;
	int64_t n[M];

	n[0] = -(int64_t)(N - 1);
	for (uint64_t i = 1; i < M; ++i)
		n[i] = n[i - 1] + 1;

	// wq calculation
	float trig_param;
	float wq_r[M];
	float wq_i[M];

	for (uint64_t i = 0; i < M; ++i)
	{
		trig_param = M_PI * n[i] * n[i] / N;
		if (inverse != 0)
			trig_param = -trig_param;
		wq_r[i] = cosf(trig_param);
		wq_i[i] = sinf(trig_param);
	}

	// xq calculation
	float xq_r[N];
	float xq_i[N];

	if(real_input==0)
	{
		for (uint64_t i = 0; i < N; ++i)
			cmplx_div(&xq_r[i], &xq_i[i], x_real[i], x_imag[i], wq_r[i+N-1], wq_i[i+N-1]);
	}
	else
	{
		for (uint64_t i = 0; i < N; ++i)
			cmplx_div(&xq_r[i], &xq_i[i], x_real[i], 0, wq_r[i+N-1], wq_i[i+N-1]);
	}


	// X calculation
	float X_r[N];
	float X_i[N];

	uint64_t jj;
	for (uint64_t j = N; j < M; ++j)
	{
		// To be threaded loop
		jj = j - N;
		X_r[jj]=0;
		X_i[jj]=0;
		for (uint64_t i = 0; i < N; ++i)
		{
			float r_tmp;
			float i_tmp;
			cmplx_mul(&r_tmp, &i_tmp, xq_r[i],xq_i[i],wq_r[j-i-1],wq_i[j-i-1]);
			X_r[jj]+=r_tmp;
			X_i[jj]+=i_tmp;
		}
		cmplx_div(&X_r[jj], &X_i[jj], X_r[jj],X_i[jj], wq_r[j-1], wq_i[j-1]);
		if (inverse != 0)
			cmplx_div(&X_r[jj], &X_i[jj], X_r[jj], X_i[jj], N, 0);
	}

	// Copy X to input
	for (uint64_t i = 0; i < N; ++i)
	{
		x_real[i]=X_r[i];
		x_imag[i]=X_i[i];
	}
}

/*
 Complex arithmetic
*/

inline void cmplx_mul(float *z_r, float *z_i, float x_r, float x_i, float y_r, float y_i)
{
	// https://mathworld.wolfram.com/ComplexMultiplication.html
	*z_r = (x_r * y_r) - (x_i * y_i);
	*z_i = (x_i * y_r) + (x_r * y_i);
}

inline void cmplx_div(float *z_r, float *z_i, float x_r, float x_i, float y_r, float y_i)
{
	// https://mathworld.wolfram.com/ComplexDivision.html
	float c = y_r * y_r + y_i * y_i;
	*z_r = ((x_r * y_r) + (x_i * y_i)) / c;
	*z_i = ((x_i * y_r) - (x_r * y_i)) / c;
}