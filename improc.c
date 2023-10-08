#include "improc.h"

int save_image(Image image, char *filename)
{
	FILE *fp = fopen(filename, "w");
	if (fp == NULL)
	{
		return -1;
	}
	fprintf(fp, "P3\n");
	fprintf(fp, "%d %d\n", WIDTH, HEIGHT);
	fprintf(fp, "255\n");
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			fprintf(fp, "%d %d %d\n", 
					(int) image[x][y].r, (int) image[x][y].g, (int) image[x][y].b);
		}
	}
	fclose(fp);
	return 0;
}

void load_image(Image img, char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) 
	{
        fprintf(stderr, "Error: could not open file '%s'\n", filename);
        exit(1);
    }

    char magic[3];
    int width, height, maxval;
    fscanf(fp, "%s\n%d %d\n%d\n", magic, &width, &height, &maxval);

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int r, g, b;
            fscanf(fp, "%d %d %d", &r, &g, &b);
            img[j][i].r = r;
            img[j][i].g = g;
            img[j][i].b = b;
        }
    }

    fclose(fp);
}
void fill_image(Image image, pixel color)
{
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			image[x][y] = color;
		}
	}
}
unsigned modulo(int value, unsigned m)
{
	int mod = value % (int) m;
	if (mod < 0) mod += m;
	return mod;
}
double k_max(Kernel kernel)
{
	double max = 0;
	for (int i = 0; i < K_SIZE; i++)
	{
		for (int j = 0; j < K_SIZE; j++)
		{
			if (kernel[i][j] >= 0)
			{
				max += 255.0*kernel[i][j];
			}
		}
	}
	return max;
}
double k_min(Kernel kernel)
{
	double min = 0;
	for (int i = 0; i < K_SIZE; i++)
	{
		for (int j = 0; j < K_SIZE; j++)
		{
			if (kernel[i][j] < 0)
			{
				min += 255.0*kernel[i][j];
			}
		}
	}
	return min;
}
void convolve(Kernel kernel, Image source, Image transformed)
{
	double min = k_min(kernel);
	double max = k_max(kernel);
	double alpha = (max - min);
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			pixel accumulator = {0, 0, 0};
			for (int row_offset = -K_SIZE/2; row_offset <= K_SIZE/2; row_offset++)
			{
				for (int col_offset = -K_SIZE/2; col_offset <= K_SIZE/2; col_offset++)
				{
					int x = modulo(j + col_offset, WIDTH);
					int y = modulo(i + row_offset, HEIGHT);

					int x_k =  col_offset + K_SIZE/2;
					int y_k =  row_offset + K_SIZE/2;
					accumulator.r += kernel[x_k][y_k]*source[x][y].r;	
					accumulator.g += kernel[x_k][y_k]*source[x][y].g;	
					accumulator.b += kernel[x_k][y_k]*source[x][y].b;	
				}
			}

			/*
			accumulator.r = (accumulator.r - min)*(255.0/alpha);
			accumulator.b = (accumulator.b - min)*(255.0/alpha);
			accumulator.g = (accumulator.g - min)*(255.0/alpha);
			*/
			accumulator.r = (accumulator.r)*(255.0/alpha);
			accumulator.b = (accumulator.b)*(255.0/alpha);
			accumulator.g = (accumulator.g)*(255.0/alpha);
			/*
			accumulator.r = (accumulator.r)*(255.99/alpha);
			accumulator.g = (accumulator.g)*(255.99/alpha);
			accumulator.b = (accumulator.b)*(255.99/alpha);
			*/


			transformed[j][i] = accumulator;
		}
	}
}

void sobel(Image input, Image sobel_out)
{
	static Image sobel_x, sobel_y;
    /*
	Kernel s_x = 
	{
		{+1, 0, -1},
		{+2, 0, -2},
		{+1, 0, -1}
	};
	Kernel s_y = 
	{
		{1, 2, 1},
		{0, 0, 0},
		{-1, -2, -1}
	};
    */
	Kernel s_x = 
	{
		{1.0,	2.0,	0.0,	-2.0,	-1.0},
		{4.0,	8.0,	0.0,  	-8.0, 	-4.0},
		{6.0,	12.0, 	0.0, 	-12.0,	-6.0},
		{4.0,	8.0, 	0.0,  	-8.0, 	-4.0},
		{1.0,	2.0, 	0.0,  	-2.0, 	-1.0}
	};
	Kernel s_y = 
	{
		{1.0,	4.0,	6.0,	4.0,	1.0},
		{2.0,	8.0,	12.0,	8.0,	2.0},
		{0.0,	0.0,	0.0,	0.0,	0.0},
		{-2.0,	-8.0,	-12.0,	-8.0,	-2.0},
		{-1.0,	-4.0,	-6.0,	-4.0,	-1.0}
	};
	convolve(s_x, input, sobel_x);
	convolve(s_y, input, sobel_y);
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			sobel_out[j][i].r = 
			sqrt(sobel_x[j][i].r*sobel_x[j][i].r + sobel_y[j][i].r*sobel_y[j][i].r);

			sobel_out[j][i].g = 
			sqrt(sobel_x[j][i].g*sobel_x[j][i].g + sobel_y[j][i].g*sobel_y[j][i].g);

			sobel_out[j][i].b = 
			sqrt(sobel_x[j][i].b*sobel_x[j][i].b + sobel_y[j][i].b*sobel_y[j][i].b);
		}
	}
}
void grayscale(Image image)
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			double avg = (image[j][i].r + image[j][i].g + image[j][i].b) / 3;
			image[j][i].r = avg;
			image[j][i].g = avg;
			image[j][i].b = avg;
		}
	}
}
void subtract(Image a, Image b, Image out)
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			out[j][i].r = modulo(a[j][i].r - b[j][i].r, 256);
			out[j][i].g = modulo(a[j][i].g - b[j][i].g, 256);
			out[j][i].b = modulo(a[j][i].b - b[j][i].b, 256);
		}
	}
}

void histogram(Image image, int hist[3][256])
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			hist[0][(int)image[j][i].r] += 1;
			hist[1][(int)image[j][i].g] += 1;
			hist[2][(int)image[j][i].b] += 1;
		}
	}
}
