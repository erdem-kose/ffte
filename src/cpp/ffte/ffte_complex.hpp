#pragma once

#include <cstdint>

template <typename T>
class ffteComplex {
    public:
        // Constructor
        ffteComplex(T real, T imag);

        // Overloaded Operators
        ffteComplex operator+(const ffteComplex& r) const;
        ffteComplex operator-(const ffteComplex& r) const;
        ffteComplex operator*(const ffteComplex& r) const;
        ffteComplex operator/(const ffteComplex& r) const;

        // Display function
        void display() const;

    private:
        T real;
        T imag;
};
