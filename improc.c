#include "improc.h"
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
int save_image(Image *image, char *filename)
{
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return -1;
    }
    fprintf(fp, "P6\n%d %d\n%d\n", image->width, image->height, image->depth);
    if (image->depth == 8)  
        fwrite(image->data, sizeof(uint8_t), image->width*image->height*image->channels, fp);
    else if (image->depth == 16)  
        fwrite(image->data, sizeof(uint16_t), image->width*image->height*image->channels, fp);
    else if (image->depth == 32)  
        fwrite(image->data, sizeof(uint32_t), image->width*image->height*image->channels, fp);
    fclose(fp);
    return 0;
}

