#include <cmath>
#include <cstdio>
#include <cstdint>
#include <cstdlib>

#include "subfuncs.hpp"
#include "../ffte.hpp"

template <typename T> void swap(T *X, uint64_t i, uint64_t j);

template <typename T> void ffte_pp_cooleytukey(T* x_real, T* x_imag, unsigned int N, bool only_real_input, bool inverse)
{
	int64_t M = log((T)N) / log(2.0);

	Cmplx<T> y;
	Cmplx<T> x1;
	Cmplx<T> x2;

	// wq calculation
	T PI_N = (inverse != 0) ? (-M_PI / (T)N) : (M_PI / (T)N);

	T trig_param;

	T wq_r[N], wq_i[N];

	int64_t i;
	for (i = 0; i < N; ++i)
	{
		trig_param = i * PI_N;
		wq_r[i] = cos(trig_param);
		wq_i[i] = sin(trig_param);
	}

	// If only real input enabled, fill x_imag with zeros
	if (only_real_input != 0)
	{
		for (i = 0; i < N; ++i)
		{
			x_imag[i] = 0;
		}
	}

	// Input reordering with bit reversal
	int64_t i_new, i_shift;

	if (only_real_input == 0)
	{
		for (i = 0; i < N; ++i)
		{
			i_new = 0;

			for (i_shift = 0; i_shift < M; ++i_shift)
			{
				i_new = (i_new << 1) | (1 & (i >> i_shift));
			}

			if (i_new < i)
			{
				swap<T>(x_real, i, i_new);
				swap<T>(x_imag, i, i_new);
			}
		}
	}
	else
	{
		for (i = 0; i < N; ++i)
		{
			i_new = 0;

			for (int64_t i_shift = 0; i_shift < M; ++i_shift)
			{
				i_new = (i_new << 1) | (1 & (i >> i_shift));
			}

			if (i_new < i)
			{
				swap<T>(x_real, i, i_new);
			}
		}
	}

	// log(N) stages
	int64_t k, j, j1, j2, j3, j3_fact;

	T r_tmp, i_tmp;
	
	int64_t n1 = 1;
	int64_t n2 = n1 << 1;

	for (i = 0; i < M; i++)
	{
		k = 0;
		j3_fact=N/n1;

		// N stages
		while (k < N - 1)
		{
			// Sections
			for (j = 0; j < n1; j++)
			{
				j1 = k + j;
				j2 = j1 + n1;
				j3 = j*j3_fact;

				x1.set(x_real[j2], x_imag[j2]);
				x2.set(wq_r[j3], wq_i[j3]);
				y=x1*x2;
				r_tmp=y.get_real();
				i_tmp=y.get_imag();

				x_real[j2] = x_real[j1] - r_tmp;
				x_imag[j2] = x_imag[j1] - i_tmp;
				x_real[j1] = x_real[j1] + r_tmp;
				x_imag[j1] = x_imag[j1] + i_tmp;
			}
			k += n2;
		}

		n1 = n2;
		n2 = n2 << 1;
	}

	if (inverse)
	{
		for (i = 0; i < N; i++)
		{
			x_real[i] /= N;
			x_imag[i] /= N;
		}
	}
}

template <typename T> void swap(T *X, uint64_t i, uint64_t j)
{
	T temp = X[i];
	X[i] = X[j];
	X[j] = temp;
}

template void swap<double>(double*, uint64_t, uint64_t);
template void swap<float>(float*, uint64_t, uint64_t);

template void ffte_pp_cooleytukey<double>(double*, double*, unsigned int, bool, bool);
template void ffte_pp_cooleytukey<float>(float*, float*, unsigned int, bool, bool);