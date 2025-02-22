#include "improc.h"
#include <stdio.h>
int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "program usage: ./main 'image name'\n");
        return -1;
    }
    Image *image = open_image(argv[1]);
    const char *output_name = "drowning.ppm";
    save_image(image, (char *) output_name);
    return 0;
}
