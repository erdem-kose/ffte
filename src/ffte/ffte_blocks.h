#ifndef FFTE_BLOCKS_H
#define FFTE_BLOCKS_H

#include <stdlib.h>
#include "../math/ffte_math.h"

void ffte_bluestein(double* x_real, double* x_imag, unsigned int N, unsigned char only_real_input, unsigned char inverse);
void ffte_cooleytukey(double* x_real, double* x_imag, unsigned int N, unsigned char only_real_input, unsigned char inverse);

#endif