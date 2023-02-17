/*
 * 	startup.c
 *
 */
#define STK_BASE 0xE000E010
#define STK_CTRL ((volatile unsigned int *) STK_BASE)
#define STK_LOAD ((volatile unsigned int *) (STK_BASE+4))
#define STK_VAL ((volatile unsigned int *) (STK_BASE+8))

#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64

#define MAX_POINTS 30
 
__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile("     LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile("     MOV SP,R0\n");
__asm__ volatile("     BL main\n");					/* call main */
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

    while (ms > 0)
    {
        delay_micro(1000);
        ms--;
    }
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
    int  (* collides) (struct tObj *, struct tObj *);
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
        graphic_pixel_set(p.x+obj->posx, p.y+obj->posy);
    }
}

void clear_object(POBJECT obj)
{
    for (int i = 0; i < obj->geo->numpoints; i++)
    {
        POINT p = obj->geo->px[i];
        graphic_pixel_clear(p.x+obj->posx, p.y+obj->posy);
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

void move_object(POBJECT obj)
{
    clear_object(obj);
    
    obj->posx += obj->dirx;
    obj->posy += obj->diry;
    
    draw_object(obj);
}

void set_object_speed(POBJECT obj, int dirx, int diry)
{
    obj->dirx = dirx;
    obj->diry = diry;
}

int object_collides(POBJECT obj1, POBJECT obj2)
{
    return obj1->posx < obj2->posx + obj2->geo->sizex &&
           obj1->posx + obj1->geo->sizex > obj2->posx &&
           obj1->posy < obj2->posy + obj2->geo->sizey &&
           obj1->posy + obj1->geo->sizey > obj2->posy;
}

int is_out_of_bounds(POBJECT obj)
{
    return obj->posx < 0 ||
           obj->posx + obj->geo->sizex >= SCREEN_WIDTH ||
           obj->posy < 0 ||
           obj->posy + obj->geo->sizey >= SCREEN_HEIGHT;
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
    4,1,
    1,1,
    draw_object,
    clear_object,
    move_ball_object,
    set_object_speed,
    object_collides
};

void set_spider_speed(POBJECT spider, int dirx, int diry)
{
    int signX = dirx < 0 ? -1 : 1;
    int signY = diry < 0 ? -1 : 1;
    spider->dirx = abs(dirx) > 2 ? signX * 2 : dirx;
    spider->diry = abs(diry) > 2 ? signY * 2 : diry;
}

GEOMETRY spider_geometry =
{
    22,
    6,8,
    {
        {2,0},{3,0},
        {1,1},{4,1},
        {0,2},{1,2},{2,2},{3,2},{4,2},{5,2},
        {0,3},{2,3},{3,3},{5,3},
        {1,4},{4,4},
        {2,5},{3,5},
        {1,6},{4,6},
        {0,7},{5,7}
    }
};

static OBJECT spider =
{
    &spider_geometry,
    0,0,
    SCREEN_WIDTH - 12, (SCREEN_HEIGHT - 9) / 2,
    draw_object,
    clear_object,
    move_object,
    set_spider_speed,
    object_collides
};

// keypad
#define GPIO_BASE 0x40020C00
#define GPIO_ODR_HIGH ((unsigned char *) (GPIO_BASE+0x15))
#define GPIO_ODR_LOW ((unsigned char *) (GPIO_BASE+0x14))
#define GPIO_IDR_HIGH ((unsigned char *) (GPIO_BASE+0x11))
#define GPIO_MODER ((unsigned int *) GPIO_BASE)
#define GPIO_OTYPER ((unsigned short *) (GPIO_BASE+0x04))
#define GPIO_PUPDR ((unsigned int *) (GPIO_BASE+0x0C))

void app_init(void)
{
    // Configure keypad D8-15
    * GPIO_OTYPER &= 0x00FF;
    
    * GPIO_PUPDR &= 0x0000FFFF;
    * GPIO_PUPDR |= 0x00AA0000;
    
    * GPIO_MODER &= 0x0000FFFF;
    * GPIO_MODER |= 0x55000000;
    
    // Configure hex display D0-7
    * GPIO_MODER &= 0xFFFF0000;
    * GPIO_MODER |= 0x00005555;
}

unsigned int ReadColumn()
{
    char c = * GPIO_IDR_HIGH;
    if (c & 1) return 1;
    if (c & 2) return 2;
    if (c & 4) return 3;
    if (c & 8) return 4;
    
    return 0;
}

void ActivateRow(int row)
{
    switch (row)
    {
        case 1: * GPIO_ODR_HIGH = 0x10; break;
        case 2: * GPIO_ODR_HIGH = 0x20; break;
        case 3: * GPIO_ODR_HIGH = 0x40; break;
        case 4: * GPIO_ODR_HIGH = 0x80; break;
        default: * GPIO_ODR_HIGH = 0x00;
    }
}

unsigned char keyb(void)
{
    char keys[] = { 1, 2, 3, 0xa, 4, 5, 6, 0xb, 7, 8, 9, 0xc, 0xe, 0, 0xf, 0xd };
    int row, col;
    for (row = 1; row <= 4; row++)
    {
        ActivateRow(row);
        if (col = ReadColumn())
        {
            ActivateRow(0);
            return keys[(row - 1) * 4 + (col - 1)];
        }
    }
    
    return 0xFF;
}

#define SIMULATOR
void main(void)
{
    char c;
    POBJECT victim = &ballobject;
    POBJECT creature = &spider;

    app_init();
    graphic_initialize();
    graphic_clear_screen();
    
    victim->set_speed(victim, 4, 1);
    
    while(1)
    {
        victim->move(victim);
        creature->move(creature);
        
        delay_micro(100);
        c = keyb();
        switch(c)
        {
            case 6: creature->set_speed(creature, 2, 0); break;
            case 4: creature->set_speed(creature, -2, 0); break;
            case 5: creature->set_speed(creature, 0, 0); break;
            case 2: creature->set_speed(creature, 0, -2); break;
            case 8: creature->set_speed(creature, 0, 2); break;
            default:
                creature->set_speed(creature, 0, 0); break;
        }
        
        if (victim->collides(victim, creature) ||
            is_out_of_bounds(creature)
        ) {
            break;
        }
        delay_micro(20);
    }
}

