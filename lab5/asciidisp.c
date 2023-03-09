/*
 * 	startup.c
 *
 */
#include "asciidisp.h"

void ascii_ctrl_bit_set(unsigned char x)
{
    *PORT_ODR_LOW |= x | B_SELECT;
}

void ascii_ctrl_bit_clear(unsigned char x)
{
    *PORT_ODR_LOW &= ~x;
    *PORT_ODR_LOW |= B_SELECT;
}

void ascii_write_controller(unsigned char byte)
{
    ascii_ctrl_bit_set(B_E);
    *PORT_ODR_HIGH = byte;
    delay_250ns();
    ascii_ctrl_bit_clear(B_E);
}

unsigned char ascii_read_controller(void) {
    *PORT_MODER = 0x00005555;
    
    ascii_ctrl_bit_set(B_E);
    delay_250ns();

    unsigned char rv = *PORT_IDR_HIGH;
    
    ascii_ctrl_bit_clear(B_E);
    
    *PORT_MODER = 0x55555555;
    return rv;
}

void ascii_write_cmd(unsigned char command)
{
    ascii_ctrl_bit_clear(B_RS);
    ascii_ctrl_bit_clear(B_RW);
    ascii_write_controller(command);
}

void ascii_write_data(unsigned char data)
{
    ascii_ctrl_bit_set(B_RS);
    ascii_ctrl_bit_clear(B_RW);
    ascii_write_controller(data);
}

unsigned char ascii_read_status(void)
{
    *PORT_MODER &= 0x0000FFFF;
    
    ascii_ctrl_bit_clear(B_RS);
    ascii_ctrl_bit_set(B_RW);
    
    char rv = ascii_read_controller();
    
    *PORT_MODER |= 0x55550000;
    return rv;
}

unsigned char ascii_read_data(void)
{
    *PORT_MODER &= 0x0000FFFF;
    
    ascii_ctrl_bit_set(B_RS);
    ascii_ctrl_bit_set(B_RW);
    
    char rv = ascii_read_controller();
    
    *PORT_MODER |= 0x55550000;
    return rv;
}

void ascii_command(unsigned char command)
{
    while((ascii_read_status() & 0x80) == 0x80);
    delay_micro(8);
    ascii_write_cmd(command);
    delay_milli(2);
}

void ascii_init(void)
{
    ascii_command(0b00111000);
    ascii_command(0b00001110);
    ascii_command(0b00000110);
    *PORT_MODER = 0x55555555;
}

void ascii_write_char(char c)
{
    while ((ascii_read_status() & 0x80) == 0x80);
    delay_micro(8);
    ascii_write_data(c);
    delay_milli(2);
}

void ascii_gotoxy(int x, int y)
{
    x--;
    y--;
    if (y == 1) {
        x += 0x40;
    }
    ascii_write_cmd(0x80 | x);
}

