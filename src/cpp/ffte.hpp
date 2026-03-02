// Copyright (c) [2022-2026] Erdem Kose

#pragma once

template <typename T> void ffte_cpp(T* x_real, T* x_imag, unsigned int N, bool only_real_input);
template <typename T> void iffte_cpp(T* x_real, T* x_imag, unsigned int N, bool only_real_input);

template <typename T> void ffte2d_cpp(T** x_real, T** x_imag, unsigned int M, unsigned int N, bool only_real_input);
template <typename T> void iffte2d_cpp(T** x_real, T** x_imag, unsigned int M, unsigned int N, bool only_real_input);
