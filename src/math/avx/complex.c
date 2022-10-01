#ifdef FFTE_AVX_ENABLE

#include <math.h>
#include <immintrin.h>

#include "../funcs.h"

void cmplx_mul(double *z_r, double *z_i, double x_r, double x_i, double y_r, double y_i)
{
	// https://mathworld.wolfram.com/ComplexMultiplication.html
	__m256d mult;
	__m128d add_pt0, add_pt1;
	__m128d out;

	// Multiply real-imaginary parts
	mult = _mm256_mul_pd(	_mm256_set_pd(x_r, x_i, -x_i, x_r),
							_mm256_set_pd(y_r, y_r, y_i, y_i));

	// Split into two parts of summation
	add_pt0 = _mm256_extractf128_pd (mult, 0);
	add_pt1 = _mm256_extractf128_pd (mult, 1);
	
	out = _mm_add_pd(add_pt0, add_pt1);

	// Restore data
	*z_i=((double*)&out)[0];
	*z_r=((double*)&out)[1];
}

void cmplx_div(double *z_r, double *z_i, double x_r, double x_i, double y_r, double y_i)
{
	// https://mathworld.wolfram.com/ComplexDivision.html
	__m256d mult;
	__m128d add_pt0, add_pt1;
	__m128d c, out;

	// Calculate division factor
	c = _mm_set_pd(y_r, y_i);
	c = _mm_mul_pd(c, c);
	c = _mm_set1_pd(((double*)&c)[0] + ((double*)&c)[1]);

	// Multiply real-imaginary parts
	mult = _mm256_mul_pd(	_mm256_set_pd(x_r, x_i, x_i, -x_r),
							_mm256_set_pd(y_r, y_r, y_i, y_i));

	// Split into two parts of summation
	add_pt0 = _mm256_extractf128_pd (mult, 0);
	add_pt1 = _mm256_extractf128_pd (mult, 1);

	out = _mm_add_pd(add_pt0, add_pt1);

	// Divide to factor c;
	out = _mm_div_pd(out, c);

	// Restore data
	*z_i=((double*)&out)[0];
	*z_r=((double*)&out)[1];
}

#endif
