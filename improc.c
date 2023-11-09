#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "improc.h"
struct Image *load(char *filename)
{
    struct Image *img = malloc(sizeof(struct Image));
    if (img == NULL) {
        fprintf(stderr, "Could not allocate memory to Image: %s\n", strerror(errno));
        return NULL;
    }
    char magic[3];
    int maxval;
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        free(img);
        return NULL;
    }
    fscanf(fp, "%2s", magic);
    if (magic[0] != 'P' && magic[1] != '6') {
        fprintf(stderr, "Not a valid P6 ppm file: %s\n", strerror(errno));
        free(img);
        fclose(fp);
        return NULL;
    }
    fscanf(fp, "%d%d%*c", &img->width, &img->height);
    img->pixels = malloc(img->width*img->height*sizeof(int));
    if (img->pixels == NULL) {
        free(img);
        fprintf(stderr, "Could not allocate memory to pixels: %s\n", strerror(errno));
        return NULL;
    }
    fscanf(fp, "%d%*c", &maxval);
    fread(img->pixels, sizeof(int),img->width*img->height, fp);
    return img;
}
int save(char *filename, struct Image img)
{
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        return -1;
    }
    fprintf(fp, "P6\n%d %d\n255\n", img.width, img.height);
    for (int row = 0; row < img.height; row++) {
        for (int col = 0; col < img.width; col++) {
            uint8_t pixel[] = 
            {
                img.pixels[row*img.width + col]>>(8*0) & 0xFF, 
                img.pixels[row*img.width + col]>>(8*1) & 0xFF, 
                img.pixels[row*img.width + col]>>(8*2) & 0xFF
            };
            fwrite(pixel, sizeof(uint8_t), 3, fp);
        }
    }
    return 0;
}
