

#include "ffte_math.h"

void cmplx_mul(double *z_r, double *z_i, double x_r, double x_i, double y_r, double y_i)
{
	// https://mathworld.wolfram.com/ComplexMultiplication.html
	*z_r = (x_r * y_r) - (x_i * y_i);
	*z_i = (x_i * y_r) + (x_r * y_i);
}

void cmplx_div(double *z_r, double *z_i, double x_r, double x_i, double y_r, double y_i)
{
	// https://mathworld.wolfram.com/ComplexDivision.html
	double c = y_r * y_r + y_i * y_i;
	*z_r = ((x_r * y_r) + (x_i * y_i)) / c;
	*z_i = ((x_i * y_r) - (x_r * y_i)) / c;
}

uint64_t nextpow2(uint64_t v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v |= v >> 32;
	v++;
	return v;
}