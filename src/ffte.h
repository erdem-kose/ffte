#ifndef FFTE_H_
#define FFTE_H_

void ffteshift(float x_real[], float x_imag[], size_t N, unsigned char only_real_input);
void iffteshift(float x_real[], float x_imag[], size_t N, unsigned char only_real_input);

void ffte(float x_real[], float x_imag[], size_t N, unsigned char only_real_input);
void iffte(float x_real[], float x_imag[], size_t N, unsigned char only_real_input);

#endif
