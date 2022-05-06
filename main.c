#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <omp.h>

#include "bvh.h"
#include "camera.h"
#include "color.h"
#include "hittable.h"
#include "hittableList.h"
#include "material.h"
#include "ray.h"
#include "sphere.h"
#include "util.h"
#include "vector3d.h"

#define MAX_COLOR 255

Color* Ray_color(Color* res, const Ray* r, const Hittable* world, 
                 int bouncesLeft) {
    if (bouncesLeft <= 0) {
        *res = (Color){0,0,0};
        return res;
    }

    Hittable record;
    if (world->hit(&record, world, r, 0.001, INFINITY)) {
        Ray scatteredRay;
        Color attenuation;
        Color stickyRes;
        if (record.material->scatteredRay(&scatteredRay, &attenuation, r,
                                          &record)) {
            return Vec3_mult(res, &attenuation, 
                    Ray_color(&stickyRes, &scatteredRay, world, bouncesLeft-1));
        }
        *res = (Color){0,0,0};
        return res;
    }

    Vec3 dir;
    Vec3_norm(&dir, &r->dir);

    double t = 0.5*(dir.y + 1.0); // make y e [0,1]
    Color gradientHigh = {1.0, 1.0, 1.0};
    Color gradientLow = {0.5, 0.7, 1.0};
    Vec3_multS(&gradientHigh, &gradientHigh, (1 - t));
    Vec3_multS(&gradientLow, &gradientLow, t);
    Vec3_add(res, &gradientHigh, &gradientLow);
    return res;
}

HittableList* randomScene(Vector* objects, Vector* materials) {
    HittableList* world = HittableList_new();

    Lambertian* groundMat = malloc(sizeof *groundMat);
    Vector_push(materials, groundMat);
    Lambertian_init(groundMat, (Color){0.5,0.5,0.5});

    Sphere* ground = malloc(sizeof *ground);
    Sphere_init(ground, (Vec3){0,-1000,0}, 1000, (Material*)groundMat);
    Vector_push(objects, (Hittable*)ground);
    
    Vec3 spot = {4,0.2,0}, os, rand1, rand2;
    for (int x = -11; x < 11; x++) {
        for (int y = -11; y < 11; y++) {
            Vec3 center = {x + 0.9*randomDouble(), 0.2, y + 0.9*randomDouble()};
            Vec3_sub(&os, &center, &spot);
            if (Vec3_mag(&spot) > 0.9) {
                double chooseMat = randomDouble();
                if (chooseMat < 0.8) {
                    // diffuse/matte
                    Vec3_randomRange(&rand1, 0, 1);
                    Vec3_randomRange(&rand2, 0, 1);
                    Vec3_mult(&rand1, &rand1, &rand2);

                    Lambertian* mat = malloc(sizeof *mat);
                    Vector_push(materials, mat);
                    Lambertian_init(mat, rand1);

                    Sphere* sph = malloc(sizeof *sph);
                    Sphere_init(sph, center, 0.2, (Material*)mat);
                    Vector_push(objects, (Hittable*)sph);
                } else if (chooseMat < 0.95) {
                    // metal
                    Vec3_randomRange(&rand1, 0.5, 1);

                    Metal* mat = malloc(sizeof *mat);
                    Vector_push(materials, mat);
                    Metal_init(mat, rand1, randomDouble());

                    Sphere* sph = malloc(sizeof *sph);
                    Sphere_init(sph, center, 0.2, (Material*)mat);
                    Vector_push(objects, (Hittable*)sph);
                } else {
                    // glass
                    Dielectric* mat = malloc(sizeof *mat);
                    Vector_push(materials, mat);
                    Dielectric_init(mat, 1.5);

                    Sphere* sph = malloc(sizeof *sph);
                    Sphere_init(sph, center, 0.2, (Material*)mat);
                    Vector_push(objects, (Hittable*)sph);
                }
            }
        }
    }

    Dielectric* m1 = malloc(sizeof *m1);
    Vector_push(materials, m1);
    Dielectric_init(m1, 1.5);

    Sphere* s1 = malloc(sizeof *s1);
    Sphere_init(s1, (Vec3){0,1,0}, 1.0, (Material*)m1);
    Vector_push(objects, (Hittable*)s1);

    Lambertian* m2 = malloc(sizeof *m2);
    Vector_push(materials, m2);
    Lambertian_init(m2, (Color){0.4,0.2,0.1});

    Sphere* s2 = malloc(sizeof *s2);
    Sphere_init(s2, (Vec3){-4,1,0}, 1.0, (Material*)m2);
    Vector_push(objects, (Hittable*)s2);

    Metal* m3 = malloc(sizeof *m3);
    Vector_push(materials, m3);
    Metal_init(m3, (Color){0.7,0.6,0.5},0.0);

    Sphere* s3 = malloc(sizeof *s3);
    Sphere_init(s3, (Vec3){4,1,0}, 1.0, (Material*)m3);
    Vector_push(objects, (Hittable*)s3);

    BVH* everything = BVH_new(objects, 0, objects->length, 0, 0);
    HittableList_add(world, (Hittable*)everything);

    return world;
}

#pragma omp declare reduction(Vec3_add : Color : \
        Vec3_add(&omp_out, &omp_out, &omp_in)) \
    initializer( omp_priv = { 0, 0, 0 } )

// Image
#define ASPECTRATIOW 16
#define ASPECTRATIOH 9
#define ASPECTRATIO (16.0 / 9.0)
#define IMAGEWIDTH 1200
#define IMAGEHEIGHT (IMAGEWIDTH * ASPECTRATIOH / ASPECTRATIOW)

// Image array
Color image[IMAGEHEIGHT][IMAGEWIDTH];

int main() {
    // samples per pixel
    const int spp = 100;
    const int maxBounces = 50;

    // World
    Vector* objects = Vector_new(8, sizeof(Hittable*));
    Vector* materials = Vector_new(8, sizeof(Material*));
    HittableList* world = randomScene(objects, materials);

    // Camera
    Camera camera;
    Vec3 lookFrom = {13,2,3}, lookAt = {0,0,0}, temp;
    Vec3_sub(&temp, &lookAt, &lookFrom);
    double focusDist = 10;
    Camera_init(&camera, lookFrom, lookAt, (Vec3){0,1,0}, 20, ASPECTRATIO, 0.1,
                focusDist);

    // Render
    printf("P3\n%i %i\n%i\n", IMAGEWIDTH, IMAGEHEIGHT, MAX_COLOR);

    int rowsLeft = IMAGEHEIGHT;
    omp_set_dynamic(0);
    omp_set_num_threads(omp_get_num_procs());
    #pragma omp parallel for
    for (int i = IMAGEHEIGHT-1; i >= 0; i--) {
        #pragma omp parallel for
        for (int j = 0; j < IMAGEWIDTH; j++) {
            Color color = { 0, 0, 0 };
            #pragma omp parallel for reduction(Vec3_add:color)
            for (int s = 0; s < spp; s++) {
                Color curr;
                Ray r;
                double u = (j + randomDouble()) / (IMAGEWIDTH - 1);
                double v = (i + randomDouble()) / (IMAGEHEIGHT - 1);
                Camera_getRay(&r, &camera, u, v);
                Ray_color(&curr, &r, (Hittable*)world, maxBounces);
                Vec3_add(&color, &color, &curr);
            }

            image[i][j] = color;
        }

        #pragma omp atomic
        rowsLeft--;
        fprintf(stderr, "Rows remaining: %5i\r", rowsLeft);
        fflush(stderr);
    }

    for (int i = IMAGEHEIGHT-1; i >= 0; i--) {
        for (int j = 0; j < IMAGEWIDTH; j++) {
            Color_fprintln(stdout, &image[i][j], spp);
        }
    }

    fprintf(stderr, "\nDone.\n");
    return 0;
}
