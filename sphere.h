#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vector3d.h"

typedef struct Sphere {
    Hittable base;
    Vec3 center;
    double radius;
} Sphere;

// Initializes the given sphere.
Sphere* Sphere_init(Sphere* s, Vec3 center, double radius);

#endif
