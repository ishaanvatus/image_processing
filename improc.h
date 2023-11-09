#ifndef IMPROC_H
#define IMPROC_H
struct Image
{
    int height;
    int width;
    int *pixels;
};
struct Image *load(char *filename);
int save(char *filename, struct Image img);
#endif
