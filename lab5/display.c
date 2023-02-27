
#include "geometry.h"

// Drivers

__attribute__((naked))
void graphic_initialize(void)
{
__asm__ volatile(".HWORD    0xDFF0\n");
__asm__ volatile(" BX       LR\n");
}

__attribute__((naked))
void graphic_clear_screen(void)
{
__asm__ volatile(".HWORD    0xDFF1\n");
__asm__ volatile(" BX       LR\n");
}

__attribute__((naked))
void graphic_pixel_set(int x, int y)
{
__asm__ volatile(".HWORD    0xDFF2\n");
__asm__ volatile(" BX       LR\n");
}


__attribute__((naked))
void graphic_pixel_clear(int x, int y)
{
__asm__ volatile(".HWORD    0xDFF3\n");
__asm__ volatile(" BX       LR\n");
}