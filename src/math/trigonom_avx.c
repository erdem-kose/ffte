#ifdef FFTE_AVX_ENABLE

#include <math.h>
#include "funcs.h"

#define FACTORIAL_MAX 20
double fact[FACTORIAL_MAX];

__m256d ffte_mm256_cos_pd(__m256d x)
{
	__m256d phase=_mm256_set1_pd(M_PI_2);
	x=_mm256_sub_pd(phase,x);
	return ffte_mm256_sin_pd(x);
}

__m256d ffte_mm256_sin_pd(__m256d x)
{
	//Taylor

	// Init
	__m256d y;

	__m256d x2 = _mm256_mul_pd(x, x);
	
	double fact;

	// Process
	y=x;

	x=_mm256_mul_pd(x,x2);
	y=_mm256_sub_pd(y, _mm256_div_pd(x, _mm256_set1_pd(6.0))); //3!

	x=_mm256_mul_pd(x,x2);
	y=_mm256_add_pd(y, _mm256_div_pd(x, _mm256_set1_pd(120.0))); //5!

	x=_mm256_mul_pd(x,x2);
	y=_mm256_sub_pd(y, _mm256_div_pd(x, _mm256_set1_pd(5040.0))); //7!

	x=_mm256_mul_pd(x,x2);
	y=_mm256_add_pd(y, _mm256_div_pd(x, _mm256_set1_pd(362880.0))); //9!

	x=_mm256_mul_pd(x,x2);
	y=_mm256_sub_pd(y, _mm256_div_pd(x, _mm256_set1_pd(39916800.0))); //11!

	x=_mm256_mul_pd(x,x2);
	y=_mm256_add_pd(y, _mm256_div_pd(x, _mm256_set1_pd(6227020800.0))); //13!

	return y;
}


#endif
