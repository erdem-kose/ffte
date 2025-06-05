#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "ffte.h"
#include "ffte/subfuncs.h"

void ffte2d_base(double** x_real, double** x_imag, unsigned int M, unsigned int  N, unsigned char only_real_input, unsigned char isinverse);

// FFT
void ffte2d(double** x_real, double** x_imag, unsigned int M, unsigned int N, unsigned char only_real_input)
{
	ffte2d_base(x_real, x_imag, M, N, only_real_input, 0);
}

// IFFT
void iffte2d(double** x_real, double** x_imag, unsigned int M, unsigned int  N, unsigned char only_real_input)
{
	ffte2d_base(x_real, x_imag, M, N, only_real_input, 1);
}

// FFT2D Base
inline void ffte2d_base(double** x_real, double** x_imag, unsigned int M, unsigned int  N, unsigned char only_real_input, unsigned char isinverse)
{
	unsigned int i, j;

	//Process first index
	if(N<2)
	{}
	else if((N & (N - 1)) == 0)
	{
		for(i=0; i<M; ++i)
			ffte_cooleytukey(x_real[i], x_imag[i], N, only_real_input, isinverse);
	}
	else
	{
		for(i=0; i<M; ++i)
			ffte_bluestein(x_real[i], x_imag[i], N, only_real_input, isinverse);
	}

	//Process second index
	double x_real_tmp[M];
	double x_imag_tmp[M];

	if(M<2)
	{}
	else if((M & (M - 1)) == 0)
	{
		for(j=0; j<N; ++j)
		{
			for(i=0; i<M; ++i)
			{
				x_real_tmp[i]=x_real[i][j];
				x_imag_tmp[i]=x_imag[i][j];
			}

			ffte_cooleytukey(x_real_tmp, x_imag_tmp, M, 0, isinverse);

			for(i=0; i<M; ++i)
			{
				x_real[i][j]=x_real_tmp[i];
				x_imag[i][j]=x_imag_tmp[i];
			}
		}
	}
	else
	{
		for(j=0; j<N; ++j)
		{
			for(i=0; i<M; ++i)
			{
				x_real_tmp[i]=x_real[i][j];
				x_imag_tmp[i]=x_imag[i][j];
			}

			ffte_bluestein(x_real_tmp, x_imag_tmp, M, 0, isinverse);

			for(i=0; i<M; ++i)
			{
				x_real[i][j]=x_real_tmp[i];
				x_imag[i][j]=x_imag_tmp[i];
			}
		}
	}
}

