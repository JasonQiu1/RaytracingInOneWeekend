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

    *attenuation = ((Lambertian*)h->material)->albedo;
    return 1;
}

// Initialize a lambertian object.
Lambertian* Lambertian_init(Lambertian* l, Color albedo) {
    l->base.scatteredRay = LAMBERTIAN_scatteredRay;
    l->albedo = albedo;
    return l;
}

static int METAL_scatteredRay(Ray* res, Color* attenuation, const Ray* in,
                              const Hittable* h) {
    res->origin = h->point;
    Vec3_reflect(&res->dir, &in->dir, &h->normal);
    // add fuzz
    Vec3 fuzzyUnitSphere;
    Vec3_multS(&fuzzyUnitSphere, Vec3_randomInUnitSphere(&fuzzyUnitSphere), 
               ((Metal*)h->material)->fuzz);
    Vec3_add(&res->dir, &res->dir, &fuzzyUnitSphere);
    *attenuation = ((Metal*)h->material)->albedo;
    return Vec3_dot(&res->dir, &h->normal) > 0;
}

// Initialize a metal material object.
Metal* Metal_init(Metal* m, Color albedo, double fuzz) {
    m->base.scatteredRay = METAL_scatteredRay;
    m->albedo = albedo;
    m->fuzz = fuzz;
    return m;
}

// Schlick's approximation for reflectance
static double DIELECTRIC_reflectance(double cosTheta, double ir) {
    double r0 = (1-ir) / (1+ir);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosTheta), 5);
}

static int DIELECTRIC_scatteredRay(Ray* res, Color* attenuation, const Ray* in,
                                   const Hittable* h) {
    res->origin = h->point;
    double refractionRatio = h->frontFace ? 
        (1.0 / ((Dielectric*)h->material)->ir) : ((Dielectric*)h->material)->ir;
    Vec3 normalizedDir;
    Vec3_norm(&normalizedDir, &in->dir);
    Vec3_multS(&normalizedDir, &normalizedDir, -1);
    double cosTheta = fmin(Vec3_dot(&normalizedDir, &h->normal), 1.0);
    double sinTheta = sqrt(1 - cosTheta*cosTheta);
    
    if (refractionRatio * sinTheta > 1 || 
            DIELECTRIC_reflectance(cosTheta, refractionRatio) > randomDouble()) 
    {
        // reflects 
        Vec3_reflect(&res->dir, &in->dir, &h->normal);
    } else {
        // can refract
        Vec3_multS(&normalizedDir, &normalizedDir, -1);
        Vec3_refract(&res->dir, &normalizedDir, &h->normal, refractionRatio);
    }

    *attenuation = (Color){1,1,1};
    return 1;
}

// Initialize a dielectric material object.
Dielectric* Dielectric_init(Dielectric* d, double ir) {
    d->base.scatteredRay = DIELECTRIC_scatteredRay;
    d->ir = ir;
    return d;
}
