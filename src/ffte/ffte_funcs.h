#ifndef FFTE_SUBFUNCS_H
#define FFTE_SUBFUNCS_H

void ffte_bluestein(double* x_real, double* x_imag, unsigned int N, unsigned char only_real_input, unsigned char inverse);
void ffte_cooleytukey(double* x_real, double* x_imag, unsigned int N, unsigned char only_real_input, unsigned char inverse);

#endif