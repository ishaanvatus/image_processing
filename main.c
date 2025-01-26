#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "improc.h"

int main(void) 
{

    Image img;
    char *output = (char *) "output.ppm";
    img.width = 3840;
    img.height = 2880;
    img.depth = 8;
    img.channels = 3;
    img.data = malloc(sizeof(uint8_t)*img.width*img.height*img.channels);
    uint8_t *data = (uint8_t *) img.data;
    for (uint32_t row = 0; row < img.height; row++) {
        for (uint32_t col = 0; col < img.width; col++) {
            uint32_t index = (row*img.width + col)*img.channels;
            data[index + 0] = (uint8_t) row^col;
            data[index + 1] = (uint8_t) row&col;
            data[index + 2] = (uint8_t) row||col;
        }
    }
    save_image(&img, output);
    return 0;
}
