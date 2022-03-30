#ifndef RAY_H
#define RAY_H

#include "vector3d.h"

typedef struct Ray {
    Vec3 origin;
    Vec3 dir;
} Ray;

// Sets res to the position of the ray after traveling in dir for time t.
int Ray_at(Vec3* res, const Ray* r, double t);

#endif
