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

	double PI=(inverse!=0)?(-M_PI):(M_PI);

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
		double t_r=cos(2.0 * PI * k / (double)N);
		double t_i=-sin(2.0 * PI * k / (double)N);
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

/*
//https://gist.github.com/lukicdarkoo/3f0d056e9244784f8b4a
void fft(int *x_in, 
	std::complex<double> *x_out,
	int N) {

	// Make copy of array and apply window
	for (int i = 0; i < N; i++) {
		x_out[i] = std::complex<double>(x_in[i], 0);
		x_out[i] *= 1; // Window
	}

	// Start recursion
	fft_rec(x_out, N);
}

void fft_rec(std::complex<double> *x, int N) {
	// Check if it is splitted enough
	if (N <= 1) {
		return;
	}

	// Split even and odd
	std::complex<double> odd[N/2];
	std::complex<double> even[N/2];
	for (int i = 0; i < N / 2; i++) {
		even[i] = x[i*2];
		odd[i] = x[i*2+1];
	}

	// Split on tasks
	fft_rec(even, N/2);
	fft_rec(odd, N/2);


	// Calculate DFT
	for (int k = 0; k < N / 2; k++) {
		std::complex<double> t = exp(std::complex<double>(0, -2 * M_PI * k / N)) * odd[k];
		x[k] = even[k] + t;
		x[N / 2 + k] = even[k] - t;
	}
}
*/