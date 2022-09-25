#ifndef FFTE_MATHFUNCS_H
#define FFTE_MATHFUNCS_H

void cmplx_mul(double *z_r, double *z_i, double x_r, double x_i, double y_r, double y_i);
void cmplx_div(double *z_r, double *z_i, double x_r, double x_i, double y_r, double y_i);

#ifdef FFTE_AVX_ENABLE
    #include <immintrin.h>
    __m256d ffte_mm256_cos_pd(__m256d x);
    __m256d ffte_mm256_sin_pd(__m256d x);
#endif

#endif