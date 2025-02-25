#include "improc.h"
#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#define M_PI 3.141592653
Image *gradient_wheel(Image *sy, Image *sx)
{
    Image *result = malloc_image(sx->width, sx->height, sx->channels, sx->bit_depth);
    for (uint32_t index = 0; index < sx->width*sx->height*sx->channels; index += sx->channels) {
        double rads = fabs(atan2(sy->pixels[index], sx->pixels[index]));
        if (rads > M_PI)
            rads = M_PI;
        if (rads < 0)
            rads = 0;
        for (uint32_t channel = 0; channel < sx->channels; channel++)
            result->pixels[index + channel] = 255*(rads/M_PI);
    }
    return result;
}
int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "program usage: ./main 'image name'\n");
        return -1;
    }
    Image *image = open_image(argv[1]), *sx, *sy, *grad;
    save_image(image, (char *) "original.ppm");
    Image *kernel = malloc_image(3, 3, 1, 8);
    Image *blur = malloc_image(5, 5, 1, 8);
    double gaussian[25] = {
        01, 04, 06, 04, 01,
        06, 24, 36, 24, 06, 
        04, 16, 24, 16, 04,
        01, 04, 06, 04, 01
    };
    for (uint32_t index = 0; index < blur->width*blur->height; index++)
        blur->pixels[index] = gaussian[index]/192;
    image = convolve(image, blur);
    save_image(image, (char *) "identity.ppm");
    double soby[9] = {
        +0.25, +0.00, -0.25,
        +0.50, +0.00, -0.50,
        +0.25, +0.00, -0.25
    };
    double sobx[9] = {
        +0.25, +0.50, +0.25,
        +0.00, +0.00, +0.00
        -0.25, -0.50, -0.25,
    };
    for (uint32_t index = 0; index < kernel->width*kernel->height; index++)
        kernel->pixels[index] = sobx[index];
    sx = convolve(image, kernel);
    save_image(normalize(sx), (char *) "sobx.ppm");
    for (uint32_t index = 0; index < kernel->width*kernel->height; index++)
        kernel->pixels[index] = soby[index];
    sy = convolve(image, kernel);
    save_image(normalize(sy), (char *) "soby.ppm");
    grad = malloc_image(image->width, image->height, image->channels, image->bit_depth);
    for (uint32_t index = 0; index < image->width*image->height*image->channels; index++)
        grad->pixels[index] = pow(pow(sx->pixels[index], 2) + pow(sy->pixels[index], 2), 0.5);
    save_image(normalize(grad), (char *) "gradient.ppm");
    Image *mag = gradient_wheel(sy, sx);
    save_image(mag, (char *) "mag.ppm");
    //same for grayscale image
    Image *gray = grayscale(image);
    save_image(gray, (char *) "identity_gray.ppm");
    for (uint32_t index = 0; index < kernel->width*kernel->height; index++)
        kernel->pixels[index] = sobx[index];
    sx = convolve(gray, kernel);
    save_image(normalize(sx), (char *) "sobx_gray.ppm");
    for (uint32_t index = 0; index < kernel->width*kernel->height; index++)
        kernel->pixels[index] = soby[index];
    sy = convolve(gray, kernel);
    save_image(normalize(sy), (char *) "soby_gray.ppm");
    grad = malloc_image(image->width, image->height, image->channels, image->bit_depth);
    for (uint32_t index = 0; index < image->width*image->height*image->channels; index++)
        grad->pixels[index] = pow(pow(sx->pixels[index], 2) + pow(sy->pixels[index], 2), 0.5);
    save_image(normalize(grad), (char *) "gradient_gray.ppm");
    mag = gradient_wheel(sy, sx);
    save_image(mag, (char *) "mag_gray.ppm");
    free_image(image);
    free_image(sx);
    free_image(sy);
    free_image(mag);
    free_image(grad);
    free_image(gray);
    free_image(kernel);
    return 0;
}
