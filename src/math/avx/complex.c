#ifdef FFTE_x86PARALLEL_ENABLE

#include <math.h>
#include <immintrin.h>

#include "../funcs.h"

void cmplx_mul(double *z_r, double *z_i, double x_r, double x_i, double y_r, double y_i)
{
	// https://mathworld.wolfram.com/ComplexMultiplication.html

	// Init inputs
	__m128d x = _mm_setr_pd(x_r, x_i);
	__m128d y = _mm_setr_pd(y_r, -y_i);
	__m128d y_perm = _mm_setr_pd(y_i, y_r);

	// Calculate output
	__m128d out = _mm_hadd_pd(_mm_mul_pd(x, y), _mm_mul_pd(x, y_perm));

	// Restore data
	*z_r=((double*)&out)[0];
	*z_i=((double*)&out)[1];
}

void cmplx_div(double *z_r, double *z_i, double x_r, double x_i, double y_r, double y_i)
{
	// https://mathworld.wolfram.com/ComplexMultiplication.html

	// Init inputs
	__m128d x = _mm_setr_pd(x_r, x_i);
	__m128d y = _mm_setr_pd(y_r, y_i);
	__m128d y_perm = _mm_setr_pd(-y_i, y_r);

	// Calculate division factor
	__m128d c = _mm_mul_pd(y, y);
	c = _mm_add_pd(c, _mm_permute_pd(c,0x1));

	// Calculate output
	__m128d out = _mm_hadd_pd(_mm_mul_pd(x, y), _mm_mul_pd(x, y_perm));
	out=_mm_div_pd(out,c);
	
	// Restore data
	*z_r=((double*)&out)[0];
	*z_i=((double*)&out)[1];
}

#endif
