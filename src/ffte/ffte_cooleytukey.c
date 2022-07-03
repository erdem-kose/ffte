#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "subfuncs.h"
#include "../ffte.h"

static void swap(float* a, float* b)
{
	float temp = *a;
	*a=*b;
	*b= temp;
}

void ffte_cooleytukey(float x_real[], float x_imag[], size_t N, unsigned char only_real, unsigned char inverse)
{
	// Parameter calculation
	double PI_N = (inverse!=0)? (-M_PI):(M_PI);
	int64_t M = log((double)N) / log(2.0);

	// If only real input enabled, fill x_imag with zeros
	if (only_real != 0)
	{
		for (int64_t i = 0; i < N; ++i)
		{
			x_imag[i] = 0;
		}
	}

	// Perform bit reversal.
	int64_t j, k;
	double arg;

	for (int64_t i = 0; i < N; ++i)
	{
		j = 0;

		for (k = 0; k < M; ++k)
		{
			j = (j << 1) | (1 & (i >> k));
		}

		if (j < i)
		{
			swap(&x_real[i], &x_real[j]);
			swap(&x_imag[i], &x_imag[j]);
		}
	}

	// For log N stages.
	int64_t n1 = 1;
	int64_t n2 = n1 << 1;

	int64_t j1, j2;

	double w, s, c;
 	double r_tmp, i_tmp;

	for (int64_t i = 0; i < M; i++)
	{
		w = PI_N / n1;

		k = 0;

		// For N components.
		while (k < N - 1)
		{
			// For each section.
			for (j = 0; j < n1; j++)
			{
				arg = -j * w;
				c = cos(arg);
				s = sin(arg);
				j1 = k + j;
				j2 = j1 + n1;
				r_tmp = x_real[j2] * c - x_imag[j1 + n1] * s;
				i_tmp = x_imag[j2] * c + x_real[j2] * s;
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