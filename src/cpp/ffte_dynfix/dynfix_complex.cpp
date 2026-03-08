// Copyright (c) [2022-2026] Erdem Kose

#include "subfuncs.hpp"

// Constructors
template <typename Tf, typename Ti>
DynFixCmplx<Tf,Ti>::DynFixCmplx() : real(), imag() {}

template <typename Tf, typename Ti>
DynFixCmplx<Tf,Ti>::DynFixCmplx(DynFix<Tf,Ti> r, DynFix<Tf,Ti> i) : real(r), imag(i) {}

// Accessors
template <typename Tf, typename Ti>
void DynFixCmplx<Tf,Ti>::set(DynFix<Tf,Ti> r, DynFix<Tf,Ti> i) {
    real = r;
    imag = i;
}

template <typename Tf, typename Ti>
DynFix<Tf,Ti> DynFixCmplx<Tf,Ti>::get_real() const { return real; }

template <typename Tf, typename Ti>
DynFix<Tf,Ti> DynFixCmplx<Tf,Ti>::get_imag() const { return imag; }

// Operators
template <typename Tf, typename Ti>
DynFixCmplx<Tf,Ti> DynFixCmplx<Tf,Ti>::operator+(const DynFixCmplx<Tf,Ti>& r) const {
    return DynFixCmplx<Tf,Ti>(real + r.real, imag + r.imag);
}

template <typename Tf, typename Ti>
DynFixCmplx<Tf,Ti> DynFixCmplx<Tf,Ti>::operator-(const DynFixCmplx<Tf,Ti>& r) const {
    return DynFixCmplx<Tf,Ti>(real - r.real, imag - r.imag);
}

template <typename Tf, typename Ti>
DynFixCmplx<Tf,Ti> DynFixCmplx<Tf,Ti>::operator*(const DynFixCmplx<Tf,Ti>& r) const {
    return DynFixCmplx<Tf,Ti>(
        real * r.real - imag * r.imag,
        real * r.imag + imag * r.real
    );
}

template <typename Tf, typename Ti>
DynFixCmplx<Tf,Ti> DynFixCmplx<Tf,Ti>::operator/(const DynFixCmplx<Tf,Ti>& r) const {
    DynFix<Tf,Ti> denom = r.real * r.real + r.imag * r.imag;
    return DynFixCmplx<Tf,Ti>(
        (real * r.real + imag * r.imag) / denom,
        (imag * r.real - real * r.imag) / denom
    );
}

template class DynFixCmplx<double, int64_t>;
template class DynFixCmplx<double, int32_t>;
template class DynFixCmplx<float,  int32_t>;
template class DynFixCmplx<float,  int16_t>;
