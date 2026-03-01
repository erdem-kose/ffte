// Copyright (c) [2022-2026] Erdem Kose

#include "subfuncs.hpp"

template <typename T>
Cmplx<T>::Cmplx() : real(0), imag(0) {}

template <typename T>
Cmplx<T>::Cmplx(T real, T imag) : real(real), imag(imag) {}

template <typename T>
void Cmplx<T>::set(T real, T imag) {
    this->real = real;
    this->imag = imag;
}

template <typename T>
T Cmplx<T>::get_real() {
    return real;
}

template <typename T>
T Cmplx<T>::get_imag() {
    return imag;
}

template <typename T>
Cmplx<T> Cmplx<T>::operator+(const Cmplx<T>& r) const {
    return Cmplx<T>(real + r.real, imag + r.imag);
}

template <typename T>
Cmplx<T> Cmplx<T>::operator-(const Cmplx<T>& r) const {
    return Cmplx<T>(real - r.real, imag - r.imag);
}

template <typename T>
Cmplx<T> Cmplx<T>::operator*(const Cmplx<T>& r) const {
    return Cmplx<T>(
        real * r.real - imag * r.imag,
        real * r.imag + imag * r.real
    );
}

template <typename T>
Cmplx<T> Cmplx<T>::operator/(const Cmplx<T>& r) const {
    T denominator = r.real * r.real + r.imag * r.imag;
    return Cmplx<T>(
        (real * r.real + imag * r.imag) / denominator,
        (imag * r.real - real * r.imag) / denominator
    );
}

template class Cmplx<double>; 
template class Cmplx<float>; 