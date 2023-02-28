
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "geometry.h"

typedef struct {
    Vec3 pos;
    Vec3 dir;
    int width, height, depth;
    Matrix44 rotation;
    int numPoints;
    Vec3 * points;
} OBJECT, *POBJECT;

void init_graphics();
void draw_object(POBJECT obj);
void create_cube(POBJECT cube, int width, int height);

void rotate_object_x(POBJECT obj, float deg);
void rotate_object_y(POBJECT obj, float deg);
void rotate_object_z(POBJECT obj, float deg);

#endif