#ifndef HITTABLELIST_H
#define HITTABLELIST_H

#include "hittable.h"
#include "vector.h"

typedef struct HittableList {
    Hittable base;
    // stores Hittable*
    Vector* objects;
} HittableList;

// Returns a malloced hittable list.
HittableList* HittableList_new();

// Frees the malloced hittable list.
void HittableList_free(HittableList* hittableList);

// Add a hittable to the list.
void HittableList_add(HittableList* hittableList, Hittable* object);

// Clear the list.
void HittableList_clear(HittableList* hittableList);

#endif
