#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "subfuncs.h"
#include "../ffte.h"

void swap(float* X, uint64_t i, uint64_t j);

void ffte_cooleytukey(float x_real[], float x_imag[], size_t N, unsigned char only_real_input, unsigned char inverse)
{
	// Parameter calculation
	double PI_N = (inverse!=0)? (-M_PI):(M_PI);
	int64_t M = log((double)N) / log(2.0);

	// If only real input enabled, fill x_imag with zeros
	if (only_real_input != 0)
	{
		for (int64_t i = 0; i < N; ++i)
		{
			x_imag[i] = 0;
		}
	}

	// Input reordering with bit reversal 
	int64_t i_new;
	double arg;

	if (only_real_input == 0)
	{
		for (int64_t i = 0; i < N; ++i)
		{
			i_new = 0;

			for (int64_t i_shift = 0; i_shift < M; ++i_shift)
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
		for (int64_t i = 0; i < N; ++i)
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
	int64_t n1 = 1;
	int64_t n2 = n1 << 1;

	int64_t k, j1, j2;

	double w, s, c;
 	double r_tmp, i_tmp;

	for (int64_t i = 0; i < M; i++)
	{
		w = PI_N / n1;

		k = 0;

		// N stages
		while (k < N - 1)
		{
			// Sections
			for (int64_t j = 0; j < n1; j++)
			{
				arg = -j * w;

				c = cos(arg);
				s = sin(arg);

				j1 = k + j;
				j2 = j1 + n1;

				cmplx_mul(&r_tmp, &i_tmp, x_real[j2], x_imag[j2], c , s);

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
		for (int64_t i = 0; i < N; i++)
		{
			x_real[i] /= N;
			x_imag[i] /= N;
		}
	}
}

inline void swap(float* X, uint64_t i, uint64_t j)
{
	float temp = X[i];
	X[i]=X[j];
	X[j]= temp;
}