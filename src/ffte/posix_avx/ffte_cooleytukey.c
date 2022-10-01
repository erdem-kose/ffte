#ifdef FFTE_AVX_ENABLE

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "../funcs.h"
#include "../../math/funcs.h"

void swap(double *X, uint64_t i, uint64_t j);

void ffte_cooleytukey(double* x_real, double* x_imag, unsigned int N, unsigned char only_real_input, unsigned char inverse)
{
	int64_t M = log((double)N) / log(2.0);
	int64_t N_2=N/2;

	// wq calculation
	double PI_N = (inverse != 0) ? (-M_PI / (double)N) : (M_PI / (double)N);

	double wq_r[N], wq_i[N];

	int64_t i;
	for (i = 0; i < N_2; ++i)
	{
		wq_r[i] = cos(i * PI_N);
		wq_i[i+N_2] = wq_r[i];
	}
	for (; i < N; ++i)
	{
		wq_r[i] = cos(i * PI_N);
		wq_i[i-N_2] = -wq_r[i];
	}
	
	// If only real input enabled, fill x_imag with zeros
	if (only_real_input != 0)
	{
		for (i = 0; i < N; ++i)
		{
			x_imag[i] = 0;
		}
	}

	// Input reordering with bit reversal
	int64_t i_new, i_shift;

	if (only_real_input == 0)
	{
		for (i = 0; i < N; ++i)
		{
			i_new = 0;

			for (i_shift = 0; i_shift < M; ++i_shift)
			{
				i_new = (i_new << 1) | (1 & (i >> i_shift));
			}

			if (i_new < i)
			{
				swap(x_real, i, i_new);
				swap(x_imag, i, i_new);
			}
		}
	}
	else
	{
		for (i = 0; i < N; ++i)
		{
			i_new = 0;

			for (int64_t i_shift = 0; i_shift < M; ++i_shift)
			{
				i_new = (i_new << 1) | (1 & (i >> i_shift));
			}

			if (i_new < i)
			{
				swap(x_real, i, i_new);
			}
		}
	}

	// log(N) stages
	int64_t k, j, j1, j2, j3, j3_fact;

	double r_tmp, i_tmp;
	
	int64_t n1 = 1;
	int64_t n2 = n1 << 1;

	for (i = 0; i < M; i++)
	{
		k = 0;
		j3_fact=N/n1;

		// N stages
		while (k < N - 1)
		{
			// Sections
			for (j = 0; j < n1; j++)
			{
				j1 = k + j;
				j2 = j1 + n1;
				j3 = j*j3_fact;

				cmplx_mul(&r_tmp, &i_tmp, x_real[j2], x_imag[j2], wq_r[j3], wq_i[j3]);

				x_real[j2] = x_real[j1] - r_tmp;
				x_imag[j2] = x_imag[j1] - i_tmp;
				x_real[j1] = x_real[j1] + r_tmp;
				x_imag[j1] = x_imag[j1] + i_tmp;
			}
			k += n2;
		}

		n1 = n2;
		n2 = n2 << 1;
	}

	if (inverse)
	{
		for (i = 0; i < N; i++)
		{
			x_real[i] /= N;
			x_imag[i] /= N;
		}
	}
}

inline void swap(double *X, uint64_t i, uint64_t j)
{
	double temp = X[i];
	X[i] = X[j];
	X[j] = temp;
}

#endif
