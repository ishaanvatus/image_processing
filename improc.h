#ifndef IMPROC_H_
#define IMPROC_H_
#include <stdint.h>

typedef struct Image {
    uint32_t height;
    uint32_t width;
    uint32_t depth;
    uint32_t channels;
    void *data;
} Image;

Image *image_malloc(int width, int height, int depth, int channels);
int save_image(Image *image, char *filename);
Image *open_image(char *filename);
#endif
