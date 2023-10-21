#include "improc.h"

void fill(uint32_t *image,  int color)
{
    for (int i = 0; i < HEIGHT*WIDTH; i++)
        image[i] = 16711935;
}
int save(uint32_t *image, char *filename)
{
    FILE *output = fopen(filename, "wb");
    if (output == NULL)
        return -1;
    fprintf(output, "P6\n%d %d\n255\n", WIDTH, HEIGHT);
    for (int row = 0; row < HEIGHT; row++) {
        for (int col = 0; col < WIDTH; col++) {
            uint8_t pixel[3] = 
            {
                (image[row*WIDTH + col] >> (8*3))&0xFF,
                (image[row*WIDTH + col] >> (8*2))&0xFF,
                (image[row*WIDTH + col] >> (8*1))&0xFF,
            };
            fwrite(pixel, sizeof(pixel), 1, output);
        }
    }
    fclose(output);
    return 0;
}
uint32_t *load(char *filename)
{
    FILE *input = fopen(filename, "rb");
    uint32_t *img = malloc(WIDTH*HEIGHT*sizeof(uint32_t));
    char buffer[16];
    fread(buffer, sizeof(buffer), 1, input);    
    for (int row = 0; row < HEIGHT; row++) {
        for (int col = 0; col < WIDTH; col++) {
            uint8_t pixel[3];
            fread(pixel, sizeof(pixel), 1, input);
            img[row*WIDTH + col] += (pixel[0] << (8*1)) | (pixel[1] << (8*3)) | (pixel[2] << (8*2));
        }
    }
    return img;
}













