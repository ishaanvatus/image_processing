#ifndef _scrimage_h
#define _scrimage_h
#define WIDTH 1920
#define HEIGHT 1080

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "improc.h"
int save(uint32_t *image, char *filename);
#endif
