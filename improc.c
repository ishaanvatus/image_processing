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
    FILE *fp = fopen(filename, "rb");
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
    fread(img->pixels, sizeof(uint8_t),3*img->width*img->height, fp);
    return img;
}
int save(char *filename, struct Image img)
{
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        return -1;
    }
    fprintf(fp, "P6\n%d %d\n255\n", img.width, img.height);
    fwrite(img.pixels, sizeof(uint8_t), 3*img.width*img.height, fp);
    return 0;
}
