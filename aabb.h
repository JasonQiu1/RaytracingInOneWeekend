#ifndef AABB_H
#define AABB_H

#include "ray.h"

typedef struct AABB {
    Vec3 min;
    Vec3 max;
} AABB;

// Returns 1 if the ray hits the box within tMin and tMax, and 0 otherwise.
int AABB_hit(const AABB* box, const Ray* r, double tMin, double tMax);

// Sets res to the minimum AABB containing both box a and b.
AABB* AABB_surroundingBox(AABB* res, AABB* a, AABB* b);

#endif
