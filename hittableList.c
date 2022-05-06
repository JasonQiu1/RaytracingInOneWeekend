#include <stdlib.h>

#include "hittableList.h"

// For a given ray, set hittable to the closest object hit.
// Returns 1 if an object was hit.
// Otherwise, returns 0;
static int HITTABLELIST_hit(Hittable* res, const Hittable* hittable,  
                     const Ray* r, double tMin, double tMax) {
    const HittableList* hittableList = (const HittableList*)hittable;
    Hittable temp;
    int hitAnything = 0;
    double closestSoFar = tMax;

    for (size_t i = 0; i < hittableList->objects->length; i++) {
        Hittable* object = (Hittable*)Vector_at(hittableList->objects, i);
        if (object->hit(&temp, object, r, tMin, closestSoFar)) {
            hitAnything = 1;
            closestSoFar = temp.t;
            *res = temp;
        }
    }

    return hitAnything;
}

// Creates the AABB containing all objects.
// Returns 0 if a bounding box cannnot be created, and 1 otherwise.
static int HITTABLELIST_boundingBox(AABB* res, const Hittable* hittable,
                                    double t0, double t1) {
    const HittableList* hl = (HittableList*)hittable;
    if (hl->objects->length == 0) return 0;

    AABB tempBox;
    int firstBox = 1;

    for (int i = 0; i < hl->objects->length; i++) {
        Hittable* object = (Hittable*)Vector_at(hl->objects, i);
        if (object->boundingBox(&tempBox, object, t0, t1) == 0) return 0;
        if (firstBox) {
            *res = tempBox;
            firstBox = 0;
        } else {
            AABB_surroundingBox(res, res, &tempBox);
        }
    }

    return 1;
}

HittableList* HittableList_new() {
    HittableList* hl = malloc(sizeof *hl);
    hl->base.hit = HITTABLELIST_hit;
    hl->base.boundingBox = HITTABLELIST_boundingBox;
    hl->objects= Vector_new(16, sizeof(Hittable*));
    return hl;
}

void HittableList_free(HittableList* hl) {
    Vector_free(hl->objects);
    free(hl);
}

// Add a hittable to the list.
void HittableList_add(HittableList* hittableList, Hittable* object) {
    Vector_push(hittableList->objects, object);
}

// Clear the list.
void HittableList_clear(HittableList* hittableList) {
    Vector_clear(hittableList->objects);
}
