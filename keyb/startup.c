/*
 * 	startup.c
 *
 */
 
 unsigned char * OUTPUT = 0x40020c14;
 
__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile("BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}


unsigned char * GPIO_ODR_HIGH = 0x40020c15;
unsigned char * GPIO_IDR_HIGH = 0x40020c11;
volatile unsigned short * GPIO_MODER = 0x40020c02;

void kbdActivate(unsigned int row)
{
    switch (row)
    {
        case 1: * GPIO_ODR_HIGH = 0x10; break;
        case 2: * GPIO_ODR_HIGH = 0x20; break;
        case 3: * GPIO_ODR_HIGH = 0x40; break;
        case 4: * GPIO_ODR_HIGH = 0x80; break;
        default: break; * GPIO_ODR_HIGH = 0x0;
    }
}


int kbdGetCol(void)
{
    unsigned char c;
    c = * GPIO_IDR_HIGH;
    
    if (c & 8) return 4;
    if (c & 4) return 3;
    if (c & 2) return 2;
    if (c & 1) return 1;
    
    return 0;
}

char keyb(void)
{
    char key[] = {1,2,3,0Xa,4,5,6,0Xb,7,8,9,0xc,0xe, 0, 0xf, 0xd};
    int row, col;
    for (row = 1; row <= 4; row++) {
        kbdActivate(row);
        if (col = kbdGetCol())
        {
            kbdActivate(0);
            return key[col - 1 + (row-1) * 4];
        }
    }
    return 0xff;
}

void init(void)
{
    * GPIO_MODER |= 0x5555;
}

void main(void)
{
    init();
    char out;
    while (1)
    {
        out = keyb();
    }
}