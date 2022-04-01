#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
#include "vector3d.h"

typedef struct Camera {
    Vec3 origin;
    Vec3 botLeftCorner;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 u, v, w;
    double lensRadius;
} Camera;

// Initializes a camera object
Camera* Camera_init(Camera* c, Vec3 lookFrom, Vec3 lookAt, Vec3 upVec, 
                    double fovV, double aspectRatio, double aperture, 
                    double focusDist);

// Sets res the the ray pointing from c to (u,v) in view space
Ray* Camera_getRay(Ray* res, Camera* c, double u, double v);

#endif
