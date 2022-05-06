#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <stdio.h>

// 3D vector library with support for basic vector math and 
// user-created vector operations.

typedef struct Vec3 {
    double x;
    double y;
    double z;
} Vec3;

// Sets each element of res to op(corresponding elements of a and b).
// Returns NULL if err(corresponding elements of a and b) returns 1.
// Otherwise, returns res.
inline Vec3* Vec3_opErr(Vec3* res, const Vec3* a, const Vec3* b, 
                      double(*op)(double, double), int(*err)(double, double))
{
    if (err(a->x, b->x) || err(a->y, b->y) || err(a->z, b->z)) return NULL;
    res->x = op(a->x, b->x);
    res->y = op(a->y, b->y);
    res->z = op(a->z, b->z);
    return res;
}

// Sets each element of res to op(corresponding element of vec, s).
// Returns NULL if err(corresponding element of vec, s) returns 1.
// Otherwise, returns res;
inline Vec3* Vec3_opErrS(Vec3* res, const Vec3* vec, double s,
                       double(*op)(double, double), int(*err)(double, double))
{
    if (err(vec->x, s) || err(vec->y, s) || err(vec->z, s)) return NULL;
    res->x = op(vec->x, s);
    res->y = op(vec->y, s);
    res->z = op(vec->z, s);
    return res;
}

// Sets each element of res to op(corresponding elements of a and b).
inline Vec3* Vec3_op(Vec3* res, const Vec3* a, const Vec3* b, 
                   double(*op)(double, double)) 
{
    res->x = op(a->x, b->x);
    res->y = op(a->y, b->y);
    res->z = op(a->z, b->z);
    return res;
}

// Sets each element of res to op(corresponding element of vec, s).
inline Vec3* Vec3_opS(Vec3* res, const Vec3* vec, double s, 
                    double(*op)(double, double))
{
    res->x = op(vec->x, s);
    res->y = op(vec->y, s);
    res->z = op(vec->z, s);
    return res;
}

// Sets res to a + b.
Vec3* Vec3_add(Vec3* res, const Vec3* a, const Vec3* b);

// Sets res to vec + s.
Vec3* Vec3_addS(Vec3* res, const Vec3* vec, double s);

// Sets res to a - b.
Vec3* Vec3_sub(Vec3* res, const Vec3* a, const Vec3* b);

// Sets res to vec - s.
Vec3* Vec3_subS(Vec3* res, const Vec3* vec, double s);

// Sets res to a * b.
Vec3* Vec3_mult(Vec3* res, const Vec3* a, const Vec3* b);

// Sets res to vec * s.
Vec3* Vec3_multS(Vec3* res, const Vec3* vec, double s);

// Sets res to a / b.
Vec3* Vec3_div(Vec3* res, const Vec3* a, const Vec3* b);

// Sets res to vec / s.
Vec3* Vec3_divS(Vec3* res, const Vec3* vec, double s);

// Sets res to min(x, y) for each element of a and b.
Vec3* Vec3_lt(Vec3* res, const Vec3* a, const Vec3* b);

// Sets res to max(x, y) for each element of a and b.
Vec3* Vec3_gt(Vec3* res, const Vec3* a, const Vec3* b);

// Returns the sum of all elements in the vector.
double Vec3_sum(const Vec3* vec);

// Returns the dot product of A and B.
double Vec3_dot(const Vec3* a, const Vec3* b);

// Returns the magnitude squared of a vector.
double Vec3_magSqr(const Vec3* vec);

// Returns the magnitude of a vector.
double Vec3_mag(const Vec3* vec);

// Returns the corresponding index of the Vec3 object.
// Aborts if index is negative or above 2.
double Vec3_at(const Vec3* vec, size_t idx);

// Sets res to the vector normalized.
Vec3* Vec3_norm(Vec3* res, const Vec3* vec);

// Sets res to the cross product;
Vec3* Vec3_cross(Vec3* res, const Vec3* a, const Vec3* b);

// Prints the vector to the given stream.
int Vec3_fprintln(const Vec3* vec, FILE* stream);

#endif
