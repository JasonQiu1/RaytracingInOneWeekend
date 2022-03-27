#include <math.h>
#include <stdio.h>

#include "vector3d.h"

// Sets each element of res to op(corresponding elements of a and b).
// Returns 0 if err(corresponding elements of a and b) returns 1. Otherwise, 1.
extern int Vec3_OpErr(Vec3* res, const Vec3* a, const Vec3* b, 
                     double(*op)(double, double), 
                     int(*err)(double, double));

// Sets each element of res to op(corresponding element of vec, s).
// Returns 0 if err(corresponding element of vec, s) returns 1. Otherwise, 1.
extern int Vec3_OpErrS(Vec3* res, const Vec3* vec, double s, 
                      double(*op)(double, double), 
                      int(*err)(double, double));

// Sets each element of res to op(corresponding elements of a and b).
extern int Vec3_Op(Vec3* res, const Vec3* a, const Vec3* b, 
                  double(*op)(double, double));

// Sets each element of res to op(corresponding element of vec, s).
extern int Vec3_OpS(Vec3* res, const Vec3* vec, double s, 
                   double(*op)(double, double));

double OP_add(double a, double b) {
    return a + b;
}

// Sets res to a + b.
int Vec3_Add(Vec3* res, const Vec3* a, const Vec3* b) {
    Vec3_Op(res, a, b, OP_add);
    return 1;
}

// Sets res to vec + s.
int Vec3_AddS(Vec3* res, const Vec3* vec, double s) {
    Vec3_OpS(res, vec, s, OP_add);
    return 1;
}

static inline double OP_sub(double a, double b) {
    return a - b;
}

// Sets res to a - b.
int Vec3_Sub(Vec3* res, const Vec3* a, const Vec3* b) {
    Vec3_Op(res, a, b, OP_sub);
    return 1;
}

// Sets res to vec - s.
int Vec3_SubS(Vec3* res, const Vec3* vec, double s) {
    Vec3_OpS(res, vec, s, OP_sub);
    return 1;
}

static inline double OP_mult(double a, double b) {
    return a * b;
}

// Sets res to a * b.
int Vec3_Mult(Vec3* res, const Vec3* a, const Vec3* b) {
    Vec3_Op(res, a, b, OP_mult);
    return 1;
}

// Sets res to vec * s.
int Vec3_MultS(Vec3* res, const Vec3* vec, double s) {
    Vec3_OpS(res, vec, s, OP_mult);
    return 1;
}

static inline double OP_div(double a, double b) {
    return a / b;
}

// Returns 1 if b is 0, otherwise 0.
static inline int ERR_bIsZero(double a, double b) {
    return b == 0;
}

// Sets res to a / b.
int Vec3_Div(Vec3* res, const Vec3* a, const Vec3* b) {
    return Vec3_OpErr(res, a, b, OP_div, ERR_bIsZero);
}

// Sets res to vec / s.
int Vec3_DivS(Vec3* res, const Vec3* vec, double s) {
    return Vec3_OpErrS(res, vec, s, OP_div, ERR_bIsZero);
}

// Returns the sum of all elements in the vector.
double Vec3_Sum(const Vec3* vec) {
    return vec->x + vec->y + vec->z;
}

// Returns the dot product of A and B.
double Vec3_Dot(const Vec3* a, const Vec3* b) {
    return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}

// Returns the magnitude squared of a vector.
double Vec3_MagSqr(const Vec3* vec) {
    return Vec3_Dot(vec, vec);
}

// Returns the magnitude of a vector.
double Vec3_Mag(const Vec3* vec) {
    return sqrt(Vec3_MagSqr(vec));
}

// Sets res to the vector normalized.
int Vec3_Norm(Vec3* res, Vec3* vec) {
    return Vec3_DivS(res, vec, Vec3_Mag(vec));
}

// Prints the vector to the given stream.
int Vec3_fprintln(Vec3* vec, FILE* stream) {
    return fprintf(stream, "%lf %lf %lf\n", vec->x, vec->y, vec->z); 
}
