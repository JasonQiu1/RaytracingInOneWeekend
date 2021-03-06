#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include "hittable.h"
#include "ray.h"

typedef struct Hittable Hittable;

typedef struct Material Material;
struct Material {
    int(*scatteredRay)(Ray*, Color*, const Ray*, const Hittable*);
};

typedef struct Lambertian {
    Material base;
    Color albedo;
} Lambertian;

// Initialize a lambertian object.
Lambertian* Lambertian_init(Lambertian* l, Color albedo);

typedef struct Metal {
    Material base;
    Color albedo;
    double fuzz;
} Metal;

// Initialize a metal material object.
Metal* Metal_init(Metal* m, Color albedo, double fuzz);

typedef struct Dielectric {
    Material base;
    double ir; // index of refraction
} Dielectric;

// Initialize a dielectric material object.
Dielectric* Dielectric_init(Dielectric* d, double ir);

#endif
