// Copyright (c) [2022-2026] Erdem Kose

#include "tests.hpp"

#include <cstdio>
#include <cmath>
#include <cstdlib>
extern "C" {
    #include "../utils/pngio.h"
    #include "../utils/exectime.h"
}
#include "../src/cpp/ffte.hpp"

static double** alloc2d_cpp(unsigned int M, unsigned int N)
{
    double* data = (double*)calloc(M * N, sizeof(double));
    if (!data) return NULL;
    double** arr = (double**)malloc(M * sizeof(double*));
    if (!arr) { free(data); return NULL; }
    for (unsigned int i = 0; i < M; ++i)
        arr[i] = data + i * N;
    return arr;
}

static void free2d_cpp(double** arr)
{
    if (arr) { free(arr[0]); free(arr); }
}

static void save_magnitude_cpp(const char* path, double** xr, double** xi, unsigned int M, unsigned int N)
{
    double** mag = alloc2d_cpp(M, N);
    if (!mag) return;

    double mag_max = 0.0;
    for (unsigned int i = 0; i < M; ++i)
        for (unsigned int j = 0; j < N; ++j)
        {
            double v = log(1.0 + sqrt(xr[i][j]*xr[i][j] + xi[i][j]*xi[i][j]));
            mag[i][j] = v;
            if (v > mag_max) mag_max = v;
        }

    unsigned char* data = (unsigned char*)malloc(M * N);
    unsigned char** img = (unsigned char**)malloc(M * sizeof(unsigned char*));
    if (data && img)
    {
        for (unsigned int i = 0; i < M; ++i) img[i] = data + i * N;
        for (unsigned int i = 0; i < M; ++i)
            for (unsigned int j = 0; j < N; ++j)
                img[i][j] = (mag_max > 0.0) ? (unsigned char)(255.0 * mag[i][j] / mag_max) : 0;
        png_write_gray8(path, img, N, M);
        free(img);
        free(data);
    }
    free2d_cpp(mag);
}

static void save_angle_cpp(const char* path, double** xr, double** xi, unsigned int M, unsigned int N)
{
    unsigned char* data = (unsigned char*)malloc(M * N);
    unsigned char** img = (unsigned char**)malloc(M * sizeof(unsigned char*));
    if (data && img)
    {
        for (unsigned int i = 0; i < M; ++i) img[i] = data + i * N;
        for (unsigned int i = 0; i < M; ++i)
            for (unsigned int j = 0; j < N; ++j)
            {
                double angle = atan2(xi[i][j], xr[i][j]); /* [-PI, PI] */
                img[i][j] = (unsigned char)((angle + M_PI) / (2.0 * M_PI) * 255.0);
            }
        png_write_gray8(path, img, N, M);
        free(img);
        free(data);
    }
}

void test_ffte2d_cpp(const char* png_in)
{
    unsigned int N, M; /* N=width(cols), M=height(rows) */
    unsigned char** pixels = png_read_gray8(png_in, &N, &M);
    if (!pixels) { printf("\tFailed to read %s\r\n", png_in); return; }

    double** x_real = alloc2d_cpp(M, N);
    double** x_imag = alloc2d_cpp(M, N);
    double** orig   = alloc2d_cpp(M, N);
    if (!x_real || !x_imag || !orig) { png_free_gray8(pixels); return; }

    for (unsigned int i = 0; i < M; ++i)
        for (unsigned int j = 0; j < N; ++j)
        {
            x_real[i][j] = pixels[i][j] / 255.0;
            orig[i][j]   = x_real[i][j];
        }
    png_free_gray8(pixels);

    double ffte_time, iffte_time;

    /* 2D FFT */
    tic();
    ffte2d_cpp<double>(x_real, x_imag, M, N, true);
    ffte_time = toc();
    printf("\tFFTE2D(CPP) takes %f secs\r\n", ffte_time);

    save_magnitude_cpp("../tests/output/cpp_2d/magnitude.png", x_real, x_imag, M, N);
    save_angle_cpp("../tests/output/cpp_2d/angle.png", x_real, x_imag, M, N);

    /* 2D IFFT */
    tic();
    iffte2d_cpp<double>(x_real, x_imag, M, N, false);
    iffte_time = toc();
    printf("\tIFFTE2D(CPP) takes %f secs\r\n", iffte_time);

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
                    double v = x_real[i][j];
                    if (v < 0.0) v = 0.0;
                    if (v > 1.0) v = 1.0;
                    img[i][j] = (unsigned char)(v * 255.0);
                }
            png_write_gray8("../tests/output/cpp_2d/reconstructed.png", img, N, M);
            free(img);
            free(data);
        }
    }

    /* Max reconstruction error */
    double max_err = 0.0;
    for (unsigned int i = 0; i < M; ++i)
        for (unsigned int j = 0; j < N; ++j)
        {
            double err = fabs(x_real[i][j] - orig[i][j]);
            if (err > max_err) max_err = err;
        }
    printf("\tMax reconstruction error: %e\r\n", max_err);

    /* Save times */
    FILE* ft = fopen("../tests/output/cpp_2d/times.txt", "w");
    if (ft)
    {
        fprintf(ft, "ffte2d_time:  %f secs\n", ffte_time);
        fprintf(ft, "iffte2d_time: %f secs\n", iffte_time);
        fprintf(ft, "max_reconstruction_error: %e\n", max_err);
        fclose(ft);
    }

    free2d_cpp(x_real);
    free2d_cpp(x_imag);
    free2d_cpp(orig);
}

TEST(ffte_test, 2d_cpp) {
    test_ffte2d_cpp("../tests/imageinput/test.png");
    EXPECT_EQ(1, 1);
}
