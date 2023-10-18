#include "improc.h"

int save(uint32_t *image, char *filename)
{
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL)
        return -1;
    fprintf(fp, "P6\n%d %d\n255\n", WIDTH, HEIGHT);
    for (int row = 0; row < HEIGHT; row++) {
        for (int col = 0; col < WIDTH; col++) {
            uint8_t pixel[3] = 
            {
                0, 0, 0
            };
            fwrite(pixel, sizeof(pixel), 1, fp);
        }
    }
    fclose(fp);
    return 0;
}
