#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "subfuncs.h"
#include "../ffte.h"

inline void ffte_bluestein(float x_real[], float x_imag[], size_t N, unsigned char only_real_input, unsigned char inverse)
{
	uint64_t M = (2 * N) - 1;

	// wq calculation
	double PI_N = (inverse!=0)? (-M_PI  / (double)N):(M_PI  / (double)N);

	int64_t n=-(int64_t)(N - 1);

	double trig_param;

	double wq_r[M], wq_i[M];

	uint64_t j=0;
	for (uint64_t i = 0; i < N; ++i)
	{
		trig_param = PI_N * n * n;
		j=M-i-1;
		wq_r[i] = cos(trig_param);
		wq_r[j] = wq_r[i];
		wq_i[i] = sin(trig_param);
		wq_i[j] = wq_i[i];
		++n;
	}

	// xq calculation
	double xq_r[N], xq_i[N];

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

    double X_r_tmp, X_i_tmp, r_tmp, i_tmp;
		
	uint64_t jj;
	for (uint64_t j = N; j <= M; ++j)
	{
		jj = j - N;

        X_r_tmp=0;
        X_i_tmp=0;

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