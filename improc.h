#ifndef _scrimage_h
#define _scrimage_h

#include "improc.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
	double r;
	double g;
	double b;
} pixel;

typedef pixel Image[WIDTH][HEIGHT];
typedef double Kernel[K_SIZE][K_SIZE];

void fill_image(Image image, pixel color);
int save_image(Image image, char *filename);
void load_image(Image img, char *filename);
unsigned modulo(int value, unsigned m);
double k_min(Kernel kernel);
double k_max(Kernel kernel);
void convolve(Kernel kernel, Image source, Image transformed);
void sobel(Image input, Image sobel_out);
void grayscale(Image image);
void subtract(Image a, Image b, Image out);
void histogram(Image image, int hist[3][256]);

#endif
