#ifndef BVH_H
#define BVH_H

#include "aabb.h"
#include "hittable.h"
#include "vector.h"

typedef struct BVH {
    Hittable base; 
    Hittable* left;
    Hittable* right;
    AABB box;
} BVH;

// Elements in object vector are moved around, 
// but none are removed and no new objects are added.
// Returns a malloced BVH. 
BVH* BVH_new(Vector* objects, size_t start, size_t end, double t0, double t1);

#endif
