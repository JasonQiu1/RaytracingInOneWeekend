#include <math.h>
#include <stdio.h>
#include <util.h>

#include "color.h"

Color* Color_process(Color* c, int spp) {
    // divide color by sample size
    double scale = 1.0 / spp;
    double r = sqrt(c->x * scale);
    double g = sqrt(c->y * scale);
    double b = sqrt(c->z * scale);
    c->x = (int)(256 * clamp(r, 0, 0.999));
    c->y = (int)(256 * clamp(g, 0, 0.999));
    c->z = (int)(256 * clamp(b, 0, 0.999));
    return c;
}

// 'spp' is samples per pixel
int Color_fprintln(FILE* stream, const Color* c, int spp) {
    // divide color by sample size
    double scale = 1.0 / spp;
    double r = sqrt(c->x * scale);
    double g = sqrt(c->y * scale);
    double b = sqrt(c->z * scale);

    return fprintf(stream, "%i %i %i\n", 
                   (int)(256 * clamp(r, 0, 0.999)),
                   (int)(256 * clamp(g, 0, 0.999)),
                   (int)(256 * clamp(b, 0, 0.999))
           );
}
