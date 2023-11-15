#ifndef IMPROC_H
#define IMPROC_H
#include <stdint.h>
typedef struct {
    int size;
    double *weights;
} Kernel;

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Pixel;
typedef struct {
    double r;
    double g;
    double b;
} Accumulator;

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
Image *load_image(char *filename);
int save_image(char *filename, Image image);

Image *grayscale(Image image);
Image *perceptual_grayscale(Image image);
double kernel_min(Kernel kernel);
double kernel_max(Kernel kernel);
Accumulator convolve(Image image, Kernel kernel, int row, int col);
Image *apply_kernel(Image image, Kernel kernel);
Image *sobel(Image image);
Kernel sobel_y(int n);
Kernel sobel_x(int n);
unsigned modulo(int value, unsigned m);

#endif
