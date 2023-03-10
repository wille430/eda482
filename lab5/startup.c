/*
 * 	startup.c
 *
 */
 
#include "geometry.h"
#include "graphics.h"
#include "vertexdata.h"
#include "delay.h"
#include "registers.h"
#include "asciidisp.h"

__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

#define GPIO_D (*((volatile PGPIO) (0x40020C00)))
#define STK ((volatile STK *) 0xE000E010)

#define CAMERA_SPEED 1
#define CAMERA_ZOOM_STEP 0.10
#define CAMERA_ROTAION_DEG 30

#define MAX_CUBE_COUNT 64
#define CUBE_WIDTH 1
#define CUBE_HEIGHT 1

typedef enum CAMERA_MODE {FOLLOW, FREE};
static int cameraMode = FOLLOW;

static OBJECT cubes[MAX_CUBE_COUNT];
static int cubesSize = 0;

static Vec3 cameraFocusOffset = {0.5,0.5,-2.5};

static OBJECT teapot = {
    { // shape
        10, 10, 10,
        0,
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
    create_cube_lines(&cube, 1, 1);
    set_camera_position(&cameraFocusOffset);

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
    create_cube_lines(cube, CUBE_WIDTH, CUBE_HEIGHT);

    // move to specifed position
    cube->transform.pos.x = pos->x;
    cube->transform.pos.y = pos->y;
    cube->transform.pos.z = pos->z;
}

void handle_key_input(void)
{
    Vec3 cubePos = {0,0,0};

    if (cubesSize)
    {
        memcpy(&cubePos, &cubes[cubesSize-1].transform.pos, sizeof(cubePos) / sizeof(cubePos.x));
    }
    
    char key = keyb();
    
    if (cameraMode == FOLLOW) 
    {
        switch (key)
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
            // build forward
            case 0x2:
                if (cubesSize) cubePos.z++;
                place_cube(&cubes[cubesSize++], &cubePos);
                break;
            // build back
            case 0x8:
                if (cubesSize) cubePos.z--;
                place_cube(&cubes[cubesSize++], &cubePos);
                break;
            // build up
            case 0x7:
                if (cubesSize) cubePos.y++;
                place_cube(&cubes[cubesSize++], &cubePos);
                break;
            // build down
            case 0xE:
                if (cubesSize) cubePos.y--;
                place_cube(&cubes[cubesSize++], &cubePos);
                break;
        }
    } else if (cameraMode == FREE) {
        Vec3 dpos = {0,0,0};
        Vec3 rot = {0,0,0};
        switch (key)
        {
            // rotate left
            case 0x1:
                rot.y = -CAMERA_ROTAION_DEG;
                rotate_camera(&rot);
                break;
            // rotate right
            case 0x3:
                rot.y = CAMERA_ROTAION_DEG;
                rotate_camera(&rot);
                break;
            // move left
            case 0x4:
                dpos.x = -CAMERA_SPEED;
                move_camera(&dpos);
                break;
            // move right
            case 0x6:
                dpos.x = CAMERA_SPEED;
                break;
            // move forward
            case 0x2:
                dpos.z = CAMERA_SPEED;
                break;
            // move back
            case 0x8:
                dpos.z = -CAMERA_SPEED;
                break;
            // move down
            case 0xE:
                dpos.y = -CAMERA_SPEED;
                break;
            // move up
            case 0x7:
                dpos.y = CAMERA_SPEED;
                break;
        }
        move_camera(&dpos);
    }
    
    
    static int key_d_down = 0;
    switch(key)
    {
        // zoom out
        case 0xA:
            cameraFocusOffset.z *= 1.0 + CAMERA_ZOOM_STEP;
            break;
        // zoom in
        case 0xB:
            cameraFocusOffset.z /= 1.0 + CAMERA_ZOOM_STEP;
            break;
        // change camera mode
        case 0xD:
            if (key_d_down) {
                break;
            }
            cameraMode = (cameraMode + 1) % 2; // change to the next camera mode
            break;
    }
    
    key_d_down = key == 0xD;
}

void camera_focus_cube(POBJECT cube)
{
    if (cube == 0) {
        set_camera_position(&cameraFocusOffset);
        return;
    }
    
    Vec3 camPos;
    add_vec(&cameraFocusOffset, &cube->transform.pos, &camPos);
    set_camera_position(&camPos);
}

void update_camera()
{
    switch (cameraMode) {
        case FOLLOW:
            // follow the last cube with camera
            if (cubesSize) {
                camera_focus_cube(&cubes[cubesSize-1]);
            } else {
                camera_focus_cube(0);
            }
            break;
        case FREE:
            // do nothing
            break;
        default: break;
    }
}

void num_to_str(char * str, int strSize, int n)
{
    char sign = 1;
    if (n < 0) {
        sign = -1;
        n *= -1;
    }
    
    for (int i = 0; i < strSize; i++)
    {
        str[i] = 0x20;
    }
    
    str[strSize-1] = '\0';
    int j = 1;
    for (int i = strSize-2; i >= 1; i--)
    {
        str[i] = (n % 10) + '0';
        n /= 10;
        j = i;
        if (n == 0) break;
    }

    if (sign == -1) {
        str[j-1] = '-';
    }
}

void update_ascii(void)
{
    char x[5], y[5], z[5];
    char *xp = &x[0], *yp = &y[0], *zp = &z[0];
    num_to_str(xp, 5, (int) get_camera_position()->x);
    num_to_str(yp, 5, (int) get_camera_position()->y);
    num_to_str(zp, 5, (int) get_camera_position()->z);
    
    ascii_gotoxy(1,1);
    ascii_write_char('X');
    ascii_write_char('=');
    while (*xp) ascii_write_char(*xp++);
    
    ascii_write_char(' ');
    
    ascii_write_char('Y');
    ascii_write_char('=');
    while (*yp) ascii_write_char(*yp++);
    
    ascii_write_char(' ');
    
    ascii_write_char('Z');
    ascii_write_char('=');
    while (*zp) ascii_write_char(*zp++);
    
    ascii_gotoxy(1,2);
    char * mode_str = "CAM MODE = ";
    while (*mode_str) ascii_write_char(*mode_str++);
    
    if (cameraMode == FREE) 
    {
        mode_str = "FREE";
    } else if (cameraMode == FOLLOW)
    {
        mode_str = "FOLLOW";
    }
    while (*mode_str) ascii_write_char(*mode_str++);
}

void game_loop(void)
{
    while (1)
    {

        update_camera();

        for (int i = 0; i < cubesSize; i++)
        {
            draw_object(&cubes[i]);
        }
        
        update_ascii();
        
        handle_key_input();
        
        delay_milli(1);
        graphic_clear_screen();       
    }
}

void main(void)
{
    init_graphics();
    init_keypad(&GPIO_D);
    ascii_init();
    // show_rotating_cube();
    // show_rotating_teapot();
    
    game_loop();
}
