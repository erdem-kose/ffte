#pragma once

template <typename T> void ffte_pp(T* x_real, T* x_imag, unsigned int N, bool only_real_input);
template <typename T> void iffte_pp(T* x_real, T* x_imag, unsigned int N, bool only_real_input);

template <typename T> void ffte2d_pp(T** x_real, T** x_imag, unsigned int M, unsigned int N, bool only_real_input);
template <typename T> void iffte2d_pp(T** x_real, T** x_imag, unsigned int M, unsigned int N, bool only_real_input);
