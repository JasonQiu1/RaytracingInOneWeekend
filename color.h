#ifndef COLOR_H
#define COLOR_H

#include <stdio.h>

#include "vector3d.h"

#define MAX_COLOR 255

typedef Vec3 Color;

int Color_fprintln(Color* c, FILE* stream) {
    return fprintf(stream, "%i %i %i\n", 
                   (int)((MAX_COLOR + 0.999) * c->x),
                   (int)((MAX_COLOR + 0.999) * c->y),
                   (int)((MAX_COLOR + 0.999) * c->z)
           );
}

#endif
