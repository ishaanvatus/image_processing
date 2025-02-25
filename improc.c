#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>
#include "improc.h" //consistency check
Image *malloc_image(uint32_t width, uint32_t height, uint32_t channels, uint32_t bit_depth)
{
    Image *image = malloc(sizeof(Image));
    image->height = height;
    image->width = width;
    image->channels = channels;
    image->bit_depth = bit_depth;
    image->pixels = malloc(sizeof(double)*height*width*channels);
    return image;

}
void free_image(Image *image)
{
    if (image) {
        if (image->pixels)
            free(image->pixels);
        free(image);
        image = NULL;
    }
}
Image *open_image(char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "error opening image\n");
        return NULL;
    }
    char magic[3];
    uint32_t width, height, maxval, channels = 3;
    fscanf(fp, "%s\n%" SCNu32 " %" SCNu32 "\n%" SCNu32 "\n", magic, &width, &height, &maxval);
    printf("%s\n%" SCNu32 " %" SCNu32 "\n%" SCNu32 "\n", magic, width, height, maxval);
    uint32_t power = 1, depth = 0;
    while (power < maxval)  {
        depth++;
        power = power*2;
    }
    printf("%d\n", depth);
    Image *image = malloc_image(width, height, channels, depth);
    for (uint32_t index = 0; index < image->width*image->height*image->channels; index++) {
        uint8_t val;
        if (fread(&val, sizeof(val), 1, fp)  == 0)
            return NULL;
        image->pixels[index] = val;
    }
    fclose(fp);
    return image;
}
int save_image(Image *image, char *filename)
{
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "error opening image\n");
        return -1;
    }
    uint32_t maxval = 1;
    for (uint32_t i = 0; i < image->bit_depth; i++)
        maxval = maxval*2;
    maxval--;
    fprintf(fp, "P6\n%" SCNu32 " %" SCNu32 "\n%" SCNu32 "\n", image->width, image->height, maxval);
    for (uint32_t index = 0; index < image->width*image->height*image->channels; index++) {
        uint8_t val = (uint8_t) image->pixels[index];
        if (fwrite(&val, sizeof(val), 1, fp) == 0)
            return -1;
    }
    fclose(fp);
    return 0;
}   
Image *convolve(Image *image, Image *kernel)
{
    Image *result = malloc_image(image->width, image->height, image->channels, image->bit_depth);
    int kw = (int) kernel->width;
    int kh = (int) kernel->height;
    for (uint32_t row = 0; row < image->height; row++) {
        for (uint32_t col = 0; col < image->width; col++) {
            uint32_t index = (row*image->width + col)*image->channels;
            double accumulator[image->channels];
            for (uint32_t channel = 0; channel < image->channels; channel++)
                accumulator[channel] = 0;
            for (int row_off = -kh/2; row_off <= kh/2; row_off++) {
                for (int col_off = -kw/2; col_off <= kw/2; col_off++) {
                    uint32_t col_rel = ((col + col_off)%image->width);
                    uint32_t row_rel = ((row + row_off)%image->height);
                    uint32_t offset_index = (row_rel*image->width + col_rel)*image->channels;
                    uint32_t kernel_index = (row_off + kh/2)*kw + (col_off + kw/2);
                    for (uint32_t channel = 0; channel < image->channels; channel++)
                        accumulator[channel] += kernel->pixels[kernel_index]*image->pixels[offset_index + channel];
                }
            }
            for (uint32_t channel = 0; channel < image->channels; channel++)
                //result->pixels[index + channel] = (accumulator[channel] - min)/div;
                result->pixels[index + channel] = accumulator[channel];
        }
    }
    return result;
}
Image *normalize(Image *image)
{
    printf("%d %d %d %d\n", image->width, image->height, image->channels, image->bit_depth);
    Image *result = malloc_image(image->width, image->height, image->channels, image->bit_depth);
    double max[image->channels];
    double min[image->channels];
    for (uint32_t channel = 0; channel < image->channels; channel++) {
        max[channel] = image->pixels[channel];
        min[channel] = image->pixels[channel];
    }
    for (uint32_t row = 0; row < image->height; row++) {
        for (uint32_t col = 0; col < image->width; col++) {
            uint32_t index = (row*image->width + col)*image->channels;
            for (uint32_t channel = 0; channel < image->channels; channel++) {
                if (image->pixels[index + channel] < min[channel])
                    min[channel] = image->pixels[index + channel];
                if (image->pixels[index + channel] > max[channel])
                    max[channel] = image->pixels[index + channel];
            }
        }
    }
    double div[image->channels];
    for (uint32_t channel = 0; channel < image->channels; channel++)
        div[channel] = (max[channel] - min[channel])/255;
    for (uint32_t row = 0; row < image->height; row++) {
        for (uint32_t col = 0; col < image->width; col++) {
            uint32_t index = (row*image->width + col)*image->channels;
            for (uint32_t channel = 0; channel < image->channels; channel++)
                result->pixels[index + channel] = (image->pixels[index + channel] - min[channel])/div[channel];
        }
    }
    return result;
}
Image *grayscale(Image *image)
{
    Image *result = malloc_image(image->width, image->height, image->channels, image->bit_depth);
    for (uint32_t index = 0; index < image->width*image->height*image->channels; index += image->channels) {
        double avg = 0;
        for (uint32_t channel = 0; channel < image->channels; channel++)
            avg += image->pixels[index + channel];
        avg = avg/(1.0*image->channels);
        for (uint32_t channel = 0; channel < image->channels; channel++)
            result->pixels[index + channel] = avg;
    }
    return result;
}
