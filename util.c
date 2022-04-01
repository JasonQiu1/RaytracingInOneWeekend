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
