#ifndef FFTE_H_
#define FFTE_H_

void ffteshift(double x_real[], double x_imag[], size_t N, unsigned char only_real_input);
void iffteshift(double x_real[], double x_imag[], size_t N, unsigned char only_real_input);

void ffte(double x_real[], double x_imag[], size_t N, unsigned char only_real_input);
void iffte(double x_real[], double x_imag[], size_t N, unsigned char only_real_input);

#endif
