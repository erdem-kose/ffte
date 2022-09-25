#ifndef FFTE_MATHFUNCS_H
#define FFTE_MATHFUNCS_H

void cmplx_mul(double *z_r, double *z_i, double x_r, double x_i, double y_r, double y_i);
void cmplx_div(double *z_r, double *z_i, double x_r, double x_i, double y_r, double y_i);

#ifdef FFTE_AVX_ENABLE
    #include <immintrin.h>
    __m512 ffte_mm512_cos_ps(__m512 a);
    __m512 ffte_mm512_sin_ps(__m512 b);
#endif

#endif