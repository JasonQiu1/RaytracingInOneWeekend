#ifndef COLOR_H
#define COLOR_H

#include <stdio.h>

#include "vector3d.h"

#define MAX_COLOR 255

typedef Vec3 Color;

// Processes a color, adding gamma and clamping it, etc.
Color* Color_process(Color* c, int spp);

// 'spp' is samples per pixel
int Color_fprintln(FILE* stream, const Color* c, int spp);

#endif
