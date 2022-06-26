#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "ffte.h"
#include "ffte/subfuncs.h"

// FFT
void ffte(float x_real[], float x_imag[], size_t N, unsigned char only_real_input)
{
	if(N<2)
		return;
	else if((N & (N - 1)) == 0)
		ffte_cooleytukey(x_real, x_imag, N, only_real_input, 0);
	else
		ffte_bluestein(x_real, x_imag, N, only_real_input, 0);
}

// IFFT
void iffte(float x_real[], float x_imag[], size_t N, unsigned char only_real_input)
{
	if(N<2)
		return;
	else if((N & (N - 1)) == 0)
		ffte_cooleytukey(x_real, x_imag, N, only_real_input, 1);
	else
		ffte_bluestein(x_real, x_imag, N, only_real_input, 1);
}

// FFTSHIFT (Use before ffte)
void ffteshift(float x_real[], float x_imag[], size_t N, unsigned char only_real_input)
{
	if(N<2)
		return;
	{
		
	}
}

// IFFTSHIFT (Use after iffte)
void iffteshift(float x_real[], float x_imag[], size_t N, unsigned char only_real_input)
{
	if(N<2)
		return;
	else
	{

	}
}