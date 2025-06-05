#include "ffte_complex.hpp"

template <typename T>
ffteComplex<T> ffteComplex<T>::operator+(const ffteComplex<T>& r) const {
    return ffteComplex<T>(real + r.real, imag + r.imag);
}

template <typename T>
ffteComplex<T> ffteComplex<T>::operator-(const ffteComplex<T>& r) const {
    return ffteComplex<T>(real - r.real, imag - r.imag);
}

template <typename T>
ffteComplex<T> ffteComplex<T>::operator*(const ffteComplex<T>& r) const {
    return ffteComplex<T>(
        real * r.real - imag * r.imag,
        real * r.imag + imag * r.real
    );
}

template <typename T>
ffteComplex<T> ffteComplex<T>::operator/(const ffteComplex<T>& r) const {
    T denominator = r.real * r.real + r.imag * r.imag;
    return ffteComplex<T>(
        (real * r.real + imag * r.imag) / denominator,
        (imag * r.real - real * r.imag) / denominator
    );
}

template <typename T>
void ffteComplex<T>::display() const {
    std::cout << real << " + " << imag << "i" << std::endl;
}