#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "subfuncs.h"
#include "../ffte.h"

static void ffte_ct_recurse(float x_real[], float x_imag[], size_t N, unsigned char only_real_input, unsigned char inverse)
{
	//https://gist.github.com/lukicdarkoo/3f0d056e9244784f8b4a
	
	if (N < 2) return;
	int64_t N_div_2=(N >> 1);

	// Split even and odd
	float even_r[N_div_2];
	float even_i[N_div_2];
	float odd_r[N_div_2];
	float odd_i[N_div_2];

	double PI2_N = (inverse!=0)? (-2.0 * M_PI / (double)N):(2.0 * M_PI / (double)N);

	if(only_real_input==0)
	{
		for (int64_t i = 0; i < N_div_2; i++) 
		{
			int64_t ii = i<<1;
			even_r[i] = x_real[ii];
			even_i[i] = x_imag[ii];

			odd_r[i] = x_real[ii|1];
			odd_i[i] = x_imag[ii|1];
		}
	}
	else
	{
		for (int64_t i = 0; i < N_div_2; i++) {
			int64_t ii = i<<1;
			even_r[i] = x_real[ii];
			even_i[i] = 0;

			odd_r[i] = x_real[ii|1];
			odd_i[i] = 0;
		}
	}

	// Split on tasks
	ffte_ct_recurse(even_r, even_i, N_div_2, 0, inverse);
	ffte_ct_recurse(odd_r, odd_i, N_div_2, 0, inverse);

	// Calculate DFT
	for (int64_t k = 0; k < N_div_2; k++)
	{
		double t_r=cos(PI2_N * k);
		double t_i=-sin(PI2_N * k);
		cmplx_mul(&t_r, &t_i, t_r, t_i, odd_r[k], odd_i[k]);

		x_real[k] = even_r[k] + t_r;
		x_imag[k] = even_i[k] + t_i;
		x_real[N_div_2 + k] = even_r[k] - t_r;
		x_imag[N_div_2 + k] = even_i[k] - t_i;
	}


}

inline void ffte_cooleytukey(float x_real[], float x_imag[], size_t N, unsigned char only_real_input, unsigned char inverse)
{
	ffte_ct_recurse(x_real, x_imag, N, only_real_input, inverse);
	if(inverse!=0)
	{
		for (int64_t k = 0; k < N; k++)
		{
			x_real[k]=x_real[k]/N;
			x_imag[k]=x_imag[k]/N;
		}
	}
}