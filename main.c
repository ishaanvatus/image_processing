#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "improc.h"

int main(int argc, char **argv) 
{
    (void) argc;
    Image *img = open_image(argv[1]);
    Image *transpose = image_malloc(img->height, img->width, img->depth, img->channels);
    Image *canvas = image_malloc(img->width, img->height, img->depth, img->channels);
    uint8_t *t_data = transpose->data;
    uint8_t *data = img->data;
    for (uint32_t row = 0; row < img->height; row++) {
        for (uint32_t col = 0; col < img->width; col++) {
            uint32_t index = (row*img->width + col)*img->channels;
            uint32_t t_index = (col*img->height + row)*img->channels;
            t_data[t_index + 0] = data[index + 0];
            t_data[t_index + 1] = data[index + 1];
            t_data[t_index + 2] = data[index + 2];
            }
        }
    for (uint32_t row = 0; row < img->height; row++) {
        for (uint32_t col = 0; col < img->width; col++) {
            uint32_t index = (row*img->width + col)*img->channels;
        }
    }
    save_image(img, "identity.ppm");
    save_image(transpose, "transpose.ppm");
    save_image(transpose, "transpose.ppm");
    return 0;
}
