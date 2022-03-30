#include <math.h>
#include <stdio.h>

#include "color.h"
#include "hittableList.h"
#include "ray.h"
#include "sphere.h"
#include "vector3d.h"

#define MAX_COLOR 255

Color Ray_color(const Ray* r, const Hittable* world) {
    Color res;
    Hittable record;
    if (world->hit(&record, world, r, 0, INFINITY)) {
        Vec3_addS(&res, &record.normal, 1);
        Vec3_multS(&res, &res, 0.5);
        return res;
    }

    Vec3 dir;
    Vec3_norm(&dir, &r->dir);

    double t = 0.5*(dir.y + 1.0); // make y e [0,1]
    Color gradientHigh = {1.0, 1.0, 1.0};
    Color gradientLow = {0.5, 0.7, 1.0};
    Vec3_multS(&gradientHigh, &gradientHigh, (1 - t));
    Vec3_multS(&gradientLow, &gradientLow, t);
    Vec3_add(&res, &gradientHigh, &gradientLow);
    return res;
}

int main() {
    // Image
    const double aspectRatio = 16.0 / 9.0;
    const int imageWidth = 400;
    const int imageHeight = imageWidth / aspectRatio;

    // World
    HittableList* world = HittableList_new();
    Sphere s1, s2;
    Sphere_init(&s1, (Vec3){0,0,-1}, 0.5);
    Sphere_init(&s2, (Vec3){0,-100.5,1}, 100);
    HittableList_add(world, (Hittable*)&s1);
    HittableList_add(world, (Hittable*)&s2);

    // Camera
    double viewHeight = 2.0;
    double viewWidth = viewHeight * aspectRatio;
    double focalLength = 1.0;

    Vec3 origin = {0, 0, 0};
    Vec3 botLeftCorner;
    {
        Vec3 temp = {viewWidth / 2, viewHeight / 2, focalLength};
        Vec3_sub(&botLeftCorner, &origin, &temp);
    }

    // Render
    printf("P3\n%i %i\n%i\n", imageWidth, imageHeight, MAX_COLOR);
    {
        double u, v;
        Vec3 dir, uv;
        Ray r;
        Color c;
        for (int i = imageHeight-1; i >= 0; i--) {
            fprintf(stderr, "\rRows remaining: %i", i);
            v = (double)i / (imageHeight - 1);
            for (int j = 0; j < imageWidth; j++) {
                u = (double)j / (imageWidth-1);
                uv = (Vec3){u * viewWidth, v * viewHeight, 0};
                // get view coordinates
                Vec3_sub(&dir, &botLeftCorner, &origin);
                Vec3_add(&dir, &dir, &uv);
                // raycast color
                r = (Ray){origin, dir};
                c = Ray_color(&r, (Hittable*)world);
                Color_fprintln(&c, stdout);
            }
        }
    }
    fprintf(stderr, "\nDone.\n");

    return 0;
}
