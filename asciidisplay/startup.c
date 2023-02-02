/*
 * 	startup.c
 *
 */
 
#define STK_CTRL ((volatile unsigned int *) 0xE000E010)
#define STK_LOAD ((volatile unsigned int *) STK_CTRL+4)
#define STK_VAL ((volatile unsigned int *) STK_CTRL+8)

#define PORT_BASE ((volatile unsigned int *) 0x40021000)
#define PORT_MODER ((volatile unsigned int *) PORT_BASE)
#define PORT_OTYPER ((volatile unsigned short *) PORT_BASE+4)
#define PORT_OSPEEDR ((volatile unsigned int *) PORT_BASE+8)
#define PORT_PUPDR ((volatile unsigned int *) PORT_BASE+0xC)

#define PORT_IDR_LOW ((volatile unsigned char *) PORT_BASE+0x10)
#define PORT_IDR_HIGH ((volatile unsigned char *) PORT_BASE+0x11)
#define PORT_ODR_LOW ((volatile unsigned char *) PORT_BASE+0x14)
#define PORT_ODR_HIGH ((volatile unsigned char *) PORT_BASE+0x15)

#define B_E         0x40
#define B_SELECT    4
#define B_RW        2
#define B_RS        1

__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

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
    
    unsigned char rv = *PORT_ODR_HIGH;
    
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
    *PORT_MODER = 0x55555555;
    
    ascii_ctrl_bit_clear(B_RS);
    ascii_ctrl_bit_set(B_RW);
    
    char rv = ascii_read_controller();
    
    *PORT_MODER = 0x00005555;
    return rv;
}

unsigned char ascii_read_data(void)
{
    *PORT_MODER = 0x55555555;
    
    ascii_ctrl_bit_set(B_RS);
    ascii_ctrl_bit_set(B_RW);
    
    char rv = ascii_read_controller();
    
    *PORT_MODER = 0x00005555;
    return rv;
}

void ascii_command(unsigned char command)
{
    while((ascii_read_status() & 0x80) == 0x80);
    delay_micro(8);
    ascii_write_cmd(command);
    delay_milli(2);
}

void main(void)
{
    char c = 100;
    while(c) {
        delay_milli(100);
        c--;
    }
}

