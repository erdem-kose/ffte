// Copyright (c) [2022-2026] Erdem Kose

#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "pngio.h"

unsigned char** png_read_gray8(const char* filename, unsigned int* width, unsigned int* height)
{
    FILE* fp = fopen(filename, "rb");
    if (!fp) return NULL;

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) { fclose(fp); return NULL; }

    png_infop info = png_create_info_struct(png);
    if (!info) { png_destroy_read_struct(&png, NULL, NULL); fclose(fp); return NULL; }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &info, NULL);
        fclose(fp);
        return NULL;
    }

    png_init_io(png, fp);
    png_read_info(png, info);

    *width  = png_get_image_width(png, info);
    *height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth  = png_get_bit_depth(png, info);

    /* Normalize to 8-bit grayscale */
    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);
    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);
    if (bit_depth == 16)
        png_set_strip_16(png);
    if (color_type & PNG_COLOR_MASK_ALPHA)
        png_set_strip_alpha(png);
    if (color_type == PNG_COLOR_TYPE_RGB       ||
        color_type == PNG_COLOR_TYPE_RGB_ALPHA ||
        color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_rgb_to_gray_fixed(png, 1, -1, -1); /* default luminance weights */

    png_read_update_info(png, info);

    unsigned int w = *width;
    unsigned int h = *height;

    /* Allocate contiguous data block and row pointer array */
    unsigned char* data = (unsigned char*)malloc(w * h);
    if (!data) { png_destroy_read_struct(&png, &info, NULL); fclose(fp); return NULL; }

    unsigned char** pixels = (unsigned char**)malloc(h * sizeof(unsigned char*));
    if (!pixels) { free(data); png_destroy_read_struct(&png, &info, NULL); fclose(fp); return NULL; }

    unsigned int y;
    for (y = 0; y < h; ++y)
        pixels[y] = data + y * w;

    png_read_image(png, (png_bytep*)pixels);

    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);
    return pixels;
}

int png_write_gray8(const char* filename, unsigned char** pixels, unsigned int width, unsigned int height)
{
    FILE* fp = fopen(filename, "wb");
    if (!fp) return -1;

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) { fclose(fp); return -1; }

    png_infop info = png_create_info_struct(png);
    if (!info) { png_destroy_write_struct(&png, NULL); fclose(fp); return -1; }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        return -1;
    }

    png_init_io(png, fp);
    png_set_IHDR(png, info,
                 width, height,
                 8,
                 PNG_COLOR_TYPE_GRAY,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png, info);
    png_write_image(png, (png_bytep*)pixels);
    png_write_end(png, NULL);

    png_destroy_write_struct(&png, &info);
    fclose(fp);
    return 0;
}

void png_free_gray8(unsigned char** pixels)
{
    if (pixels) {
        free(pixels[0]); /* free contiguous data block */
        free(pixels);    /* free row pointer array */
    }
}
