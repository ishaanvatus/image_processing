#include <stdio.h>
#include <stdlib.h>
#include "improc.h"

int main(int argc, char **argv)
{
    (void) argc;
    /*
     * create 5x5 box blur kernel
     * 1.0 1.0 1.0 1.0 1.0
     * 1.0 1.0 1.0 1.0 1.0
     * 1.0 1.0 1.0 1.0 1.0
     * 1.0 1.0 1.0 1.0 1.0
     * 1.0 1.0 1.0 1.0 1.0
     * 1.0 1.0 1.0 1.0 1.0
    */
    Kernel box_blur;
    box_blur.size = 5;
    box_blur.weights = malloc(box_blur.size*box_blur.size*sizeof(double));
    for (int row = 0; row < box_blur.size; row++) {
        for (int  col = 0; col < box_blur.size; col++)
            box_blur.weights[row*box_blur.size + col] = 1.0;
    }

    Image *ema = load_image(argv[1]);
    Image *blur;
    save_image("identity.ppm", *ema);
    blur = convolve(*ema, box_blur);
    save_image("blur.ppm", *blur);
}
