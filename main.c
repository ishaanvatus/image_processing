#include "improc.h"
#include <stdio.h>
#include <math.h>

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "program usage: ./main 'image name'\n");
        return -1;
    }
    Image *image = open_image(argv[1]), *sx, *sy, *grad;
    Image *kernel = malloc_image(3, 3, 1, 8);
    save_image(image, "identity.ppm");
    double sobx[9] = {
        +0.25, +0.00, -0.25,
        +0.50, +0.00, -0.50,
        +0.25, +0.00, -0.25
    };
    double soby[9] = {
        +0.25, +0.50, +0.25,
        +0.00, +0.00, +0.00
        -0.25, -0.50, -0.25,
    };
    for (uint32_t index = 0; index < kernel->width*kernel->height; index++)
        kernel->pixels[index] = sobx[index];
    sx = convolve(image, kernel);
    save_image(normalize(sx), "sobx.ppm");
    for (uint32_t index = 0; index < kernel->width*kernel->height; index++)
        kernel->pixels[index] = soby[index];
    sy = convolve(image, kernel);
    save_image(normalize(sy), "soby.ppm");
    grad = malloc_image(image->width, image->height, image->channels, image->bit_depth);
    for (uint32_t index = 0; index < image->width*image->height*image->channels; index++)
        grad->pixels[index] = pow(pow(sx->pixels[index], 2) + pow(sy->pixels[index], 2), 0.5);
    save_image(normalize(grad), "gradient.ppm");
    return 0;
}
