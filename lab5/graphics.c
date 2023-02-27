
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
    float angleOfView = 90;
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

void draw_object(POBJECT obj)
{
    Matrix44 translateM;
    
    translateM[0][3] = obj->pos.x;
    translateM[1][3] = obj->pos.y;
    translateM[2][3] = obj->pos.z;
    
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

#define cubeVerticesSize (12*10)
static Vec3 cubeVertices[cubeVerticesSize];

// this is set to 1 after cubeVertices is filled
// with vertices for the first time
char cubeVerticesReady = 0;

void create_cube(POBJECT cube, int width, int height)
{
    int sideVerts = cubeVerticesSize / 12;
    
    cube->numPoints = 12 * sideVerts;
    cube->points = &cubeVertices;
    create_identity_matrix(&cube->rotation);
    
    // to take in consideration that coordinates start at 0
    width--;
    height--;
    
    cube->pos.x = -width/2;
    cube->pos.y = -height/2;
    cube->pos.z = -width;
    
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
    
    for (int x = 0; x < sideVerts; x++) {
        PVec3 p = &cubeVertices[pointsSize++];
        p->x = x * spacing;
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
    
    for (int z = 0; z < sideVerts; z++)
        cubeVertices[pointsSize++].z = z * spacing;
        
    cubeVerticesReady = 1;
}

// rotates x axis
void rotate_object(POBJECT obj, float deg)
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