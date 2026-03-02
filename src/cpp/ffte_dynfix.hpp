// Copyright (c) [2022-2026] Erdem Kose

#pragma once

#include <cstdint>
#include "ffte_dynfix/subfuncs.hpp"

template <typename Tf, typename Ti>
void ffte_cpp_dynfix(DynFix<Tf,Ti>* x_real, DynFix<Tf,Ti>* x_imag,
                 unsigned int N, bool only_real_input);

template <typename Tf, typename Ti>
void iffte_cpp_dynfix(DynFix<Tf,Ti>* x_real, DynFix<Tf,Ti>* x_imag,
                  unsigned int N, bool only_real_input);

template <typename Tf, typename Ti>
void ffte2d_cpp_dynfix(DynFix<Tf,Ti>** x_real, DynFix<Tf,Ti>** x_imag,
                   unsigned int M, unsigned int N, bool only_real_input);

template <typename Tf, typename Ti>
void iffte2d_cpp_dynfix(DynFix<Tf,Ti>** x_real, DynFix<Tf,Ti>** x_imag,
                    unsigned int M, unsigned int N, bool only_real_input);
