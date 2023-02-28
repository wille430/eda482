/*
 * 	startup.c
 *
 */
 
#include "graphics.h"
#include "vertexdata.h"
#include "delay.h"

__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

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
    
    cube.transform.pos.z = -10;
    cube.transform.pos.y = cube.shape.height*0.75;
    cube.transform.pos.x = cube.shape.width*0.25;
    
    while (1)
    {
        draw_object(&cube);
        rotate_object_x(&cube, -30.0);
        rotate_object_z(&cube, 30.0);
        delay_milli(1);
        graphic_clear_screen();
        
        // Vec3 cam_dpos = {0, 0, 5};
        // move_camera(&cam_dpos);
    }
}

void show_rotating_teapot()
{
    teapot.transform.pos.z = -20;
    while (1)
    {
        draw_object(&teapot);
        rotate_object_y(&teapot, 30.0);
        delay_milli(1);
        graphic_clear_screen();
    }
}

void main(void)
{
    init_graphics();
    show_rotating_cube();
    // show_rotating_teapot();
}
