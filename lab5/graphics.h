
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "geometry.h"

typedef struct {
    Vec3 start;
    Vec3 end;
} LINE, *PLINE;

typedef struct {
    int width, height, depth;
    char hasLines;
    union {
        struct {
            int numPoints;
            Vec3 * points;
        };
        struct {
            int numLines;
            PLINE lines;
        };
    };
} SHAPE, *PSHAPE;

typedef struct {
    Vec3 pos;
    Vec3 dir;
    Matrix44 rotation;
    
} TRANSFORM, *PTRANSFORM;

typedef struct {
    SHAPE shape;
    TRANSFORM transform;
} OBJECT, *POBJECT;

typedef struct {
    TRANSFORM transform;
} CAMERA, *PCAMERA;

void init_graphics();
void draw_object(POBJECT obj);
void create_cube(POBJECT cube, int width, int height);

void rotate_object_x(POBJECT obj, float deg);
void rotate_object_y(POBJECT obj, float deg);
void rotate_object_z(POBJECT obj, float deg);

// camera functions

void move_camera(PVec3 dpos);
void set_camera_position(PVec3 pos);
// rot.x = rotate along x axis, rot.y = rotate along y axis etc.
void rotate_camera(PVec3 rot);

#endif
