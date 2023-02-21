/*
 * 	startup.c
 *
 */
#define STK_CTRL ((volatile unsigned int *) 0xE000E010)
#define STK_LOAD ((volatile unsigned int *) STK_CTRL+4)
#define STK_VAL ((volatile unsigned int *) STK_CTRL+8)

#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64

#define MAX_POINTS 30
 
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

typedef struct polygonpoint
{
    char x, y;
    struct polygonpoint *next;
} POLYPOINT, *PPOLYPOINT;

typedef struct 
{
    int numpoints;
    int sizex;
    int sizey;
    POINT px[MAX_POINTS];
} GEOMETRY, *PGEOMETRY;

typedef struct tObj
{
    PGEOMETRY geo;
    int dirx, diry;
    int posx, posy;
    void (* draw) (struct tObj *);
    void (* clear) (struct tObj *);
    void (* move) (struct tObj *);
    void (* set_speed) (struct tObj *, int, int);
} OBJECT, *POBJECT;

int abs(int n)
{
    if (n < 0) return -n;
    return n;
}

void swap(char * n, char * m)
{
    char tmp = *n;
    *n = *m;
    *m = tmp;
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
    if (!valid_line(l)) return 0;
    
    char x0, y0, x1, y1;
    x0 = l->p0.x;
    y0 = l->p0.y;
    x1 = l->p1.x;
    y1 = l->p1.y;
    
    signed char dx, dy, sx, sy;
    dx = abs(x1 - x0);
    sx = x0 < x1 ? 1 : -1;
    dy = -abs(y1 - y0);
    sy = y0 < y1 ? 1 : -1;
    
    int err = dx + dy;
    int e2;
    
    while (1)
    {
        graphic_pixel_set(x0, y0);
        if (x0 == x1 && y0 == y1) break;
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

/*
int draw_line(PLINE l)
{
    signed char x0, y0, x1, y1, steep, dX, dY;
    x0 = l->p0.x;
    y0 = l->p0.y;
    x1 = l->p1.x;
    y1 = l->p1.y;
    
    steep = 0;
    
    if (!valid_line(l))
    {
        return 0;
    }
    
  
    // for vertical lines
    if (abs(x1 - x0) == 0 && y1 != y0) {
        swap(&x1, &y0);
        swap(&x0, &y1);
        steep = 1;
    } else 
 
    
    if (abs(y1 - y0) > abs(x1 - x0))
    {
        steep = 1;
        swap(&x0, &x1);
        swap(&y0, &y1);
    }
    
    signed char ystep, error, y;
    dX = x1 - x0;
    dY = abs(y1 - y0);
    ystep = -1;
    error = 0;
    y = y0;
    
    if (y0 < y1) ystep = 1;
    
    char startX = x0;
    char endX = x1;
    
    if (dX < 0) {
        swap(&startX, &endX);
    }
    
    for (char x = startX; x <= endX; x++) {
        if (steep == 1)
        {
            graphic_pixel_set(y, x);
        } else
        {
            graphic_pixel_set(x, y);
        }
        
        error += dY;
        if (2*error >= dX)
        {
            y += ystep;
            error -= dX;
        }
        x++;
    }
    
    return 1;
}
*/

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

int draw_polygon(PPOLYPOINT point)
{
    POINT p0 = {point->x, point->y};
    PPOLYPOINT pnext = point->next;
    char ret = 1;
    while (pnext != 0)
    {
        POINT p1 = {pnext->x, pnext->y};
        LINE l = {p0, p1};
        ret &= draw_line(&l);
        p0.x = p1.x;
        p0.y = p1.y;
        pnext = pnext->next;
    }
    
    return ret;
}

void draw_object(POBJECT obj)
{
    for (int i = 0; i < obj->geo->numpoints; i++)
    {
        POINT p = obj->geo->px[i];
        graphic_pixel_set(p.x, p.y);
    }
}

void clear_object(POBJECT obj)
{
    for (int i = 0; i < obj->geo->numpoints; i++)
    {
        POINT p = obj->geo->px[i];
        graphic_pixel_clear(p.x, p.y);
    }
}

void move_ball_object(POBJECT obj)
{
    clear_object(obj);
    
    obj->posx += obj->dirx;
    obj->posy += obj->diry;
    
    if (obj->posx < 1 ||
        obj->posx + obj->geo->sizex > SCREEN_WIDTH) obj->dirx *= -1;
    if (obj->posy < 1 ||
        obj->posy + obj->geo->sizey > SCREEN_HEIGHT) obj->diry *= -1;
    
    draw_object(obj);
}

void set_object_speed(POBJECT obj, int dirx, int diry)
{
    obj->dirx = dirx;
    obj->diry = diry;
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

POLYPOINT pg8 = {20,20, 0};
POLYPOINT pg7 = {20,55, &pg8};
POLYPOINT pg6 = {70,60, &pg7};
POLYPOINT pg5 = {80,35, &pg6};
POLYPOINT pg4 = {100,25, &pg5};
POLYPOINT pg3 = {90,10, &pg4};
POLYPOINT pg2 = {40,10, &pg3};
POLYPOINT pg1 = {20,20, &pg2};

GEOMETRY ball_geometry =
{
    12,
    4,4,
    {
        {0,1},{0,2},{1,0},{1,1},{1,2},{1,3},{2,0},{2,1},{2,2},{2,3},{3,1},{3,2}
    }
};

static OBJECT ballobject =
{
    &ball_geometry,
    0,0,
    1,1,
    draw_object,
    clear_object,
    move_ball_object,
    set_object_speed
};

void main(void)
{
    graphic_initialize();
    graphic_clear_screen();
    
    // ex 3.12
    /*
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
    
    // ex 3.13
    /*
    while (1)
    {
        for (int i = 0; i < sizeof(rectangles)/sizeof(RECT); i++)
        {
            draw_rect(&rectangles[i]);
            delay_milli(500);
        }
        graphic_clear_screen();
    }
    */
    
    // ex3.14
    while (1)
    {
        draw_polygon(&pg1);
        delay_milli(500);
        graphic_clear_screen();
        delay_milli(500);
    }
}
