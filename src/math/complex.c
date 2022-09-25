#include <math.h>

#include "funcs.h"

#ifdef FFTE_AVX_ENABLE
    #include <immintrin.h>
    inline void cmplx_mul(double *z_r, double *z_i, double x_r, double x_i, double y_r, double y_i)
	{
		// https://mathworld.wolfram.com/ComplexMultiplication.html
		__m256d mult;
		__m128d add_pt0, add_pt1;

		double out[2];

		// Multiply real-imaginary parts
		mult = _mm256_mul_pd(	_mm256_set_pd(x_r, x_i, -x_i, x_r),
								_mm256_set_pd(y_r, y_r, y_i, y_i));

		// Split into two parts of summation
		add_pt0 = _mm256_extractf128_pd (mult, 0);
		add_pt1 = _mm256_extractf128_pd (mult, 1);
		
		add_pt0 = _mm_add_pd(add_pt0, add_pt1);

		// Restore data
		_mm_store_pd(out, add_pt0);
		*z_i=out[0];
		*z_r=out[1];
	}

	inline void cmplx_div(double *z_r, double *z_i, double x_r, double x_i, double y_r, double y_i)
	{
		// https://mathworld.wolfram.com/ComplexDivision.html
		__m256d mult;
		__m128d add_pt0, add_pt1, div;
		__m128d c;

		double out[2];

		// Calculate division factor
		c = _mm_set_pd(y_r, y_i);
		c = _mm_mul_pd(c, c);
		_mm_store_pd(out, c);
		c = _mm_set1_pd(out[0]+out[1]);

		// Multiply real-imaginary parts
		mult = _mm256_mul_pd(	_mm256_set_pd(x_r, x_i, x_i, -x_r),
								_mm256_set_pd(y_r, y_r, y_i, y_i));

		// Split into two parts of summation
		add_pt0 = _mm256_extractf128_pd (mult, 0);
		add_pt1 = _mm256_extractf128_pd (mult, 1);

		add_pt0 = _mm_add_pd(add_pt0, add_pt1);

		// Divide to factor c;
		div = _mm_div_pd(add_pt0, c);

		// Restore data
		_mm_store_pd(out, div);

		*z_r=out[1];
		*z_i=out[0];
	}
#else
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
#endif