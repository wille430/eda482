
#include "registers.h"

static volatile PGPIO pgpio;

void init_keypad(PGPIO gpio)
{
    pgpio = gpio;
    
    // Configure keypad D8-15
    gpio->OTYPER &= 0x00FF;
    
    gpio->PUPDR &= 0x0000FFFF;
    gpio->PUPDR |= 0x00AA0000;
    
    gpio->MODER &= 0x0000FFFF;
    gpio->MODER |= 0x55000000;
}

unsigned int ReadColumn()
{
    char c = pgpio->IDR_HIGH;
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
        case 1: pgpio->ODR_HIGH = 0x10; break;
        case 2: pgpio->ODR_HIGH = 0x20; break;
        case 3: pgpio->ODR_HIGH = 0x40; break;
        case 4: pgpio->ODR_HIGH = 0x80; break;
        default: pgpio->ODR_HIGH = 0x00;
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
