// Copyright (c) [2022-2026] Erdem Kose

#pragma once

#include <cstdint>
#include "ffte_dynfix/subfuncs.hpp"

// 1D FFT / IFFT using dynamic fixed-point arithmetic (Cooley-Tukey for power-of-2, Bluestein otherwise)
template <typename Tf, typename Ti>
void ffte_cpp_dynfix(DynFix<Tf,Ti>* x_real, DynFix<Tf,Ti>* x_imag,
                 unsigned int N, bool only_real_input);

template <typename Tf, typename Ti>
void iffte_cpp_dynfix(DynFix<Tf,Ti>* x_real, DynFix<Tf,Ti>* x_imag,
                  unsigned int N, bool only_real_input);

// 2D FFT / IFFT using dynamic fixed-point arithmetic (row-wise then column-wise)
template <typename Tf, typename Ti>
void ffte2d_cpp_dynfix(DynFix<Tf,Ti>** x_real, DynFix<Tf,Ti>** x_imag,
                   unsigned int M, unsigned int N, bool only_real_input);

template <typename Tf, typename Ti>
void iffte2d_cpp_dynfix(DynFix<Tf,Ti>** x_real, DynFix<Tf,Ti>** x_imag,
                    unsigned int M, unsigned int N, bool only_real_input);
