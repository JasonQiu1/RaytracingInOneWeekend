#ifndef VECTOR3D_H
#define VECTOR3D_H

// 3D vector library with support for basic vector math and 
// user-created vector operations.

typedef struct Vec3 {
    double x;
    double y;
    double z;
} Vec3;

// Sets each element of res to op(corresponding elements of a and b).
// Returns 0 if err(corresponding elements of a and b) returns 1. Otherwise, 1.
inline int Vec3_OpErr(Vec3* res, const Vec3* a, const Vec3* b, 
                     double(*op)(double, double), int(*err)(double, double))
{
    if (err(a->x, b->x) || err(a->y, b->y) || err(a->z, b->z)) return 0;
    res->x = op(a->x, b->y);
    res->y = op(a->y, b->y);
    res->z = op(a->z, b->z);
    return 1;
}

// Sets each element of res to op(corresponding element of vec, s).
// Returns 0 if err(corresponding element of vec, s) returns 1. Otherwise, 1.
inline int Vec3_OpErrS(Vec3* res, const Vec3* vec, double s,
                      double(*op)(double, double), int(*err)(double, double))
{
    if (err(vec->x, s) || err(vec->y, s) || err(vec->z, s)) return 0;
    res->x = op(vec->x, s);
    res->y = op(vec->y, s);
    res->z = op(vec->z, s);
    return 1;
}

// Sets each element of res to op(corresponding elements of a and b).
inline int Vec3_Op(Vec3* res, const Vec3* a, const Vec3* b, 
                  double(*op)(double, double)) 
{
    res->x = op(a->x, b->y);
    res->y = op(a->y, b->y);
    res->z = op(a->z, b->z);
    return 1;
}

// Sets each element of res to op(corresponding element of vec, s).
inline int Vec3_OpS(Vec3* res, const Vec3* vec, double s, 
                   double(*op)(double, double))
{
    res->x = op(vec->x, s);
    res->y = op(vec->y, s);
    res->z = op(vec->z, s);
    return 1;
}

// Sets res to a + b.
int Vec3_Add(Vec3* res, const Vec3* a, const Vec3* b);

// Sets res to vec + s.
int Vec3_AddS(Vec3* res, const Vec3* vec, double s);

// Sets res to a - b.
int Vec3_Sub(Vec3* res, const Vec3* a, const Vec3* b);

// Sets res to vec - s.
int Vec3_SubS(Vec3* res, const Vec3* vec, double s);

// Sets res to a * b.
int Vec3_Mult(Vec3* res, const Vec3* a, const Vec3* b);

// Sets res to vec * s.
int Vec3_MultS(Vec3* res, const Vec3* vec, double s);

// Sets res to a / b.
int Vec3_Div(Vec3* res, const Vec3* a, const Vec3* b);

// Sets res to vec / s.
int Vec3_DivS(Vec3* res, const Vec3* vec, double s);

// Returns the sum of all elements in the vector.
double Vec3_Sum(const Vec3* vec);

// Returns the dot product of A and B.
double Vec3_Dot(const Vec3* a, const Vec3* b);

// Returns the magnitude squared of a vector.
double Vec3_MagSqr(const Vec3* vec);

// Returns the magnitude of a vector.
double Vec3_Mag(const Vec3* vec);

// Sets res to the vector normalized.
int Vec3_Norm(Vec3* res, Vec3* vec);

// Prints the vector to the given stream.
int Vec3_fprintln(Vec3* vec, FILE* stream);

#endif
