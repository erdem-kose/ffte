// Copyright (c) [2022-2026] Erdem Kose

#ifndef FFTE_PLOT_H_
#define FFTE_PLOT_H_

#ifdef FFTE_PLOT_PNG
    #define PLOT_EXT ".png"
    #define PLOT_DEV "png"
#else
    #define PLOT_EXT ".svg"
    #define PLOT_DEV "svg"
#endif

void plot(const char* filename, double x[], double y[], int N, const char* x_lbl, const char* y_lbl);

#endif
