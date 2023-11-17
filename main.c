#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "improc.h"

int main(int argc, char **argv)
{
    struct Image *ema;
    ema = load_image(argv[1]);
    save_image("identity.ppm", *ema);
    return 0;
}
