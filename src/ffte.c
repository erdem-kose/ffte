#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "ffte.h"
#include "ffte/subfuncs.h"

// FFT
void ffte(double x_real[], double x_imag[], size_t N, unsigned char only_real_input)
{
	if(N<2)
		return;
	else if((N & (N - 1)) == 0)
		ffte_cooleytukey(x_real, x_imag, N, only_real_input, 0);
	else
		ffte_bluestein(x_real, x_imag, N, only_real_input, 0);
}

// IFFT
void iffte(double x_real[], double x_imag[], size_t N, unsigned char only_real_input)
{
	if(N<2)
		return;
	else if((N & (N - 1)) == 0)
		ffte_cooleytukey(x_real, x_imag, N, only_real_input, 1);
	else
		ffte_bluestein(x_real, x_imag, N, only_real_input, 1);
}

// FFTSHIFT (Use before ffte)
void ffteshift(double x_real[], double x_imag[], size_t N, unsigned char only_real_input)
{
	if(N<2)
		return;
	{
		
	}
}

// IFFTSHIFT (Use after iffte)
void iffteshift(double x_real[], double x_imag[], size_t N, unsigned char only_real_input)
{
	if(N<2)
		return;
	else
	{

	}
}