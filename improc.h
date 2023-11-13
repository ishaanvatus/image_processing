#ifndef IMPROC_H
#define IMPROC_H
#include <stdint.h>
typedef struct
{
    int height;
    int width;
    uint8_t *pixels;
} Image;
typedef struct 
{
    double re;
    double im;
} Complex;
void free_image(Image *image);
Image *load(char *filename);
int save(char *filename, Image image);
Image *grayscale(Image image);
Image *perceptual_grayscale(Image image);
Image *gradient(Image image);
#endif
