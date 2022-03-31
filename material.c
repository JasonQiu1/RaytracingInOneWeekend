#include "color.h"
#include "hittable.h"
#include "material.h"
#include "ray.h"
#include "util.h"

static int LAMBERTIAN_scatteredRay(Ray* res, Color* attenuation, const Ray* in,
                                   const Hittable* h) {
    res->origin = h->point;
    Vec3_randomInHemisphere(&res->dir, &h->normal);
    Vec3_add(&res->dir, &res->dir, &h->normal);

    // Change targets pointing directly down to the normal vector instead.
    if (Vec3_nearZero(&res->dir)) res->dir = h->normal;

    *attenuation = h->material->albedo;
    return 1;
}

// Initialize a lambertian object.
Lambertian* Lambertian_init(Lambertian* l, Color albedo) {
    l->base.scatteredRay = LAMBERTIAN_scatteredRay;
    l->base.albedo = albedo;
    return l;
}

static int METAL_scatteredRay(Ray* res, Color* attenuation, const Ray* in,
                              const Hittable* h) {
    res->origin = h->point;
    Vec3_reflect(&res->dir, &in->dir, Vec3_norm(&res->dir, &h->normal));
    // add fuzz
    Vec3 fuzzyUnitSphere;
    Vec3_multS(&fuzzyUnitSphere, Vec3_randomInUnitSphere(&fuzzyUnitSphere), 
               ((Metal*)h->material)->fuzz);
    Vec3_add(&res->dir, &res->dir, &fuzzyUnitSphere);
    *attenuation = h->material->albedo;
    return Vec3_dot(&res->dir, &h->normal) > 0;
}

// Initialize a metal material object.
Metal* Metal_init(Metal* m, Color albedo, double fuzz) {
    m->base.scatteredRay = METAL_scatteredRay;
    m->base.albedo = albedo;
    m->fuzz = fuzz;
    return m;
}
