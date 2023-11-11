#ifndef IMAGE_H
#define IMAGE_H
#include <stdint.h>
struct Image
{
    int height;
    int width;
    uint8_t *pixels;
};
struct Image *load(char *filename);
int save(char *filename, struct Image img);
void grayscale(struct Image *img);
void perceptual_grayscale(struct Image *img);
#endif
