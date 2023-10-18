#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "improc.h"

int main()
{
    uint32_t *sample = malloc(WIDTH*HEIGHT*sizeof(uint32_t));
    char *filename = "output.ppm";
    sample = load(filename);
    save(sample, "killme.ppm");
    return 0;
}
