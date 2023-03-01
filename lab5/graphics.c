
#include "display.h"
#include "geometry.h"
#include "math.h"
#include "vertexdata.h"
#include "graphics.h"
#include "utils.h"

static Matrix44 Mproj;
static Matrix44 worldToCamera;

static CAMERA camera;

void init_camera()
{
    create_identity_matrix(worldToCamera);
    create_identity_matrix(&camera.transform.rotation);
}

void init_graphics()
{
    float angleOfView = 45;
    float near = 0.1;
    float far = 100;
    
    graphic_initialize();
    graphic_clear_screen();
    
    init_camera();

    setProjectionMatrix(angleOfView, near, far, Mproj);
}

void move_camera(PVec3 dpos)
{
    add_vec(&camera.transform.pos, dpos, &camera.transform.pos);
}

void rotate_camera(PVec3 rot)
{
    if (rot->x) rotate_object_x(&camera, rot->x);
    if (rot->y) rotate_object_y(&camera, rot->y);
    if (rot->z) rotate_object_z(&camera, rot->z);
}

void set_camera_position(PVec3 pos)
{
    camera.transform.pos.x = pos->x;
    camera.transform.pos.y = pos->y;
    camera.transform.pos.z = pos->z;
}

void create_translate_matrix(PVec3 pos, Matrix44 out)
{
    create_identity_matrix(out);
    
    out[0][3] = pos->x;
    out[1][3] = pos->y;
    out[2][3] = pos->z;
}

void update_world_to_cam_transform_matrix()
{
    create_identity_matrix(worldToCamera);
    // apply camera rotation
    Matrix44 res;
    matmul(worldToCamera, camera.transform.rotation, res);
    memcpy(worldToCamera, res, 16);

    // if camera is 10 units to the right, then the world should move 10 units to the left
    worldToCamera[0][3] = -camera.transform.pos.x; // x+t
    worldToCamera[1][3] = -camera.transform.pos.y; // y+t
    worldToCamera[2][3] = -camera.transform.pos.z; // z+t
}

int is_valid_point(int x, int y)
{
    return x >= 0 && x < SCREEN_WIDTH &&
           y >= 0 && y < SCREEN_HEIGHT;
}

int draw_line(PLINE l)
{
    int x0, y0, x1, y1;
    x0 = l->start.x;
    y0 = l->start.y;
    x1 = l->end.x;
    y1 = l->end.y;
    
    signed int dx, dy, sx, sy;
    dx = abs(x1 - x0);
    sx = x0 < x1 ? 1 : -1;
    dy = -abs(y1 - y0);
    sy = y0 < y1 ? 1 : -1;
    
    int err = dx + dy;
    int e2;
    
    while (1)
    {
        graphic_pixel_set(x0, y0);
        if ((x0 == x1 && y0 == y1) ||
            !is_valid_point(x0, y0)) break;
        e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
    
    return 1;
}


void draw_object_lines(POBJECT obj)
{
    update_world_to_cam_transform_matrix();
    
    Matrix44 translateM;
    create_translate_matrix(&obj->transform.pos, translateM);
    
    for (int i = 0; i < obj->shape.numLines; i++)
    {
        Vec3 rotatedVert1, translatedVert1, vertCamera1, projectedVert1;
        Vec3 rotatedVert2, translatedVert2, vertCamera2, projectedVert2;
        
        // apply transform rotation
        multPointMatrix(&obj->shape.lines[i].start, &rotatedVert1, obj->transform.rotation);
        multPointMatrix(&obj->shape.lines[i].end, &rotatedVert2, obj->transform.rotation);
        
        // translate
        multPointMatrix(&rotatedVert1, &translatedVert1, translateM);
        multPointMatrix(&rotatedVert2, &translatedVert2, translateM);

        multPointMatrix(&translatedVert1, &vertCamera1, worldToCamera);
        multPointMatrix(&translatedVert2, &vertCamera2, worldToCamera);
        
        // points behind the camera should not be displayed
        if (translatedVert1.z < camera.transform.pos.z && 
            translatedVert2.z < camera.transform.pos.z) continue;

        // project to screen
        multPointMatrix(&vertCamera1, &projectedVert1, Mproj);
        multPointMatrix(&vertCamera2, &projectedVert2, Mproj);
        
        if ((projectedVert1.x < -1 || projectedVert1.x > 1 || projectedVert1.y < -1 || projectedVert1.y > 1) &&
            (projectedVert2.x < -1 || projectedVert2.x > 1 || projectedVert2.y < -1 || projectedVert2.y > 1)) continue;
        
        int x1 = min(SCREEN_WIDTH - 1, (int)((projectedVert1.x + 1) * 0.5 * SCREEN_WIDTH)); 
        int y1 = min(SCREEN_HEIGHT - 1, (int)((1 - (projectedVert1.y + 1) * 0.5) * SCREEN_HEIGHT));

        int x2 = min(SCREEN_WIDTH - 1, (int)((projectedVert2.x + 1) * 0.5 * SCREEN_WIDTH)); 
        int y2 = min(SCREEN_HEIGHT - 1, (int)((1 - (projectedVert2.y + 1) * 0.5) * SCREEN_HEIGHT));

        LINE line = {
            {x1,y1,0},
            {x2,y2,0}
        };
        
        // plot
        draw_line(&line);
    }
}

void draw_object(POBJECT obj)
{
    if (obj->shape.hasLines)
    {
        draw_object_lines(obj);
        return;
    }

    update_world_to_cam_transform_matrix();
    
    Matrix44 translateM;
    create_translate_matrix(&obj->transform.pos, translateM);
    
    for (int i = 0; i < obj->shape.numPoints; i++)
    {
        Vec3 rotatedVert, translatedVert, vertCamera, projectedVert;
        
        // apply transform rotation
        multPointMatrix(&obj->shape.points[i], &rotatedVert, obj->transform.rotation);
        
        // translate
        multPointMatrix(&rotatedVert, &translatedVert, translateM);
        multPointMatrix(&translatedVert, &vertCamera, worldToCamera);
        
        // points behind the camera should not be displayed
        if (translatedVert.z < camera.transform.pos.z) continue;

        // project to screen
        multPointMatrix(&vertCamera, &projectedVert, Mproj);
        
        if (projectedVert.x < -1 || projectedVert.x > 1 || projectedVert.y < -1 || projectedVert.y > 1) continue;
        
        int x = min(SCREEN_WIDTH - 1, (int)((projectedVert.x + 1) * 0.5 * SCREEN_WIDTH)); 
        int y = min(SCREEN_HEIGHT - 1, (int)((1 - (projectedVert.y + 1) * 0.5) * SCREEN_HEIGHT));
        
        // plot
        graphic_pixel_set(x, y);
    }
}


// moves all verteces so that the object is placed in the middle
void move_to_middle(POBJECT obj)
{
    Matrix44 translateM;
    Vec3 move = { -obj->shape.width/2.0, -obj->shape.height/2.0, -obj->shape.depth/2.0 };
    create_translate_matrix(&move, translateM);
    
    for (int i = 0; i < obj->shape.numPoints; i++)
    {
        Vec3 out;
        matvec_mul(translateM, &obj->shape.points[i], &out);
        obj->shape.points[i].x = out.x;
        obj->shape.points[i].y = out.y;
        obj->shape.points[i].z = out.z;
    }
}


#define cubeVerticesSize (12*5)
static Vec3 cubeVertices[cubeVerticesSize];
// this is set to 1 after cubeVertices is filled
// with vertices for the first time
char cubeVerticesReady = 0;

static LINE cubeLines[12] = {
    {{0,0,0}, {0,0,1}},
    {{0,0,1}, {1,0,1}},
    {{1,0,1}, {1,0,0}},
    {{1,0,0}, {0,0,0}},
    {{0,0,0}, {0,1,0}},
    {{0,1,0}, {0,1,1}},
    {{0,1,1}, {1,1,1}},
    {{1,1,1}, {1,1,0}},
    {{1,1,0}, {0,1,0}},
    {{1,0,0}, {1,1,0}},
    {{1,0,1}, {1,1,1}},
    {{0,0,1}, {0,1,1}}
};

void create_cube_lines(POBJECT cube, int width, int height)
{
    cube->shape.hasLines = 1;

    cube->shape.numLines = 12;
    cube->shape.lines = &cubeLines;
    cube->shape.width = width;
    cube->shape.height = height;
    cube->shape.depth = width;

    cube->transform.pos.x = 0;
    cube->transform.pos.y = 0;
    cube->transform.pos.z = 0;

    cube->transform.dir.x = 0;
    cube->transform.dir.y = 0;
    cube->transform.dir.z = 0;

    create_identity_matrix(&cube->transform.rotation);
}

void create_cube(POBJECT cube, int width, int height)
{
    int sideVerts = cubeVerticesSize / 12;
    
    cube->shape.numPoints = 12 * sideVerts;
    cube->shape.points = &cubeVertices;
    cube->shape.width = width;
    cube->shape.height = height;
    cube->shape.depth = width;

    cube->transform.pos.x = 0;
    cube->transform.pos.y = 0;
    cube->transform.pos.z = 0;

    cube->transform.dir.x = 0;
    cube->transform.dir.y = 0;
    cube->transform.dir.z = 0;

    create_identity_matrix(&cube->transform.rotation);
    
    // to take in consideration that coordinates start at 0
    // width--;
    // height--;
    
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
    
    // move_to_middle(cube);
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
    matmul(R, obj->transform.rotation, ret);
    memcpy(obj->transform.rotation, ret, 16);
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
    matmul(R, obj->transform.rotation, ret);
    memcpy(obj->transform.rotation, ret, 16);
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
    matmul(R, obj->transform.rotation, ret);
    memcpy(obj->transform.rotation, ret, 16);
}
