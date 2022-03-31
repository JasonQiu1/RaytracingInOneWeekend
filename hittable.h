#ifndef HITTABLE_H
#define HITTABLE_H

#include "material.h"
#include "ray.h"
#include "vector3d.h"

typedef struct Material Material;

typedef struct Hittable Hittable;
struct Hittable {
    int(*hit)(Hittable*, const Hittable*, const Ray*, double, double);
    Vec3 point;
    Vec3 normal;
    Material* material;
    //void(*setFaceNormal)(Hittable*, const Ray*, const Vec3*);
    int frontFace;
    double t;
};

inline void Hittable_setFaceNormal(Hittable* res, const Ray* r,
                                   const Vec3* outwardNormal) {
    res->frontFace = Vec3_dot(&r->dir, outwardNormal) < 0;
    res->normal = *outwardNormal;
    if (!res->frontFace) Vec3_multS(&res->normal, &res->normal, -1);
}

#endif
