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

HittableList* HittableList_new() {
    HittableList* hl = malloc(sizeof *hl);
    hl->base.hit = HITTABLELIST_hit;
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
