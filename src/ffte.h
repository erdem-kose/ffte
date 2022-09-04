#ifndef FFTE_H_
#define FFTE_H_

void ffte(double* x_real, double* x_imag, unsigned int N, unsigned char only_real_input);
void iffte(double* x_real, double* x_imag, unsigned int N, unsigned char only_real_input);

void ffte2d(double** x_real, double** x_imag, unsigned int M, unsigned int N, unsigned char only_real_input);
void iffte2d(double** x_real, double** x_imag, unsigned int M, unsigned int N, unsigned char only_real_input);

#endif
