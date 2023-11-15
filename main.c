#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "improc.h"

int main(int argc, char **argv)
{
    (void) argc;
    Image *ema = load_image(argv[1]);
    save_image("identity.ppm", *ema);
    Image *sob = sobel(*ema);
    save_image("sobel.ppm", *sob);
}
