#ifndef IMPROC_H
#define IMPROC_H
struct Image {
    size_t height;
    size_t width;
    size_t channels;
    double **data;
};

struct Image *create_image(size_t width, size_t height, size_t channels);
int save_image(const char *filename, struct Image image);
struct Image *load_image(const char *filename);
#endif
