// Copyright (c) [2022-2026] Erdem Kose

#pragma once

#include <cstdint>

template <typename T>
class Cmplx {
    public:
        // Constructor
        Cmplx();
        Cmplx(T real, T imag);
        void set(T real, T imag);
        T get_real();
        T get_imag();

        // Overloaded Operators
        Cmplx operator+(const Cmplx& r) const;
        Cmplx operator-(const Cmplx& r) const;
        Cmplx operator*(const Cmplx& r) const;
        Cmplx operator/(const Cmplx& r) const;
    private:
        T real;
        T imag;
};

template <typename T> void ffte_cpp_bluestein(T* x_real, T* x_imag, unsigned int N, bool only_real_input, bool inverse);
template <typename T> void ffte_cpp_cooleytukey(T* x_real, T* x_imag, unsigned int N, bool only_real_input, bool inverse);