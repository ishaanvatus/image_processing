#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "improc.h"
struct Image *load(char *filename)
{
    struct Image *img = malloc(sizeof(struct Image));
    if (img == NULL) {
        fprintf(stderr, "Could not allocate memory to Image: %s\n", strerror(errno));
        return NULL;
    }
    char magic[3];
    int maxval;
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        free(img);
        return NULL;
    }
    fscanf(fp, "%2s", magic);
    if (magic[0] != 'P' && magic[1] != '6') {
        fprintf(stderr, "Not a valid P6 ppm file: %s\n", strerror(errno));
        free(img);
        fclose(fp);
        return NULL;
    }
    fscanf(fp, "%d%d%*c", &img->width, &img->height);
    img->pixels = malloc(3*img->width*img->height*sizeof(uint8_t));
    if (img->pixels == NULL) {
        free(img);
        fprintf(stderr, "Could not allocate memory to pixels: %s\n", strerror(errno));
        return NULL;
    }
    fscanf(fp, "%d%*c", &maxval);
    fread(img->pixels, sizeof(uint8_t),3*img->width*img->height, fp);
    return img;
}
int save(char *filename, struct Image img)
{
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        return -1;
    }
    fprintf(fp, "P6\n%d %d\n255\n", img.width, img.height);
    fwrite(img.pixels, sizeof(uint8_t), 3*img.width*img.height, fp);
    return 0;
}
void grayscale(struct Image *img)
{
	uint8_t avg;
   	uint8_t r, g, b;
	for (int row = 0; row < img->height*3; row += 3) {
		for (int col = 0; col < img->width*3; col += 3) {
			r = img->pixels[row*img->width + col + 0];
			g = img->pixels[row*img->width + col + 1];
			b = img->pixels[row*img->width + col + 2];
			avg = ((int) (r+g+b))/3;
			img->pixels[row*img->width + col + 0] = avg;
			img->pixels[row*img->width + col + 1] = avg;
			img->pixels[row*img->width + col + 2] = avg;
		}
	}
}
void perceptual_grayscale(struct Image *img)
{
	uint8_t avg;
   	uint8_t r, g, b;
	for (int row = 0; row < img->height*3; row += 3) {
		for (int col = 0; col < img->width*3; col += 3) {
			r = img->pixels[row*img->width + col + 0];
			g = img->pixels[row*img->width + col + 1];
			b = img->pixels[row*img->width + col + 2];
            uint8_t gray_value = (uint8_t)(0.299 * r + 0.587 * g + 0.114 * b);
			img->pixels[row*img->width + col + 0] = b;
			img->pixels[row*img->width + col + 1] = r;
			img->pixels[row*img->width + col + 2] = g;
		}
	}
}
