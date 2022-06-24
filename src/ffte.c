#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "ffte.h"
#include "ffte/subfuncs.h"

// Main function
void ffte(float x_real[], float x_imag[], size_t N, unsigned char only_real_input, unsigned char inverse)
{
	if(N<2)
		return;
	else if((N & (N - 1)) == 0)
		ffte_cooleytukey(x_real, x_imag, N, only_real_input, inverse);
	else
		ffte_bluestein(x_real, x_imag, N, only_real_input, inverse);
}

