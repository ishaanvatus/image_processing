#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "improc.h"
int main()
{
    struct Image *sample = malloc(sizeof(struct Image));
    sample->height = 1080;
    sample->width = 1920;
    sample->pixels = malloc(sample->height*sample->width*sizeof(uint32_t));
    char *filename = "input.ppm";
    sample = load(filename);
    save(sample, "output.ppm");
    return 0;
}
