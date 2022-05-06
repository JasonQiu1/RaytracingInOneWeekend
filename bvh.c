#include <stdlib.h>

#include "bvh.h"
#include "util.h"

static int BVH_hit(Hittable* res, const Hittable* hittable, const Ray* r, 
               double tMin, double tMax) {
    const BVH* bvh = (BVH*)hittable;
    if (AABB_hit(&bvh->box, r, tMin, tMax) == 0) return 0; 

    int hitLeft = bvh->left->hit(res, bvh->left, r, tMin, tMax); 
    int hitRight = bvh->right->hit(res, bvh->right, r, tMin, 
                                   hitLeft ? res->t : tMax);
    return hitLeft || hitRight;
}

static int BVH_boundingBox(AABB* res, const Hittable* hittable, double t0, 
                       double t1) {
    *res = ((BVH*)hittable)->box;
    return 1;
}

static inline int Hittable_compareBox(const void* a, const void* b, 
                                      size_t axis) {
    AABB boxA, boxB;
    (*(Hittable**)a)->boundingBox(&boxA, *(Hittable**)a, 0, 0);
    (*(Hittable**)b)->boundingBox(&boxB, *(Hittable**)b, 0, 0);
    return Vec3_at(&boxA.min, axis) > Vec3_at(&boxB.min, axis);
}

static int Hittable_compareBoxX(const void* a, const void* b) {
    return Hittable_compareBox(a, b, 0); 
}

static int Hittable_compareBoxY(const void* a, const void* b) {
    return Hittable_compareBox(a, b, 1); 
}

static int Hittable_compareBoxZ(const void* a, const void* b) {
    return Hittable_compareBox(a, b, 2); 
}

// Elements in object vector are moved around, 
// but none are removed and no new objects are added.
// Returns a malloced BVH. 
BVH* BVH_new(Vector* objects, size_t start, size_t end, double t0, double t1) {
    BVH* res = malloc(sizeof *res);
    res->base.hit = BVH_hit;
    res->base.boundingBox = BVH_boundingBox;
    
    // build BVH
    int axis = (int)randomDoubleRange(0,3);
    int(*compare)(const void*, const void*) = (axis == 0) ? Hittable_compareBoxX
                                            : (axis == 1) ? Hittable_compareBoxY
                                            : Hittable_compareBoxZ;

    size_t length = end - start;
    Hittable* obj1 = Vector_at(objects, start);
    if (length == 1) {
        res->left = res->right = obj1;
    } else if (length == 2) {
        Hittable* obj2 = Vector_at(objects, start+1);
        if (compare(&obj1, &obj2)) {
            res->left = obj2;
            res->right = obj1;
        } else {
            res->left = obj1;
            res->right = obj2;
        }
    } else {
        qsort(objects->array + start, length, objects->width, compare);

        int mid = start + length/2;
        res->left = (Hittable*)BVH_new(objects, start, mid, t0, t1);
        res->right = (Hittable*)BVH_new(objects, mid, end, t0, t1);
    }

    AABB left, right;
    if (!res->left->boundingBox(&left, res->left, t0, t1) || 
        !res->right->boundingBox(&right, res->right, t0, t1))
    {
        free(res);
        return NULL;
    }
    AABB_surroundingBox(&res->box, &left, &right);

    return res;
}
