#include <stdio.h>
#include <stdlib.h>
#include "improc.h"

int main(int argc, char **argv)
{
    (void) argc;
    Image *ema = load(argv[1]), *perc, *gray;
    save("identity.ppm", *ema);
    gray = grayscale(*ema);
    save("grayscale.ppm", *gray);
    free_image(gray);
    perc = perceptual_grayscale(*ema);
    save("perceptual.ppm", *perc);
}
