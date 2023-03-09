/*
 * 	startup.c
 *
 */
#define PORT_BASE ((volatile unsigned int *) 0x40021000)
#define PORT_MODER ((volatile unsigned int *) PORT_BASE)
#define PORT_OTYPER ((volatile unsigned short *) PORT_BASE+4)
#define PORT_OSPEEDR ((volatile unsigned int *) PORT_BASE+8)
#define PORT_PUPDR ((volatile unsigned int *) PORT_BASE+0xC)

#ifndef ASCIIDISP_H
#define ASCIIDISP_H

#define PORT_IDR_LOW ((volatile unsigned char *) PORT_BASE+0x10)
#define PORT_IDR_HIGH ((volatile unsigned char *) PORT_BASE+0x11)
#define PORT_ODR_LOW ((volatile unsigned char *) PORT_BASE+0x14)
#define PORT_ODR_HIGH ((volatile unsigned char *) PORT_BASE+0x15)

#define B_E         0x40
#define B_SELECT    4
#define B_RW        2
#define B_RS        1

void ascii_ctrl_bit_set(unsigned char x);
void ascii_ctrl_bit_clear(unsigned char x);
void ascii_write_controller(unsigned char byte);
unsigned char ascii_read_controller(void);
void ascii_write_cmd(unsigned char command);
void ascii_write_data(unsigned char data);
unsigned char ascii_read_status(void);
unsigned char ascii_read_data(void);
void ascii_command(unsigned char command);
void ascii_init(void);
void ascii_write_char(char c);
void ascii_gotoxy(int x, int y);

#endif