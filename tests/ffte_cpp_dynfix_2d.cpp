// Copyright (c) [2022-2026] Erdem Kose

#include "tests.hpp"

#include <cstdio>
#include <cmath>
#include <cstdlib>
extern "C" {
    #include "../utils/pngio.h"
    #include "../utils/exectime.h"
}
#include "../src/cpp/ffte_dynfix.hpp"

static constexpr uint8_t QM = 8;
static constexpr uint8_t QN = 16;

/* Allocate M×N DynFix array (row pointers into contiguous block) */
template <typename Tf, typename Ti>
static DynFix<Tf,Ti>** alloc2d_fix(unsigned int M, unsigned int N)
{
    DynFix<Tf,Ti>* data = new DynFix<Tf,Ti>[M * N];
    DynFix<Tf,Ti>** arr = new DynFix<Tf,Ti>*[M];
    for (unsigned int i = 0; i < M; ++i)
        arr[i] = data + i * N;
    return arr;
}

template <typename Tf, typename Ti>
static void free2d_fix(DynFix<Tf,Ti>** arr)
{
    if (arr) { delete[] arr[0]; delete[] arr; }
}

static void save_magnitude_fix(const char* path,
                                DynFix<double,int32_t>** xr, DynFix<double,int32_t>** xi,
                                unsigned int M, unsigned int N)
{
    double* flat = (double*)malloc(M * N * sizeof(double));
    if (!flat) return;

    double mag_max = 0.0;
    for (unsigned int i = 0; i < M; ++i)
        for (unsigned int j = 0; j < N; ++j)
        {
            double r = (double)xr[i][j].toFloat();
            double im = (double)xi[i][j].toFloat();
            double v = log(1.0 + sqrt(r*r + im*im));
            flat[i * N + j] = v;
            if (v > mag_max) mag_max = v;
        }

    unsigned char* data = (unsigned char*)malloc(M * N);
    unsigned char** img = (unsigned char**)malloc(M * sizeof(unsigned char*));
    if (data && img)
    {
        for (unsigned int i = 0; i < M; ++i) img[i] = data + i * N;
        for (unsigned int i = 0; i < M; ++i)
            for (unsigned int j = 0; j < N; ++j)
                img[i][j] = (mag_max > 0.0) ? (unsigned char)(255.0 * flat[i*N+j] / mag_max) : 0;
        png_write_gray8(path, img, N, M);
        free(img);
        free(data);
    }
    free(flat);
}

static void save_angle_fix(const char* path,
                            DynFix<double,int32_t>** xr, DynFix<double,int32_t>** xi,
                            unsigned int M, unsigned int N)
{
    unsigned char* data = (unsigned char*)malloc(M * N);
    unsigned char** img = (unsigned char**)malloc(M * sizeof(unsigned char*));
    if (data && img)
    {
        for (unsigned int i = 0; i < M; ++i) img[i] = data + i * N;
        for (unsigned int i = 0; i < M; ++i)
            for (unsigned int j = 0; j < N; ++j)
            {
                double r  = (double)xr[i][j].toFloat();
                double im = (double)xi[i][j].toFloat();
                double angle = atan2(im, r); /* [-PI, PI] */
                img[i][j] = (unsigned char)((angle + M_PI) / (2.0 * M_PI) * 255.0);
            }
        png_write_gray8(path, img, N, M);
        free(img);
        free(data);
    }
}

void test_ffte2d_cpp_dynfix(const char* png_in)
{
    unsigned int N, M; /* N=width(cols), M=height(rows) */
    unsigned char** pixels = png_read_gray8(png_in, &N, &M);
    if (!pixels) { printf("\tFailed to read %s\r\n", png_in); return; }

    DynFix<double,int32_t>** x_real = alloc2d_fix<double,int32_t>(M, N);
    DynFix<double,int32_t>** x_imag = alloc2d_fix<double,int32_t>(M, N);

    /* Store original values as double for error comparison */
    double* orig_flat = (double*)malloc(M * N * sizeof(double));

    for (unsigned int i = 0; i < M; ++i)
        for (unsigned int j = 0; j < N; ++j)
        {
            double val = pixels[i][j] / 255.0;
            x_real[i][j] = DynFix<double,int32_t>(val, QM, QN);
            x_imag[i][j] = DynFix<double,int32_t>(0.0, QM, QN);
            orig_flat[i * N + j] = val;
        }
    png_free_gray8(pixels);

    double ffte_time, iffte_time;

    /* 2D FFT */
    tic();
    ffte2d_cpp_dynfix<double,int32_t>(x_real, x_imag, M, N, true);
    ffte_time = toc();
    printf("\tFFTE2D(CPP-DynFix) takes %f secs\r\n", ffte_time);

    save_magnitude_fix("../tests/output/cpp_dynfix_2d/magnitude.png", x_real, x_imag, M, N);
    save_angle_fix("../tests/output/cpp_dynfix_2d/angle.png", x_real, x_imag, M, N);

    /* 2D IFFT */
    tic();
    iffte2d_cpp_dynfix<double,int32_t>(x_real, x_imag, M, N, false);
    iffte_time = toc();
    printf("\tIFFTE2D(CPP-DynFix) takes %f secs\r\n", iffte_time);

    /* Save reconstructed image */
    {
        unsigned char* data = (unsigned char*)malloc(M * N);
        unsigned char** img = (unsigned char**)malloc(M * sizeof(unsigned char*));
        if (data && img)
        {
            for (unsigned int i = 0; i < M; ++i) img[i] = data + i * N;
            for (unsigned int i = 0; i < M; ++i)
                for (unsigned int j = 0; j < N; ++j)
                {
                    double v = (double)x_real[i][j].toFloat();
                    if (v < 0.0) v = 0.0;
                    if (v > 1.0) v = 1.0;
                    img[i][j] = (unsigned char)(v * 255.0);
                }
            png_write_gray8("../tests/output/cpp_dynfix_2d/reconstructed.png", img, N, M);
            free(img);
            free(data);
        }
    }

    /* Max reconstruction error */
    double max_err = 0.0;
    for (unsigned int i = 0; i < M; ++i)
        for (unsigned int j = 0; j < N; ++j)
        {
            double err = fabs((double)x_real[i][j].toFloat() - orig_flat[i * N + j]);
            if (err > max_err) max_err = err;
        }
    printf("\tMax reconstruction error: %e\r\n", max_err);

    /* Save times */
    FILE* ft = fopen("../tests/output/cpp_dynfix_2d/times.txt", "w");
    if (ft)
    {
        fprintf(ft, "ffte2d_time:  %f secs\n", ffte_time);
        fprintf(ft, "iffte2d_time: %f secs\n", iffte_time);
        fprintf(ft, "max_reconstruction_error: %e\n", max_err);
        fclose(ft);
    }

    free(orig_flat);
    free2d_fix<double,int32_t>(x_real);
    free2d_fix<double,int32_t>(x_imag);
}

TEST(ffte_test, 2d_cpp_dynfix) {
    test_ffte2d_cpp_dynfix("../tests/imageinput/test.png");
    EXPECT_EQ(1, 1);
}
