/*
 * 	startup.c
 *
 */
 
unsigned char * GPIO_ODR_HIGH = 0x40020C15;
unsigned char * GPIO_ODR_LOW = 0x40020C14;
unsigned char * GPIO_IDR_HIGH = 0x40020C11;
unsigned int * GPIO_MODER = 0x40020C00;
unsigned short * GPIO_OTYPER = 0x40020C04;
unsigned int * GPIO_PUPDR = 0x40020C0C;
 
__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

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

void out7seg(unsigned char c) {
    if (c >= 16) * GPIO_ODR_LOW = 0xFF;
    
    char keys[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x77, 0b01111100, 0b00111001, 0x5E, 0x80, 0b01110110}; // incomplete
    * GPIO_ODR_LOW = keys[c];
}

void main(void)
{
    app_init();
    while (1)
    {
        out7seg(keyb());
    }
}

