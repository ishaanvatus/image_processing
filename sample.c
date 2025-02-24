#include <stdio.h>
#include <inttypes.h>

int main()
{
    float f = -192;
    printf("%0f\n", f);
    uint8_t fint = (uint8_t) f;
    printf(" %" SCNu8 "\n", fint);
}
