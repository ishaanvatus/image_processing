#include "improc.h"
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

Image *image_malloc(int width, int height, int depth, int channels)
{
        Image *image = malloc(sizeof(Image *));
        image->height = height;
        image->width = width;
        image->depth = depth;
        image->channels = channels;
        image->data = malloc(sizeof(uint8_t)*image->width*image->height*image->channels);
        return image;
}
int save_image(Image *image, char *filename)
{
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return -1;
    }
    fprintf(fp, "P6\n%d %d\n%d\n", image->width, image->height, (1<<image->depth) - 1); // depth = 8, max_val = 2^8 - 1 = 255
    if (image->depth == 8)  
        fwrite(image->data, sizeof(uint8_t), image->width*image->height*image->channels, fp);
    else if (image->depth == 16)  
        fwrite(image->data, sizeof(uint16_t), image->width*image->height*image->channels, fp);
    else if (image->depth == 32)  
        fwrite(image->data, sizeof(uint32_t), image->width*image->height*image->channels, fp);
    fclose(fp);
    return 0;
}

Image *open_image(char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
        fprintf(stderr, "Error opening file %s\n", filename);
    int width, height, max_val;
    char magick[2];
    fscanf(fp, "%s\n%d %d\n%d\n", magick, &width, &height, &max_val);
    Image *image = image_malloc(width, height, log2(max_val) + 1, 3);
    if (image->depth == 8)
        fread(image->data, sizeof(uint8_t), image->width*image->height*image->channels, fp);
    fclose(fp);
    return image;
}

