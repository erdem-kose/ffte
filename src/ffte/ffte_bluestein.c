#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "subfuncs.h"
#include "../ffte.h"

inline void ffte_bluestein(float x_real[], float x_imag[], size_t N, unsigned char only_real_input, unsigned char inverse)
{
	// n axis creation
	uint64_t M = (2 * N) - 1;
	int64_t n[M];

	n[0] = -(int64_t)(N - 1);
	for (uint64_t i = 1; i < M; ++i)
		n[i] = n[i - 1] + 1;

	// wq calculation
	double trig_param;
	double wq_r[M];
	double wq_i[M];

	for (uint64_t i = 0; i < M; ++i)
	{
		trig_param = M_PI * n[i] * n[i] / N;
		if (inverse != 0)
			trig_param = -trig_param;
		wq_r[i] = cos(trig_param);
		wq_i[i] = sin(trig_param);
	}

	// xq calculation
	double xq_r[N];
	double xq_i[N];

	if(only_real_input==0)
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
	float* X_r=x_real;
	float* X_i=x_imag;

	uint64_t jj;
	for (uint64_t j = N; j <= M; ++j)
	{
		// To be threaded loop
		jj = j - N;

        double X_r_tmp=0;
        double X_i_tmp=0;
        double r_tmp;
        double i_tmp;
		for (uint64_t i = 0; i < N; ++i)
		{
			cmplx_mul(&r_tmp, &i_tmp, xq_r[i], xq_i[i],wq_r[j-i-1],wq_i[j-i-1]);
			X_r_tmp+=r_tmp;
			X_i_tmp+=i_tmp;
		}

		cmplx_div(&X_r_tmp, &X_i_tmp, X_r_tmp, X_i_tmp, wq_r[j-1], wq_i[j-1]);
		if (inverse != 0)
			cmplx_div(&X_r_tmp, &X_i_tmp, X_r_tmp, X_i_tmp, N, 0);

        X_r[jj]=X_r_tmp;
        X_i[jj]=X_i_tmp;
	}
}