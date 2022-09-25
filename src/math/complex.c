#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "funcs.h"

inline void cmplx_mul(double *z_r, double *z_i, double x_r, double x_i, double y_r, double y_i)
{
	// https://mathworld.wolfram.com/ComplexMultiplication.html
	*z_r = (x_r * y_r) - (x_i * y_i);
	*z_i = (x_i * y_r) + (x_r * y_i);
}

inline void cmplx_div(double *z_r, double *z_i, double x_r, double x_i, double y_r, double y_i)
{
	// https://mathworld.wolfram.com/ComplexDivision.html
	double c = y_r * y_r + y_i * y_i;
	*z_r = ((x_r * y_r) + (x_i * y_i)) / c;
	*z_i = ((x_i * y_r) - (x_r * y_i)) / c;
}