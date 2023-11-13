#include <stdio.h>
#include <stdlib.h>
#include "improc.h"

int main(int argc, char **argv)
{
    (void) argc;
    Image *ema = load_image(argv[1]), *perc, *gray;
    save_image("identity.ppm", *ema);
    gray = grayscale(*ema);
    save_image("grayscale.ppm", *gray);
    free_image(gray);
    perc = perceptual_grayscale(*ema);
    save_image("perceptual.ppm", *perc);
}
