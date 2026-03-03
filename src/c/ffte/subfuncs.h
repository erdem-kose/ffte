// Copyright (c) [2022-2026] Erdem Kose

#pragma once

// Complex multiply and divide in split real/imag form
void cmplx_mul(double *z_r, double *z_i, double x_r, double x_i, double y_r, double y_i);
void cmplx_div(double *z_r, double *z_i, double x_r, double x_i, double y_r, double y_i);

// FFT kernels (called by ffte / iffte based on N)
void ffte_bluestein(double* x_real, double* x_imag, unsigned int N, unsigned char only_real_input, unsigned char inverse);
void ffte_cooleytukey(double* x_real, double* x_imag, unsigned int N, unsigned char only_real_input, unsigned char inverse);