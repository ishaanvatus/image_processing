#ifndef KERNELS_H
#define KERNELS_H
double GAUSSIAN_3[9] = {
    0.0625, 0.125, 0.0625,
    0.125, 0.25, 0.125, 
    0.0625, 0.125, 0.0625
};
double SOBEL_Y[9] = {
    +1, +0.00, -1,
    +2, +0.00, -2,
    +1, +0.00, -1
};
double SOBEL_X[9] = {
    +1, +2, +1,
    +0.00, +0.00, +0.00
    -1, -2, -1,
};
#endif
