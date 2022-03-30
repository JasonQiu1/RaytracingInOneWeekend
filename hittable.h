#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "vector3d.h"

typedef struct Hittable Hittable;
struct Hittable {
    int(*hit)(Hittable*, const Hittable*, const Ray*, double, double);
    Vec3 point;
    Vec3 normal;
    void(*setFaceNormal)(Hittable* hittable, const Ray* r,
                         const Vec3* outwardNormal);
    int frontFace;
    double t;
};

inline void HITTABLE_setFaceNormal(Hittable* hittable, const Ray* r,
                                   const Vec3* outwardNormal) {
    hittable->frontFace = Vec3_dot(&r->dir, outwardNormal) < 0;
    hittable->normal = *outwardNormal;
    if (!hittable->frontFace) Vec3_multS(&hittable->normal, &hittable->normal, -1);
}

#endif
