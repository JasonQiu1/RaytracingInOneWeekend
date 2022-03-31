#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
#include "vector3d.h"

typedef struct Camera {
    Vec3 origin;
    Vec3 botLeftCorner;
    double viewWidth;
    double viewHeight;
} Camera;

// Initializes a camera object
Camera* Camera_init(Camera* c);

// Sets res the the ray pointing from c to (u,v) in view space
Ray* Camera_getRay(Ray* res, Camera* c, double u, double v);

#endif
