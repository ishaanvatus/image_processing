#ifndef IMPROC_H
#define IMPROC_H
#include <stdint.h>
typedef struct {
    uint32_t height;
    uint32_t width;
    double *pixels;
    uint32_t channels;
    uint32_t bit_depth;
} Image;
Image *malloc_image(uint32_t width, uint32_t height, uint32_t channels, uint32_t bit_depth);
void free_image(Image *image);
Image *open_image(char *filename);
int save_image(Image *image, char *filename);
#endif 
