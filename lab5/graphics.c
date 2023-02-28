
#include "display.h"
#include "geometry.h"
#include "math.h"
#include "vertexdata.h"
#include "graphics.h"
#include "utils.h"

static Matrix44 Mproj;
static Matrix44 worldToCamera;

void init_graphics()
{
    float angleOfView = 45;
    float near = 0.1;
    float far = 100;
    
    graphic_initialize();
    graphic_clear_screen();
    
    worldToCamera[0][0] = 1;
    worldToCamera[1][1] = 1;
    worldToCamera[2][2] = 1;
    worldToCamera[3][3] = 1;
    worldToCamera[0][3] = 0; // x+t
    worldToCamera[1][3] = -7; // y+t
    worldToCamera[2][3] = -15; // z+t

    setProjectionMatrix(angleOfView, near, far, Mproj);
}

void create_translate_matrix(PVec3 pos, Matrix44 out)
{
    create_identity_matrix(out);
    
    out[0][3] = pos->x;
    out[1][3] = pos->y;
    out[2][3] = pos->z;
}

void draw_object(POBJECT obj)
{
    Matrix44 translateM;
    create_translate_matrix(&obj->pos, translateM);
    
    for (int i = 0; i < obj->numPoints; i++)
    {
        Vec3 rotatedVert, translatedVert, vertCamera, projectedVert;
        
        // rotate
        multPointMatrix(&obj->points[i], &rotatedVert, obj->rotation);
        
        // translate
        multPointMatrix(&rotatedVert, &translatedVert, translateM);
        multPointMatrix(&translatedVert, &vertCamera, worldToCamera);
        
        // project to screen
        multPointMatrix(&vertCamera, &projectedVert, Mproj);
        
        if (projectedVert.x < -1 || projectedVert.x > 1 || projectedVert.y < -1 || projectedVert.y > 1) continue;
        
        int x = min(SCREEN_WIDTH - 1, (int)((projectedVert.x + 1) * 0.5 * SCREEN_WIDTH)); 
        int y = min(SCREEN_HEIGHT - 1, (int)((1 - (projectedVert.y + 1) * 0.5) * SCREEN_HEIGHT));
        
        // plot
        graphic_pixel_set(SCREEN_WIDTH - x, SCREEN_HEIGHT - y);
    }
}


// moves all verteces so that the object is placed in the middle
void move_to_middle(POBJECT obj)
{
    Matrix44 translateM;
    Vec3 move = { -obj->width/2, -obj->height/2, -obj->depth/2 };
    create_translate_matrix(&move, translateM);
    
    for (int i = 0; i < obj->numPoints; i++)
    {
        Vec3 out;
        matvec_mul(translateM, &obj->points[i], &out);
        obj->points[i].x = out.x;
        obj->points[i].y = out.y;
        obj->points[i].z = out.z;
    }
}


#define cubeVerticesSize (12*5)
static Vec3 cubeVertices[cubeVerticesSize];

// this is set to 1 after cubeVertices is filled
// with vertices for the first time
char cubeVerticesReady = 0;

void create_cube(POBJECT cube, int width, int height)
{
    int sideVerts = cubeVerticesSize / 12;
    
    cube->numPoints = 12 * sideVerts;
    cube->points = &cubeVertices;
    cube->width = width;
    cube->height = height;
    cube->depth = width;
    
    create_identity_matrix(&cube->rotation);
    
    // to take in consideration that coordinates start at 0
    width--;
    height--;
    
    if (cubeVerticesReady) return;
    
    float spacing = (float) width / sideVerts;
    int pointsSize = 0;
    for (int x = 0; x < sideVerts; x++)
        cubeVertices[pointsSize++].x = x * spacing;
        
    for (int x = 0; x < sideVerts; x++) {
        PVec3 p = &cubeVertices[pointsSize++];
        p->x = x * spacing;
        p->y = height;
    }
    
    for (int x = 0; x < sideVerts; x++) {
        PVec3 p = &cubeVertices[pointsSize++];
        p->x = x * spacing;
        p->z = width;
    }
    
    for (int x = 0; x < sideVerts; x++) {
        PVec3 p = &cubeVertices[pointsSize++];
        p->x = x * spacing;
        p->y = height;
        p->z = width;
    }
    
    for (int y = 0; y < sideVerts; y++)
        cubeVertices[pointsSize++].y = y * spacing;
        
    for (int y = 0; y < sideVerts; y++) {
        PVec3 p = &cubeVertices[pointsSize++];
        p->x = width;
        p->y = y * spacing;
    }
    
    for (int y = 0; y < sideVerts; y++) {
        PVec3 p = &cubeVertices[pointsSize++];
        p->y = y * spacing;
        p->z = width;
    }
    
    for (int y = 0; y < sideVerts; y++) {
        PVec3 p = &cubeVertices[pointsSize++];
        p->x = width;
        p->y = y * spacing;
        p->z = width;
    }
        
    for (int z = 0; z < sideVerts; z++) {
        PVec3 p = &cubeVertices[pointsSize++];
        p->z = z * spacing;
        p->y = height;
    }
    
    for (int z = 0; z < sideVerts; z++) {
        PVec3 p = &cubeVertices[pointsSize++];
        p->x = width;
        p->z = z * spacing;
    }
    
   for (int z = 0; z < sideVerts; z++) {
        PVec3 p = &cubeVertices[pointsSize++];
        p->x = width;
        p->y = height;
        p->z = z * spacing;
    }
    
    for (int z = 0; z < sideVerts; z++)
        cubeVertices[pointsSize++].z = z * spacing;
    
    move_to_middle(cube);
    cubeVerticesReady = 1;
}

// rotates x axis
void rotate_object_x(POBJECT obj, float deg)
{
    Matrix44 R;
    
    create_identity_matrix(R);
    
    R[1][1] = cos(deg * M_PI / 180);
    R[1][2] = -sin(deg * M_PI / 180);
    R[2][1] = sin(deg * M_PI / 180);
    R[2][2] = cos(deg * M_PI / 180);
    
    Matrix44 ret;
    matmul(R, obj->rotation, ret);
    memcpy(obj->rotation, ret, 16);
}

void rotate_object_y(POBJECT obj, float deg)
{
    Matrix44 R;
    
    create_identity_matrix(R);
    
    R[0][0] = cos(deg * M_PI / 180);
    R[0][2] = sin(deg * M_PI / 180);
    R[2][0] = -sin(deg * M_PI / 180);
    R[2][2] = cos(deg * M_PI / 180);
    
    Matrix44 ret;
    matmul(R, obj->rotation, ret);
    memcpy(obj->rotation, ret, 16);
}

void rotate_object_z(POBJECT obj, float deg)
{
    Matrix44 R;
    
    create_identity_matrix(R);
    
    R[0][0] = cos(deg * M_PI / 180);
    R[0][1] = -sin(deg * M_PI / 180);
    R[1][0] = sin(deg * M_PI / 180);
    R[1][1] = cos(deg * M_PI / 180);
    
    Matrix44 ret;
    matmul(R, obj->rotation, ret);
    memcpy(obj->rotation, ret, 16);
}