#pragma once

#include <cstdint>
#include "ffte/subfuncs.hpp"

template <typename Tf, typename Ti>
void ffte_pp_fix(DynFix<Tf,Ti>* x_real, DynFix<Tf,Ti>* x_imag,
                 unsigned int N, bool only_real_input);

template <typename Tf, typename Ti>
void iffte_pp_fix(DynFix<Tf,Ti>* x_real, DynFix<Tf,Ti>* x_imag,
                  unsigned int N, bool only_real_input);
