#include <stdio.h>

#include "color.h"
#include "vector3d.h"

#define MAX_COLOR 255

int main() {
    const int width = 256;
    const int height = 256;

    printf("P3\n%i %i\n%i\n", width, height, MAX_COLOR);
    for (int i = height-1; i >= 0; i--) {
        fprintf(stderr, "\rRows remaining: %i", i);
        for (int j = 0; j < width; j++) {
            Color c = {(double)i/(width-1), (double)j/(height-1), 0.25};
            Color_fprintln(&c, stdout);
        }
    }
    fprintf(stderr, "\nDone.\n");

    return 0;
}
