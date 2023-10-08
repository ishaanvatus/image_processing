#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "improc.h"
#include "config.h"

int main()
{
    static Image input, blurred;
    Kernel box_blur = {
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1}
    };
    load_image(input, "bocchi_ascii.ppm");
    convolve(box_blur, input, blurred);
    save_image(blurred, "box_blur.ppm");
}
