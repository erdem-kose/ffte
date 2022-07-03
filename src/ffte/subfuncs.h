#ifndef FFTE_SUBFUNCS_H
#define FFTE_SUBFUNCS_H

void cmplx_mul(double *z_r, double *z_i, double x_r, double x_i, double y_r, double y_i);
void cmplx_div(double *z_r, double *z_i, double x_r, double x_i, double y_r, double y_i);

void ffte_bluestein(float x_real[], float x_imag[], size_t N, unsigned char only_real_input, unsigned char inverse);
void ffte_cooleytukey(float x_real[], float x_imag[], size_t N, unsigned char only_real_input, unsigned char inverse);

#endif