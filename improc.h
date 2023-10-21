#ifndef _scrimage_h
#define _scrimage_h

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "improc.h"
struct Image
{
    uint32_t *pixels;
    int width;
    int height;
};

void fill(struct Image *image,  int color);
int save(struct Image *image, char *filename);
struct Image *load(char *filename);
#endif
