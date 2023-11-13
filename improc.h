#ifndef IMPROC_H
#define IMPROC_H
#include <stdint.h>
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Pixel;
typedef struct {
    int height;
    int width;
    Pixel *pixels;
} Image;
typedef struct {
    double re;
    double im;
} Complex;
Image *create_image(int width, int height);
void free_image(Image *image);
Image *load(char *filename);
int save(char *filename, Image image);
Image *grayscale(Image image);
Image *perceptual_grayscale(Image image);
#endif
