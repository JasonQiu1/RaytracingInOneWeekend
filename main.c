#include <math.h>
#include <stdio.h>

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

int main() {
    // Image
    const double aspectRatio = 16.0 / 9.0;
    const int imageWidth = 400;
    const int imageHeight = imageWidth / aspectRatio;
    // samples per pixel
    const int spp = 100;
    const int maxBounces = 50;

    // World
    HittableList* world = HittableList_new();

    Lambertian groundMat;
    Lambertian centerMat;
    Dielectric leftMat;
    Metal rightMat;

    Lambertian_init(&groundMat, (Color){0.8,0.8,0.0});
    Lambertian_init(&centerMat, (Color){0.1,0.2,0.5});
    Dielectric_init(&leftMat, 1.5);
    Metal_init(&rightMat, (Color){0.8,0.6,0.2}, 0.0);

    Sphere s1, s2, s3, s4, s5;

    Sphere_init(&s1, (Vec3){0,-100.5,-1}, 100, (Material*)&groundMat);
    Sphere_init(&s2, (Vec3){0,0,-1}, 0.5, (Material*)&centerMat);
    Sphere_init(&s3, (Vec3){-1,0,-1}, 0.5, (Material*)&leftMat);
    Sphere_init(&s4, (Vec3){-1,0,-1}, -0.4, (Material*)&leftMat);
    Sphere_init(&s5, (Vec3){1,0,-1}, 0.5, (Material*)&rightMat);

    HittableList_add(world, (Hittable*)&s1);
    HittableList_add(world, (Hittable*)&s2);
    HittableList_add(world, (Hittable*)&s3);
    HittableList_add(world, (Hittable*)&s4);
    HittableList_add(world, (Hittable*)&s5);

    // Camera
    Camera camera;
    Camera_init(&camera);

    // Render
    printf("P3\n%i %i\n%i\n", imageWidth, imageHeight, MAX_COLOR);

    double u, v;
    Ray r;
    Color color, curr;
    for (int i = imageHeight-1; i >= 0; i--) {
        fprintf(stderr, "Rows remaining: %5i\r", i);
        fflush(stderr);
        for (int j = 0; j < imageWidth; j++) {
            color = (Color){0,0,0};
            for (int s = 0; s < spp; s++) {
                u = (j + randomDouble()) / (imageWidth - 1);
                v = (i + randomDouble()) / (imageHeight - 1);
                Camera_getRay(&r, &camera, u, v);
                Ray_color(&curr, &r, (Hittable*)world, maxBounces);
                Vec3_add(&color, &color, &curr);
            }

            Color_fprintln(stdout, &color, spp);
        }
    }

    fprintf(stderr, "\nDone.\n");
    return 0;
}
