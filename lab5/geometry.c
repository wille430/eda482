
#include "geometry.h"
#include "math.h"
#include "display.h"

float dot(float * vec1, float * vec2, int size)
{
    float sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += vec1[i] * vec2[i];
    }
    return sum;
}

void scale(int fac, PVec3 vec)
{
    vec->x *= fac;
    vec->y *= fac;
    vec->z *= fac;
}

void matvec_mul(Matrix44 m, PVec3 vec, PVec3 out)
{
    out->x = vec->x * m[0][0] + vec->y * m[0][1] + vec->z * m[0][2] + m[0][3];
    out->y = vec->x * m[1][0] + vec->y * m[1][1] + vec->z * m[1][2] + m[1][3];
    out->z = vec->x * m[2][0] + vec->y * m[2][1] + vec->z * m[2][2] + m[2][3];
    
    float w = vec->x * m[3][0] + vec->y * m[3][1] + vec->z * m[3][2] + m[3][3];
 
    // normalize if w is different than 1 (convert from homogeneous to Cartesian coordinates)
    if (w != 1) { 
        out->x /= w; 
        out->y /= w; 
        out->z /= w; 
    } 
}

void matmul(Matrix44 A, Matrix44 B, Matrix44 ret)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            float row[] = { A[i][0], A[i][1], A[i][2], A[i][3] };
            float col[] = { B[0][j], B[1][j], B[2][j], B[3][j] };
            ret[i][j] = dot(row, col, 4);
        }
    }
}


void setProjectionMatrix(float angleOfView, float near, float far, Matrix44 M)
{
    float t = tan(angleOfView * M_PI / 180 / 2);
    float left, right, bottom, top;
    right = near * SCREEN_WIDTH / SCREEN_HEIGHT * t;
    bottom = near * t;
    left = -right;
    top = -bottom;
     
     M[0][0] = 1 / (SCREEN_WIDTH / SCREEN_HEIGHT * t);
     M[1][1] = 1/ t;
     M[2][2] = far / (far - near);
     M[2][3] = -far * near / (far - near);
     M[3][2] = 1;
}

void multPointMatrix(const PVec3 in, PVec3 out, Matrix44 M) 
{ 
    //out = in * M;
    out->x   = in->x * M[0][0] + in->y * M[0][1] + in->z * M[0][2] + /* in.z = 1 */ M[0][3]; 
    out->y   = in->x * M[1][0] + in->y * M[1][1] + in->z * M[1][2] + /* in.z = 1 */ M[1][3]; 
    out->z   = in->x * M[2][0] + in->y * M[2][1] + in->z * M[2][2] + /* in.z = 1 */ M[2][3]; 
    float w = in->x * M[3][0] + in->y * M[3][1] + in->z * M[3][2] + /* in.z = 1 */ M[3][3]; 
 
    // normalize if w is different than 1 (convert from homogeneous to Cartesian coordinates)
    if (w != 1) { 
        out->x /= w; 
        out->y /= w; 
        out->z /= w; 
    } 
}

void create_identity_matrix(Matrix44 M)
{
    memset(&M[0], 0x0, 16);
    M[0][0] = 1;
    M[1][1] = 1;
    M[2][2] = 1;
    M[3][3] = 1;
}


void add_vec(PVec3 v1, PVec3 v2, PVec3 out)
{
    out->x = v1->x + v2->x;
    out->y = v1->y + v2->y;
    out->z = v1->z + v2->z;
}