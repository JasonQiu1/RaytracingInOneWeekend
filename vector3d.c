#include <math.h>
#include <stdio.h>

#include "vector3d.h"

// Sets each element of res to op(corresponding elements of a and b).
// Returns 0 if err(corresponding elements of a and b) returns 1. Otherwise, 1.
extern Vec3* Vec3_opErr(Vec3* res, const Vec3* a, const Vec3* b, 
                      double(*op)(double, double), int(*err)(double, double));

// Sets each element of res to op(corresponding element of vec, s).
// Returns 0 if err(corresponding element of vec, s) returns 1. Otherwise, 1.
extern Vec3* Vec3_opErrS(Vec3* res, const Vec3* vec, double s, 
                       double(*op)(double, double), int(*err)(double, double));

// Sets each element of res to op(corresponding elements of a and b).
extern Vec3* Vec3_op(Vec3* res, const Vec3* a, const Vec3* b, 
                   double(*op)(double, double));

// Sets each element of res to op(corresponding element of vec, s).
extern Vec3* Vec3_opS(Vec3* res, const Vec3* vec, double s, 
                    double(*op)(double, double));

static inline double OP_add(double a, double b) {
    return a + b;
}

// Sets res to a + b.
Vec3* Vec3_add(Vec3* res, const Vec3* a, const Vec3* b) {
    return Vec3_op(res, a, b, OP_add);
}

// Sets res to vec + s.
Vec3* Vec3_addS(Vec3* res, const Vec3* vec, double s) {
    return Vec3_opS(res, vec, s, OP_add);
}

static inline double OP_sub(double a, double b) {
    return a - b;
}

// Sets res to a - b.
Vec3* Vec3_sub(Vec3* res, const Vec3* a, const Vec3* b) {
    return Vec3_op(res, a, b, OP_sub);
}

// Sets res to vec - s.
Vec3* Vec3_subS(Vec3* res, const Vec3* vec, double s) {
    return Vec3_opS(res, vec, s, OP_sub);
}

static inline double OP_mult(double a, double b) {
    return a * b;
}

// Sets res to a * b.
Vec3* Vec3_mult(Vec3* res, const Vec3* a, const Vec3* b) {
    return Vec3_op(res, a, b, OP_mult);
}

// Sets res to vec * s.
Vec3* Vec3_multS(Vec3* res, const Vec3* vec, double s) {
    return Vec3_opS(res, vec, s, OP_mult);
}

static inline double OP_div(double a, double b) {
    return a / b;
}

// Returns 1 if b is 0, otherwise 0.
static inline int ERR_bIsZero(double a, double b) {
    return b == 0;
}

// Sets res to a / b.
Vec3* Vec3_div(Vec3* res, const Vec3* a, const Vec3* b) {
    return Vec3_opErr(res, a, b, OP_div, ERR_bIsZero);
}

// Sets res to vec / s.
Vec3* Vec3_divS(Vec3* res, const Vec3* vec, double s) {
    return Vec3_opErrS(res, vec, s, OP_div, ERR_bIsZero);
}

// Returns the sum of all elements in the vector.
double Vec3_sum(const Vec3* vec) {
    return vec->x + vec->y + vec->z;
}

// Returns the dot product of A and B.
double Vec3_dot(const Vec3* a, const Vec3* b) {
    return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}

// Returns the magnitude squared of a vector.
double Vec3_magSqr(const Vec3* vec) {
    return Vec3_dot(vec, vec);
}

// Returns the magnitude of a vector.
double Vec3_mag(const Vec3* vec) {
    return sqrt(Vec3_magSqr(vec));
}

// Sets res to the vector normalized.
Vec3* Vec3_norm(Vec3* res, const Vec3* vec) {
    return Vec3_divS(res, vec, Vec3_mag(vec));
}

// Prints the vector to the given stream.
int Vec3_fprintln(const Vec3* vec, FILE* stream) {
    return fprintf(stream, "%lf %lf %lf\n", vec->x, vec->y, vec->z); 
}
