// Copyright (c) [2022-2026] Erdem Kose

#include "subfuncs.hpp"
#include <cstdint>

// --- Constructors ---

template <typename Tf, typename Ti>
DynFix<Tf,Ti>::DynFix() : Q(0), m(0), n(0) {}

// From raw integer (e.g. already-scaled value from hardware or prior conversion)
template <typename Tf, typename Ti>
DynFix<Tf,Ti>::DynFix(Ti raw, uint8_t m, uint8_t n) : Q(raw), m(m), n(n) {}

// From float: converts val to fixed-point by scaling by 2^n
template <typename Tf, typename Ti>
DynFix<Tf,Ti>::DynFix(Tf val, uint8_t m, uint8_t n)
    : Q(static_cast<Ti>(val * static_cast<Tf>(int64_t(1) << n))), m(m), n(n) {}

// --- Accessors ---

template <typename Tf, typename Ti>
void DynFix<Tf,Ti>::set(Tf val) {
    Q = static_cast<Ti>(val * static_cast<Tf>(int64_t(1) << n));
}

template <typename Tf, typename Ti>
Tf DynFix<Tf,Ti>::toFloat() const {
    return static_cast<Tf>(Q) / static_cast<Tf>(int64_t(1) << n);
}

template <typename Tf, typename Ti>
Ti DynFix<Tf,Ti>::raw() const { return Q; }

template <typename Tf, typename Ti>
uint8_t DynFix<Tf,Ti>::getM() const { return m; }

template <typename Tf, typename Ti>
uint8_t DynFix<Tf,Ti>::getN() const { return n; }

// --- Arithmetic operators ---
// All operators assume both operands share the same Q(m.n) format.

template <typename Tf, typename Ti>
DynFix<Tf,Ti> DynFix<Tf,Ti>::operator+(const DynFix<Tf,Ti>& r) const {
    return DynFix<Tf,Ti>(static_cast<Ti>(Q + r.Q), m, n);
}

template <typename Tf, typename Ti>
DynFix<Tf,Ti> DynFix<Tf,Ti>::operator-(const DynFix<Tf,Ti>& r) const {
    return DynFix<Tf,Ti>(static_cast<Ti>(Q - r.Q), m, n);
}

// Multiply: integer product would be scaled by 2^(2n), so right-shift by n to
// restore Q(m.n) format. Uses int64_t intermediate to avoid overflow for
// int16_t and int32_t operands.
template <typename Tf, typename Ti>
DynFix<Tf,Ti> DynFix<Tf,Ti>::operator*(const DynFix<Tf,Ti>& r) const {
    int64_t intermediate = static_cast<int64_t>(Q) * static_cast<int64_t>(r.Q);
    return DynFix<Tf,Ti>(static_cast<Ti>(intermediate >> n), m, n);
}

// Divide: left-shift numerator by n to pre-scale before integer division,
// restoring Q(m.n) format in the result.
template <typename Tf, typename Ti>
DynFix<Tf,Ti> DynFix<Tf,Ti>::operator/(const DynFix<Tf,Ti>& r) const {
    int64_t shifted = static_cast<int64_t>(Q) << n;
    return DynFix<Tf,Ti>(static_cast<Ti>(shifted / static_cast<int64_t>(r.Q)), m, n);
}

// --- Explicit instantiations ---
template class DynFix<double, int64_t>;
template class DynFix<double, int32_t>;
template class DynFix<float,  int32_t>;
template class DynFix<float,  int16_t>;
