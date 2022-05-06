#include "aabb.h"
#include "vector3d.h"

// Returns 1 if the ray hits the box within tMin and tMax, and 0 otherwise.
int AABB_hit(const AABB* box, const Ray* r, double tMin, double tMax) {
    for (int i = 0; i < 3; i++) {
        double invDir = 1.0 / Vec3_at(&r->dir, i);
        double t0 = (Vec3_at(&box->min, i) - Vec3_at(&r->origin, i)) * invDir;
        double t1 = (Vec3_at(&box->max, i) - Vec3_at(&r->origin, i)) * invDir;
        if (invDir < 0.0) {
            double temp = t0;
            t0 = t1;
            t1 = temp;
        }
        tMin = t0 > tMin ? t0 : tMin;
        tMax = t1 < tMax ? t1 : tMax;
        if (tMax <= tMin) {
            return 0;
        }
    }

    return 1;
}

// Sets res to the minimum AABB containing both box a and b.
// Returns res.
AABB* AABB_surroundingBox(AABB* res, AABB* a, AABB* b) {
    Vec3_lt(&res->min, &a->min, &b->min);
    Vec3_gt(&res->max, &a->max, &b->max);
    return res;
}
