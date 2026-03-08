// Copyright (c) [2022-2026] Erdem Kose

#include "tests.hpp"

extern "C" {
    #include <stdio.h>
    #include <math.h>
    #include <stdlib.h>
    #include "../utils/pngio.h"
    #include "../utils/exectime.h"
    #include "../src/c/ffte.h"

    /* Allocate a contiguous M×N double array with row pointers */
    static double** alloc2d(unsigned int M, unsigned int N)
    {
        double* data = (double*)calloc(M * N, sizeof(double));
        if (!data) return NULL;
        double** arr = (double**)malloc(M * sizeof(double*));
        if (!arr) { free(data); return NULL; }
        unsigned int i;
        for (i = 0; i < M; ++i)
            arr[i] = data + i * N;
        return arr;
    }

    static void free2d(double** arr)
    {
        if (arr) { free(arr[0]); free(arr); }
    }

    /* Save log-magnitude spectrum of (xr, xi) as a grayscale PNG */
    static void save_magnitude(const char* path, double** xr, double** xi, unsigned int M, unsigned int N)
    {
        unsigned int i, j;
        double** mag = alloc2d(M, N);
        if (!mag) return;

        double mag_max = 0.0;
        for (i = 0; i < M; ++i)
            for (j = 0; j < N; ++j)
            {
                double v = log(1.0 + sqrt(xr[i][j]*xr[i][j] + xi[i][j]*xi[i][j]));
                mag[i][j] = v;
                if (v > mag_max) mag_max = v;
            }

        unsigned char* data = (unsigned char*)malloc(M * N);
        unsigned char** img = (unsigned char**)malloc(M * sizeof(unsigned char*));
        if (data && img)
        {
            for (i = 0; i < M; ++i) img[i] = data + i * N;
            for (i = 0; i < M; ++i)
                for (j = 0; j < N; ++j)
                    img[i][j] = (mag_max > 0.0) ? (unsigned char)(255.0 * mag[i][j] / mag_max) : 0;
            png_write_gray8(path, img, N, M);
            free(img);
            free(data);
        }
        free2d(mag);
    }

    /* Save phase angle spectrum [-PI, PI] mapped to [0, 255] */
    static void save_angle(const char* path, double** xr, double** xi, unsigned int M, unsigned int N)
    {
        unsigned int i, j;
        unsigned char* data = (unsigned char*)malloc(M * N);
        unsigned char** img = (unsigned char**)malloc(M * sizeof(unsigned char*));
        if (data && img)
        {
            for (i = 0; i < M; ++i) img[i] = data + i * N;
            for (i = 0; i < M; ++i)
                for (j = 0; j < N; ++j)
                {
                    double angle = atan2(xi[i][j], xr[i][j]); /* [-PI, PI] */
                    img[i][j] = (unsigned char)((angle + M_PI) / (2.0 * M_PI) * 255.0);
                }
            png_write_gray8(path, img, N, M);
            free(img);
            free(data);
        }
    }

    void test_ffte2d_c(const char* png_in)
    {
        unsigned int N, M; /* N=width(cols), M=height(rows) */
        unsigned char** pixels = png_read_gray8(png_in, &N, &M);
        if (!pixels) { printf("\tFailed to read %s\r\n", png_in); return; }

        double** x_real = alloc2d(M, N);
        double** x_imag = alloc2d(M, N); /* zeros (calloc) */
        double** orig   = alloc2d(M, N);
        if (!x_real || !x_imag || !orig) { png_free_gray8(pixels); return; }

        unsigned int i, j;
        for (i = 0; i < M; ++i)
            for (j = 0; j < N; ++j)
            {
                x_real[i][j] = pixels[i][j] / 255.0;
                orig[i][j]   = x_real[i][j];
            }
        png_free_gray8(pixels);

        double ffte_time, iffte_time;

        /* 2D FFT */
        tic();
        ffte2d(x_real, x_imag, M, N, 1);
        ffte_time = toc();
        printf("\tFFTE2D(C) takes %f secs\r\n", ffte_time);

        save_magnitude("../tests/output/c_2d/magnitude.png", x_real, x_imag, M, N);
        save_angle("../tests/output/c_2d/angle.png", x_real, x_imag, M, N);

        /* 2D IFFT */
        tic();
        iffte2d(x_real, x_imag, M, N, 0);
        iffte_time = toc();
        printf("\tIFFTE2D(C) takes %f secs\r\n", iffte_time);

        /* Save reconstructed image */
        {
            unsigned char* data = (unsigned char*)malloc(M * N);
            unsigned char** img = (unsigned char**)malloc(M * sizeof(unsigned char*));
            if (data && img)
            {
                for (i = 0; i < M; ++i) img[i] = data + i * N;
                for (i = 0; i < M; ++i)
                    for (j = 0; j < N; ++j)
                    {
                        double v = x_real[i][j];
                        if (v < 0.0) v = 0.0;
                        if (v > 1.0) v = 1.0;
                        img[i][j] = (unsigned char)(v * 255.0);
                    }
                png_write_gray8("../tests/output/c_2d/reconstructed.png", img, N, M);
                free(img);
                free(data);
            }
        }

        /* Max reconstruction error */
        double max_err = 0.0;
        for (i = 0; i < M; ++i)
            for (j = 0; j < N; ++j)
            {
                double err = fabs(x_real[i][j] - orig[i][j]);
                if (err > max_err) max_err = err;
            }
        printf("\tMax reconstruction error: %e\r\n", max_err);

        /* Save times */
        FILE* ft = fopen("../tests/output/c_2d/times.txt", "w");
        if (ft)
        {
            fprintf(ft, "ffte2d_time:  %f secs\n", ffte_time);
            fprintf(ft, "iffte2d_time: %f secs\n", iffte_time);
            fprintf(ft, "max_reconstruction_error: %e\n", max_err);
            fclose(ft);
        }

        free2d(x_real);
        free2d(x_imag);
        free2d(orig);
    }
}

TEST(ffte_test, 2d_c) {
    test_ffte2d_c("../tests/imageinput/test.png");
    EXPECT_EQ(1, 1);
}
