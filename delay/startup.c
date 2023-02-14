/*
 * 	startup.c
 *
 */
#define SIMULATOR 1
#define STK_CTRL ((volatile unsigned int *) 0xE000E010)
#define STK_LOAD ((volatile unsigned int *) STK_CTRL+4)
#define STK_VAL ((volatile unsigned int *) STK_CTRL+8)

#define GPIO_E_MODER ((volatile unsigned int *) 0x40021000)
#define Bargraph ((volatile unsigned char *) 0x40021014)

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

void init_app()
{
    *GPIO_E_MODER = 0x00005555;
}

void main(void)
{
    init_app();
    while (1)
    {
        *Bargraph = 0;
        delay_milli(500);
        *Bargraph = 0xFF;
        delay_milli(500);
    }
}

