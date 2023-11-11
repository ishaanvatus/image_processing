#include <stdio.h>
#include <stdlib.h>
#include "improc.h"

int main(int argc, char **argv)
{
    struct Image *ema = load(argv[1]);
    save("identity.ppm", *ema);
    grayscale(ema);
    save("grayscale.ppm", *ema);
    perceptual_grayscale(ema);
    save("perceptual.ppm", *ema);
}
