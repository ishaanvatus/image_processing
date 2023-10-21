#include "improc.h"

void fill(struct Image *image,  int color)
{
    for (int i = 0; i < image->height*image->width; i++)
        image->pixels[i] = color;
}
int save(struct Image *image, char *filename)
{
    FILE *output = fopen(filename, "wb");
    if (output == NULL)
        return -1;
    fprintf(output, "P6\n%d %d\n255\n", image->width, image->height);
    for (int row = 0; row < image->height; row++) {
        for (int col = 0; col < image->width; col++) {
            uint8_t pixel[3] = 
            {
                (image->pixels[row*image->width + col] >> (8*3))&0xFF,
                (image->pixels[row*image->width + col] >> (8*2))&0xFF,
                (image->pixels[row*image->width + col] >> (8*1))&0xFF,
            };
            fwrite(pixel, sizeof(pixel), 1, output);
        }
    }
    fclose(output);
    return 0;
}
struct Image *load(char *filename)
{
    FILE *input = fopen(filename, "rb");
    struct Image *image = malloc(sizeof(struct Image));
    char p;
    int type, height, width, max_val;
    fscanf(input, "%c%d\n%d %d\n%d\n", &p, &type, &width, &height, &max_val);
    image->height = height;
    image->width = width;
    image->pixels = malloc(height*width*sizeof(uint32_t));
    for (int row = 0; row < image->height; row++) {
        for (int col = 0; col < image->width; col++) {
            uint8_t pixel[3];
            fread(pixel, sizeof(pixel), 1, input);
            image->pixels[row*image->width + col] = (pixel[0] << (8*3)) | (pixel[1] << (8*2)) | (pixel[2] << (8*1));
        }
    }
    return image;
}













