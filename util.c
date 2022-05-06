#include "util.h"

// snell's law to find refracted vector
Vec3* Vec3_refract(Vec3* res, const Vec3* normalizedIn, 
                   const Vec3* normalizedNormal, double etaiOverEtat) {
    Vec3 negIn;
    Vec3_multS(&negIn, normalizedIn, -1);
    double cosTheta = fmin(Vec3_dot(&negIn, normalizedNormal), 1.0);
    Vec3 resPerp;
    Vec3_multS(&resPerp, 
               Vec3_add(&resPerp, normalizedIn, 
                        Vec3_multS(&resPerp, normalizedNormal, cosTheta)), 
               etaiOverEtat);
    Vec3 resParallel;
    Vec3_multS(&resParallel,
               normalizedNormal,
               -sqrt(fabs(1.0 - Vec3_magSqr(&resPerp))));
    return Vec3_add(res, &resPerp, &resParallel);
}

double clamp(double x, double min, double max);
double degToRad(double deg);
double randomDouble();
double randomDoubleRange(double min, double max);
Vec3* Vec3_random(Vec3* res);
Vec3* Vec3_randomRange(Vec3* res, double min, double max);
Vec3* Vec3_randomInUnitDisk(Vec3* res);
Vec3* Vec3_randomInUnitSphere(Vec3* res);
Vec3* Vec3_randomUnitVector(Vec3* res);
Vec3* Vec3_randomInHemisphere(Vec3* res, const Vec3* normal);
int Vec3_nearZero(Vec3* v);
Vec3* Vec3_reflect(Vec3* res, const Vec3* in, const Vec3* normal);
