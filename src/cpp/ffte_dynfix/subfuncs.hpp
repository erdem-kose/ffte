// Copyright (c) [2022-2026] Erdem Kose

#pragma once

#include <cstdint>

// Dynamic fixed-point type: Tf = float reference type, Ti = integer storage type.
// Format: Q(m.n) where m = integer bits, n = fractional bits.
// Stored value represents: Q * 2^(-n)
//
// The radix point shifts automatically after every arithmetic operation:
//   - result grew  → n decreases (point moves right, more integer range)
//   - result shrank → n increases (point moves left, more fractional precision)
// Each operand may carry a different n; operators align before computing.
// Constructors and set() always use the caller-specified Q(m.n) format.
template <typename Tf, typename Ti>
class DynFix {
    public:
        DynFix();
        DynFix(Ti raw, uint8_t m, uint8_t n);  // from raw integer
        DynFix(Tf val, uint8_t m, uint8_t n);  // from float, converts to fixed-point

        void set(Tf val);       // update value keeping current format
        Tf   toFloat() const;   // convert back to float
        Ti   raw()     const;   // get raw integer representation
        uint8_t getM() const;
        uint8_t getN() const;

        DynFix operator+(const DynFix& r) const;  // aligns to max(n, r.n), renormalizes
        DynFix operator-(const DynFix& r) const;  // aligns to max(n, r.n), renormalizes
        DynFix operator*(const DynFix& r) const;  // exact product at n+r.n, renormalizes
        DynFix operator/(const DynFix& r) const;  // max-precision boost, renormalizes
    private:
        Ti      Q;  // raw fixed-point integer
        uint8_t m;  // integer bits (metadata)
        uint8_t n;  // fractional bits (determines scale: 2^n)
};

// Fixed-point complex number built on DynFix<Tf,Ti>.
// All components must share the same Q(m.n) format (same n).
template <typename Tf, typename Ti>
class DynFixCmplx {
    public:
        DynFixCmplx();
        DynFixCmplx(DynFix<Tf,Ti> r, DynFix<Tf,Ti> i);
        void          set(DynFix<Tf,Ti> r, DynFix<Tf,Ti> i);
        DynFix<Tf,Ti> get_real() const;
        DynFix<Tf,Ti> get_imag() const;

        DynFixCmplx operator+(const DynFixCmplx& r) const;
        DynFixCmplx operator-(const DynFixCmplx& r) const;
        DynFixCmplx operator*(const DynFixCmplx& r) const;  // (ac-bd) + j(ad+bc), rescales via >>n
        DynFixCmplx operator/(const DynFixCmplx& r) const;  // conjugate / |r|^2
    private:
        DynFix<Tf,Ti> real;
        DynFix<Tf,Ti> imag;
};

// Fixed-point FFT kernels (arrays of DynFix; all elements must share the same n)
template <typename Tf, typename Ti>
void ffte_cpp_cooleytukey_dynfix(DynFix<Tf,Ti>* x_real, DynFix<Tf,Ti>* x_imag,
                              unsigned int N, bool only_real_input, bool inverse);
template <typename Tf, typename Ti>
void ffte_cpp_bluestein_dynfix(DynFix<Tf,Ti>* x_real, DynFix<Tf,Ti>* x_imag,
                            unsigned int N, bool only_real_input, bool inverse);