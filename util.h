#ifndef UTIL_H
#define UTIL_H

#include <math.h>
#include <stdlib.h>

#include "vector3d.h"

#define NEAR_ZERO 1e-8
#define PI 3.14159265358979323846

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

inline double degToRad(double deg) {
    return deg * PI / 180.0;
}

// Returns a random real number [0,1)
inline double randomDouble() {
    return rand() / (RAND_MAX + 1.0);
}

// Returns a random real number [min,max)
inline double randomDoubleRange(double min, double max) {
    return min + (max - min)*randomDouble();
}

inline Vec3* Vec3_random(Vec3* res) {
    *res = (Vec3){randomDouble(), randomDouble(), randomDouble()};
    return res;
}

inline Vec3* Vec3_randomRange(Vec3* res, double min, double max) {
    *res = (Vec3){randomDoubleRange(min, max), randomDoubleRange(min, max),
                  randomDoubleRange(min, max)};
    return res;
}

inline Vec3* Vec3_randomInUnitDisk(Vec3* res) {
    do {
        res->x = randomDoubleRange(-1,1);
        res->y = randomDoubleRange(-1,1);
        res->z = 0;
    } while (Vec3_magSqr(res) >= 1);
    return res;
}

inline Vec3* Vec3_randomInUnitSphere(Vec3* res) {
    do {
        Vec3_randomRange(res, -1, 1);
    } while (Vec3_magSqr(res) >= 1);
    return res;
}

inline Vec3* Vec3_randomUnitVector(Vec3* res) {
    return Vec3_norm(res, Vec3_randomInUnitSphere(res));
}

inline Vec3* Vec3_randomInHemisphere(Vec3* res, const Vec3* normal) {
    Vec3_randomInUnitSphere(res);
    if (Vec3_dot(res, normal) > 0.0) return res;
    return Vec3_multS(res, res, -1);
}

// Returns 1 if each element is close to zero.
inline int Vec3_nearZero(Vec3* v) {
    return (fabs(v->x) < NEAR_ZERO) && (fabs(v->y) < NEAR_ZERO) && (fabs(v->z) < NEAR_ZERO);
}

inline Vec3* Vec3_reflect(Vec3* res, const Vec3* in, const Vec3* normal) {
    return Vec3_sub(res, in, Vec3_multS(res, normal, 2*Vec3_dot(in, normal)));
}

// snell's law to find refracted vector
Vec3* Vec3_refract(Vec3* res, const Vec3* in, const Vec3* normal, 
                   double etaiOverEtat);

#endif
