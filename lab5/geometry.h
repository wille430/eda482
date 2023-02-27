
#ifndef GEOMETRY_H
#define GEOMETRY_H

typedef union {
    struct
    {
        float x, y, z;
    };
    float ps[3];
} Vec3, *PVec3;

typedef float Matrix44[4][4];

float dot(float * vec1, float * vec2, int size);
void scale(int fac, PVec3 vec);
void matvec_mul(Matrix44 m, PVec3 vec, PVec3 retVec);
void matmul(Matrix44 A, Matrix44 B, Matrix44 ret);

void setProjectionMatrix(float angleOfView, float near, float far, Matrix44 M);
void multPointMatrix(const PVec3 in, PVec3 out, Matrix44 M);
void create_identity_matrix(Matrix44 M);

#endif