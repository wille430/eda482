/*
 * 	startup.c
 *
 */
 
#include "geometry.h"
#include "graphics.h"
#include "vertexdata.h"
#include "delay.h"
#include "registers.h"

__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

#define GPIO_D (*((volatile PGPIO) (0x40020C00)))

#define CAMERA_SPEED 1
#define CAMERA_ZOOM_STEP 0.10

#define MAX_CUBE_COUNT 64
#define CUBE_WIDTH 1
#define CUBE_HEIGHT 1

static OBJECT cubes[MAX_CUBE_COUNT];
static int cubesSize = 0;

static Vec3 cameraFocusOffset = {0,0.5,-2.5};

static OBJECT teapot = {
    { // shape
        10, 10, 10,
        numVertices,
        &vertices    
    },
    { // transform
        {0,0,0},
        {0,0,0},
        {{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1}}
    }
};

void show_rotating_cube(void)
{
    OBJECT cube;
    create_cube(&cube, 10, 10);

    Vec3 cam_pos = {0,0,0};
    set_camera_position(&cam_pos);

    while (1)
    {
        draw_object(&cube);
        rotate_object_x(&cube, -30.0);
        rotate_object_z(&cube, 30.0);
        delay_milli(1);
        graphic_clear_screen();
    }
}

void show_rotating_teapot()
{
    static Vec3 camPos = {0,7,-30}; 
    set_camera_position(&camPos);
    while (1)
    {
        draw_object(&teapot);
        rotate_object_y(&teapot, 30.0);
        delay_milli(1);
        graphic_clear_screen();
    }
}

POBJECT place_cube(POBJECT cube, PVec3 pos) {
    create_cube(cube, CUBE_WIDTH, CUBE_HEIGHT);

    // move to specifed position
    cube->transform.pos.x = pos->x;
    cube->transform.pos.y = pos->y;
    cube->transform.pos.z = pos->z;
}

void handle_key_input(void)
{
    Vec3 dpos = {0,0,0};
    Vec3 rot = {0,0,0};
    Vec3 cubePos = {0,0,0};

    if (cubesSize)
    {
        memcpy(&cubePos, &cubes[cubesSize-1].transform.pos, sizeof(cubePos) / sizeof(cubePos.x));
    }
    
    switch(keyb())
    {
        // build left
        case 0x4:
            if (cubesSize) cubePos.x--;
            place_cube(&cubes[cubesSize++], &cubePos);
            break;
        // build right
        case 0x6:
            // place cube right of the last added cube
            if (cubesSize) cubePos.x++;
            place_cube(&cubes[cubesSize++], &cubePos);
            break;
        // build up
        case 0x2:
            if (cubesSize) cubePos.y++;
            place_cube(&cubes[cubesSize++], &cubePos);
            break;
        // build down
        case 0x8:
            if (cubesSize) cubePos.y--;
            place_cube(&cubes[cubesSize++], &cubePos);
            break;
        // zoom out
        case 0xA:
            cameraFocusOffset.z *= 1.0 + CAMERA_ZOOM_STEP;
            break;
        // zoom in
        case 0xB:
            cameraFocusOffset.z /= 1.0 + CAMERA_ZOOM_STEP;
            break;
    }
}

void game_loop(void)
{
    while (1)
    {

        // follow the last cube with camera
        if (cubesSize) {
            Vec3 camPos;
            add_vec(&cameraFocusOffset, &cubes[cubesSize-1].transform.pos, &camPos);
            set_camera_position(&camPos);
        } else {
            set_camera_position(&cameraFocusOffset);
        }

        for (int i = 0; i < cubesSize; i++)
        {
            draw_object(&cubes[i]);
        }
        
        handle_key_input();
        
        delay_milli(1);
        graphic_clear_screen();       
    }
}

// TODO
// [x] flytta camera med tangentbord
// [ ] uppdatera frames med avbrott (60hz?)

void main(void)
{
    init_graphics();
    init_keypad(&GPIO_D);
    // show_rotating_cube();
    // show_rotating_teapot();
    
    game_loop();
}
