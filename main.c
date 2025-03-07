#include "improc.h"
#include <stdio.h>
#include <math.h>
#include "kernels.h"
#define M_PI 3.141592653589793
#define DEG_CONV 57.29577951308232
Image *kernel_init(uint32_t kernel_width, uint32_t kernel_height, double *weights)
{
    Image *kernel = malloc_image(kernel_width, kernel_height, 1, 8);
    for (uint32_t index = 0; index < kernel->width*kernel->height; index++)
        kernel->pixels[index] = weights[index];
    return kernel;
}
Image *angle(Image *sy, Image *sx)
{
    Image *result = malloc_image(sx->width, sx->height, sx->channels, sx->bit_depth);
    uint32_t width = sx->width;
    uint32_t height = sx->height;
    uint32_t channels = sx->channels;
    for (uint32_t row = 0; row < height; row++) {
        for (uint32_t col = 0; col < width; col++) {
            uint32_t index = (row*width + col)*channels;
            double angle = fabs(atan2(sy->pixels[index], sx->pixels[index]));
            angle = angle*DEG_CONV;
            double mag = pow(pow(sx->pixels[index], 2) + pow(sy->pixels[index], 2), 0.5);
            if (angle >= 0 && angle <= 60) {
                result->pixels[index + 0] = mag*angle;
                result->pixels[index + 1] = 255*(angle/120);
                result->pixels[index + 2] = 255*(angle/180);
            }
            if (angle > 60 && angle <= 120) {
                result->pixels[index + 0] = 255*((angle - 60)/60);
                result->pixels[index + 1] = mag*angle;
                result->pixels[index + 2] = 255*((angle - 60)/180);
            }
            if (angle > 120 && angle <= 180) {
                result->pixels[index + 0] = 255*((angle - 120)/60);
                result->pixels[index + 1] = 255*((angle - 120)/120);
                result->pixels[index + 2] = mag*angle;
            }
        }
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

    Image *sobel_x = kernel_init(3, 3, SOBEL_X);
    Image *sobel_y = kernel_init(3, 3, SOBEL_Y);
    Image *gaussian_3 = kernel_init(3, 3, GAUSSIAN_3);

    image = convolve(image, gaussian_3);
    save_image(image, (char *) "gaussian_3.ppm");
    /*
    Image *gray = grayscale(image);
    save_image(gray, (char *) "gray.ppm");
    */

    sx = convolve(image, sobel_x);
    save_image(normalize(sx), (char *) "sobx.ppm");

    sy = convolve(image, sobel_y);
    save_image(normalize(sy), (char *) "soby.ppm");
    Image *dir = angle(sy, sx);
    save_image(normalize(dir), (char *) "angle.ppm");

    grad = malloc_image(image->width, image->height, image->channels, image->bit_depth);
    for (uint32_t index = 0; index < image->width*image->height*image->channels; index++)
        grad->pixels[index] = pow(pow(sx->pixels[index], 2) + pow(sy->pixels[index], 2), 0.5);
    save_image(normalize(grad), (char *) "gradient.ppm");

    free_image(image);
    free_image(sx);
    free_image(sy);
    free_image(grad);
    free_image(sobel_x);
    //free_image(gray);
    free_image(sobel_y);
    free_image(gaussian_3);

    return 0;
}
