#include <math.h>

#include "sphere.h"

static int SPHERE_hit(Hittable* res, const Hittable* hittable, const Ray* r, 
                      double tMin, double tMax) {
    Sphere* s = (Sphere*)hittable;
    Vec3 oc;
    Vec3_sub(&oc, &r->origin, &s->center);
    double a = Vec3_magSqr(&r->dir);
    double halfB = Vec3_dot(&oc, &r->dir);
    double c = Vec3_magSqr(&oc) - (s->radius*s->radius);
    double discriminant = (halfB*halfB - a*c);
    if (discriminant < 0) return 0;

    // Find nearest root in the given range
    double sqrtD = sqrt(discriminant);
    double root = (-halfB - sqrtD) / a;
    if (root < tMin || root > tMax) {
        root = (-halfB + sqrtD) / a;
        if (root < tMin || root > tMax) return 0;
    }

    // Set hitrecord
    res->t = root;
    Ray_at(&res->point, r, res->t);
    Vec3 outwardNormal;
    Vec3_sub(&outwardNormal, &res->point, &s->center);
    Vec3_divS(&outwardNormal, &outwardNormal, s->radius);
    Hittable_setFaceNormal(res, r, &outwardNormal);
    res->material = hittable->material; 

    return 1;
}

// Initializes the given sphere.
Sphere* Sphere_init(Sphere* s, Vec3 center, double radius, Material* material) {
    s->base.hit = SPHERE_hit;
    s->base.material = material;
    s->center = center;
    s->radius = radius;
    return s;
}
