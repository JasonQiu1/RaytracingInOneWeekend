#include "camera.h"
#include "ray.h"
#include "vector3d.h"

// Initializes a camera object
Camera* Camera_init(Camera* c) {
    double aspectRatio = 16.0 / 9.0;
    double focalLength = 1.0;

    c->viewHeight = 2.0;
    c->viewWidth = c->viewHeight * aspectRatio;
    c->origin = (Vec3){0, 0, 0};

    Vec3 temp = {c->viewWidth / 2, c->viewHeight / 2, focalLength};
    Vec3_sub(&c->botLeftCorner, &c->origin, &temp);

    return c;
}

// Sets res to the ray pointing from c to (u,v) in view space
Ray* Camera_getRay(Ray* res, Camera* c, double u, double v) {
    res->origin = c->origin;
    Vec3 uv = (Vec3){u * c->viewWidth, v * c->viewHeight, 0};
    // get view coordinates
    Vec3_sub(&res->dir, &c->botLeftCorner, &c->origin);
    Vec3_add(&res->dir, &res->dir, &uv);
    return res;
}
