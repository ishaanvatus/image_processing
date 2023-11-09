#include <stdio.h>
#include <stdlib.h>
#include "improc.h"

int main()
{
    struct Image *ema = load("ema.ppm");
    save("yasuhara.ppm", *ema);
}
