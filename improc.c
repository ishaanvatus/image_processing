#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "improc.h"
struct Image *create_image(size_t width, size_t height, size_t channels)
{
    struct Image *image;
    image = malloc(sizeof(struct Image));
    if (image == NULL)
        fprintf(stderr, "Couldn't allocate memory for image:%s\n", strerror(errno));
    image->width = width;
    image->height = height;
    image->channels = channels;
    image->data = malloc(channels*sizeof(double *));
    if (image->data == NULL)
        fprintf(stderr, "Couldn't allocate memory for channels:%s\n", strerror(errno));
    for (size_t i = 0; i < channels; i++) {
        image->data[i] = malloc(width*height*sizeof(double));
        if (image->data[i] == NULL)
            fprintf(stderr, "Couldn't allocate memory to channel %zu:%s\n", i, strerror(errno));
    }
    return image;
}
int save_image(const char *filename, struct Image image)
{
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file:%s\n", strerror(errno));
        return -1;
    }
    uint8_t val;
    fprintf(fp, "P6\n%zu %zu\n255\n", image.width, image.height);   
    for (size_t row = 0; row < image.height; row++) {
        for (size_t col = 0; col < image.width; col++) {
            for (size_t channel = 0; channel < image.channels; channel++) {
                val = (uint8_t) image.data[channel][row*image.width + col];
                fwrite(&val, sizeof(uint8_t), 1, fp);
            }
        }
    }
    return 0;
}
struct Image *load_image(const char *filename)
{
    char magic[3];
    int height, width;
    int maxval;
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
        fprintf(stderr, "Failed to open file:%s\n", strerror(errno));
    fscanf(fp, "%2s", magic);
    if (magic[0] != 'P' || magic[1] != '6') {
        fprintf(stderr, "Not a valid P6 ppm file:%s\n", strerror(errno));
        fclose(fp);
        return NULL;
    }
    fscanf(fp, "%d %d\n", &width, &height);
    fscanf(fp, "%d\n", &maxval);
    uint8_t val;
    struct Image *image = create_image(width, height, 3);
    for (size_t row = 0; row < image->height; row++) {
        for (size_t col = 0; col < image->width; col++) {
            for (size_t channel = 0; channel < image->channels; channel++) {
                fread(&val, sizeof(uint8_t), 1, fp);
                image->data[channel][row*image->width + col] = (double) val;
            }
        }
    }
    return image;
}
