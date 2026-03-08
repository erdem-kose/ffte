// Copyright (c) [2022-2026] Erdem Kose

#ifndef FFTE_PNGIO_H_
#define FFTE_PNGIO_H_

#ifdef __cplusplus
extern "C" {
#endif

unsigned char** png_read_gray8(const char* filename, unsigned int* width, unsigned int* height);

int png_write_gray8(const char* filename, unsigned char** pixels, unsigned int width, unsigned int height);

void png_free_gray8(unsigned char** pixels);

#ifdef __cplusplus
}
#endif

#endif
