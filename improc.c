#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "improc.h"
Image *create_image(int width, int height)
{
    Image *image = malloc(sizeof(Image));
    if (image == NULL) {
        fprintf(stderr, "Could not allocate memory to Image: %s\n", strerror(errno));
        return NULL;
    }
    image->width = width;
    image->height = height;
    image->pixels = malloc(width*height*sizeof(Pixel));
    if (image->pixels == NULL) {
        free(image);
        fprintf(stderr, "Could not allocate memory to pixels: %s\n", strerror(errno));
        return NULL;
    }
    return image;
}
void free_image(Image *image)
{
    if (image != NULL) {
        if (image->pixels != NULL)
            free(image->pixels);
        free(image);
    }
}
Image *load_image(char *filename)
{
    char magic[3];
    int maxval, width, height;
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        return NULL;
    }
    fscanf(fp, "%2s", magic);
    if (magic[0] != 'P' && magic[1] != '6') {
        fprintf(stderr, "Not a valid P6 ppm file: %s\n", strerror(errno));
        fclose(fp);
        return NULL;
    }
    fscanf(fp, "%d%d%*c", &width, &height);
    Image *image = create_image(width, height);
    fscanf(fp, "%d%*c", &maxval);
    fread(image->pixels, sizeof(Pixel),image->width*image->height, fp);
    fclose(fp);
    return image;
}
int save_image(char *filename, Image image)
{
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        return -1;
    }
    fprintf(fp, "P6\n%d %d\n255\n", image.width, image.height);
    fwrite(image.pixels, sizeof(Pixel), image.width*image.height, fp);
    fclose(fp);
    return 0;
}
Image *grayscale(Image image)
{
    Image *gray = create_image(image.width, image.height);
    Pixel pix;
    int index;
	uint8_t avg;
	for (int row = 0; row < image.height; row++) {
		for (int col = 0; col < image.width; col++) {
            index = row*image.width + col;
			pix = image.pixels[index];
			avg = (uint8_t) ((pix.r + pix.g + pix.b)/3.0);
			gray->pixels[index] = (Pixel) {avg, avg, avg};
		}
	}
    return gray;
}
Image *perceptual_grayscale(Image image)
{
    Image *gray = create_image(image.width, image.height);
    Pixel pix;
	uint8_t bt_601;
    int index;
	for (int row = 0; row < image.height; row++) {
		for (int col = 0; col < image.width; col++) {
            index = row*image.width + col;
            pix = image.pixels[index];
            bt_601 = (uint8_t) (0.2126*pix.r + 0.7152*pix.g + 0.0722*pix.b);
			gray->pixels[index] = (Pixel) {bt_601, bt_601, bt_601};
		}
	}
    return gray;
}
