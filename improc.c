#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "improc.h"
void free_image(Image *image)
{
    if (image != NULL) {
        if (image->pixels != NULL)
            free(image->pixels);
        free(image);
    }
}
Image *load(char *filename)
{
    Image *image = malloc(sizeof(Image));
    if (image == NULL) {
        fprintf(stderr, "Could not allocate memory to Image: %s\n", strerror(errno));
        return NULL;
    }
    char magic[3];
    int maxval;
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        free(image);
        return NULL;
    }
    fscanf(fp, "%2s", magic);
    if (magic[0] != 'P' && magic[1] != '6') {
        fprintf(stderr, "Not a valid P6 ppm file: %s\n", strerror(errno));
        free(image);
        fclose(fp);
        return NULL;
    }
    fscanf(fp, "%d%d%*c", &image->width, &image->height);
    image->pixels = malloc(3*image->width*image->height*sizeof(uint8_t));
    if (image->pixels == NULL) {
        free(image);
        fprintf(stderr, "Could not allocate memory to pixels: %s\n", strerror(errno));
        return NULL;
    }
    fscanf(fp, "%d%*c", &maxval);
    fread(image->pixels, sizeof(uint8_t),3*image->width*image->height, fp);
    return image;
}
int save(char *filename, Image image)
{
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        return -1;
    }
    fprintf(fp, "P6\n%d %d\n255\n", image.width, image.height);
    fwrite(image.pixels, sizeof(uint8_t), 3*image.width*image.height, fp);
    return 0;
}
Image *grayscale(Image image)
{
    Image *gray = malloc(sizeof(Image));
    if (gray == NULL) {
        fprintf(stderr, "Could not allocate memory to Image: %s\n", strerror(errno));
        return NULL;
    }
    gray->pixels = malloc(3*image.width*image.height*sizeof(uint8_t));
    if (gray->pixels == NULL) {
        free(gray);
        fprintf(stderr, "Could not allocate memory to pixels: %s\n", strerror(errno));
        return NULL;
    }
    gray->width = image.width;
    gray->height = image.height;
	uint8_t avg;
   	uint8_t r, g, b;
	for (int row = 0; row < image.height*3; row += 3) {
		for (int col = 0; col < image.width*3; col += 3) {
			r = image.pixels[row*image.width + col + 0];
			g = image.pixels[row*image.width + col + 1];
			b = image.pixels[row*image.width + col + 2];
			avg = ((int) (r+g+b))/3;
			gray->pixels[row*image.width + col + 0] = avg;
			gray->pixels[row*image.width + col + 1] = avg;
			gray->pixels[row*image.width + col + 2] = avg;
		}
	}
    return gray;
}
Image *perceptual_grayscale(Image image)
{
    Image *gray = malloc(sizeof(Image));
    if (gray == NULL) {
        fprintf(stderr, "Could not allocate memory to Image: %s\n", strerror(errno));
        return NULL;
    }
    gray->pixels = malloc(3*image.width*image.height*sizeof(uint8_t));
    if (gray->pixels == NULL) {
        free(gray);
        fprintf(stderr, "Could not allocate memory to pixels: %s\n", strerror(errno));
        return NULL;
    }
    gray->width = image.width;
    gray->height = image.height;
	uint8_t yuv_gray;
   	uint8_t r, g, b;
	for (int row = 0; row < image.height*3; row += 3) {
		for (int col = 0; col < image.width*3; col += 3) {
			r = image.pixels[row*image.width + col + 0];
			g = image.pixels[row*image.width + col + 1];
			b = image.pixels[row*image.width + col + 2];
            yuv_gray = (uint8_t)(0.299 * r + 0.587 * g + 0.114 * b);
			gray->pixels[row*image.width + col + 0] = yuv_gray;
			gray->pixels[row*image.width + col + 1] = yuv_gray;
			gray->pixels[row*image.width + col + 2] = yuv_gray;
		}
	}
    return gray;
}
Image *gradient(Image image)
{
    Image *gray = malloc(sizeof(Image));
    if (gray == NULL) {
        fprintf(stderr, "Could not allocate memory to Image: %s\n", strerror(errno));
        return NULL;
    }
    gray->pixels = malloc(3*image.width*image.height*sizeof(uint8_t));
    if (gray->pixels == NULL) {
        free(gray);
        fprintf(stderr, "Could not allocate memory to pixels: %s\n", strerror(errno));
        return NULL;
    }
    gray->width = image.width;
    gray->height = image.height;
    int count;
    uint8_t r, g, b;
    double h = image.height;
    double w = image.width;
	for (int row = 0; row < image.height*3; row += 3) {
		for (int col = 0; col < image.width*3; col += 3) {
			r = image.pixels[row*image.width + col + 0];
			g = image.pixels[row*image.width + col + 1];
			b = image.pixels[row*image.width + col + 2];
            count = row*image.width + col;
			gray->pixels[count + 0] =  (r*1.0)*(row/(h*3.0));
			gray->pixels[count + 1] =  (g*1.0)*(col/(w*3.0));
			gray->pixels[count + 2] =  (b*1.0)*(row + col)/((h + w)*9.0);
		}
	}
    return gray;
}
