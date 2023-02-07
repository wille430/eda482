/*
 * 	startup.c
 *
 */
#define STK_CTRL ((volatile unsigned int *) 0xE000E010)
#define STK_LOAD ((volatile unsigned int *) STK_CTRL+4)
#define STK_VAL ((volatile unsigned int *) STK_CTRL+8)

#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64
 
__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

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

// Delays

void delay_250ns(void)
{
    *STK_CTRL = 0;
    *STK_LOAD = 168/4;
    *STK_VAL = 0;
    *STK_CTRL = 5;
    
    while ((*STK_CTRL & 0x10000) == 0);
    
    *STK_CTRL = 0;
}

void delay_micro(unsigned int us) {
    while (us > 0)
    {
        delay_250ns();
        delay_250ns();
        delay_250ns();
        delay_250ns();
        us--;
    }
}

void delay_milli(unsigned int ms) {
    
#ifdef SIMULATOR
    ms /= 1000;
    ms++;
#endif
    
    delay_micro(ms * 1000);
}

typedef struct
{
    char x, y;
} POINT, *PPOINT;

typedef struct
{
    POINT p0, p1;
} LINE, *PLINE;

typedef struct
{
    POINT origo;
    char x, y;
} RECT, *PRECT;

int abs(int n)
{
    if (n < 0) return -n;
    return n;
}

void swap(char * n, char * m)
{
    char tmp = *n;
    *n = *m;
    *m = *n;
}

int valid_point(PPOINT p)
{
    return p->x >= 0 && p->x < SCREEN_WIDTH && p->y >= 0 && p->y < SCREEN_WIDTH;
}

int valid_line(PLINE l)
{
    return valid_point(&l->p0) && valid_point(&l->p1);
}

int draw_line(PLINE l)
{
    if (!valid_line(l))
    {
        return 0;
    }
    
    char steep = 0;
    if (abs(l->p1.y - l->p0.y) > abs(l->p1.x - l->p0.x))
    {
        steep = 1;
    }
    
    if (steep)
    {
        swap(&(l->p0.x), &(l->p1.x));
        swap(&(l->p0.y), &(l->p1.y));
    }
    
    int dX = l->p1.x - l->p0.x;
    int dY = abs(l->p1.y - l->p0.y);
    int ystep = -1;
    int error = 0;
    char y = l->p0.y;
    if (l->p0.y < l->p1.y) ystep = 1;
    
    for (int x  = l->p0.x; x <= l->p1.x; x++) {
        if (steep)
        {
            graphic_pixel_set(x, y);
        } else
        {
            graphic_pixel_set(y, x);
        }
        
        error += dY;
        if (2*error >= dX)
        {
            y += ystep;
            error -= dX;
        }
    }
    
    return 1;
}

int draw_rect(PRECT rect)
{
    LINE lines[] = {
        {rect->origo.x, rect->origo.y, rect->origo.x, rect->origo.y+rect->y}, // Left vertical
        {rect->origo.x, rect->origo.y, rect->origo.x+rect->x, rect->origo.y}, // Upper horizontal
        {rect->origo.x, rect->origo.y+rect->y, rect->origo.x+rect->x, rect->origo.y+rect->y}, // Bottom horizontal
        {rect->origo.x+rect->x, rect->origo.y, rect->origo.x+rect->x, rect->origo.y+rect->y} // Right vertical
    };
    
    int ret = 1;
    for (int i = 0; i < sizeof(lines) / sizeof(LINE); i++)
    {
        ret &= draw_line(&lines[i]);
    }
    return ret;
}

LINE lines[] = {
    {40,10, 100,10},
    {40,10, 100,20},
    {40,10, 100,30},
    {40,10, 100,40},
    {40,10, 100,50},
    {40,10, 100,60},
    {40,10, 90,60},
    {40,10, 80,60},
    {40,10, 70,60},
    {40,10, 60,60},
    {40,10, 50,60},
    {40,10, 40,60}
};

RECT rectangles[] = {
    {10,10, 20,10},
    {25,25, 10,20},
    {40,30, 70,20},
    {60,35, 10,10},
    {70,10, 5,5}
};

void main(void)
{
    graphic_initialize();
    graphic_clear_screen();
    
    /*
    // ex 3.12
    while (1)
    {
        for (int i = 0; i < sizeof(lines)/sizeof(LINE); i++)
        {
            draw_line(&lines[i]);
            delay_milli(500);
        }
        graphic_clear_screen();
    }
    */
    
    LINE l = {10,10, 20,10};
    draw_line(&l);
    
    // ex 3.13
    while (1)
    {
        for (int i = 0; i < sizeof(rectangles)/sizeof(RECT); i++)
        {
            draw_rect(&rectangles[i]);
            delay_milli(500);
        }
        graphic_clear_screen();
    }
}
