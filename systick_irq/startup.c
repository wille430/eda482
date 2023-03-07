/*
 * 	startup.c
 *
 */
 
// TA BORT FÖR KÖRNING PÅ HÅRDVARA 
#define SIMULATOR

#ifdef SIMULATOR
    #define DELAY_COUNT 2000
#else
    #define DELAY_COUNT 1000000
#endif
 
#define SCB_BASE    0xE000ED00
#define SCB_VTOR    ((volatile unsigned int *) (SCB_BASE+0x8))
 
#define STK_BASE    0xE000E010
#define STK_CTRL    ((volatile unsigned int *) (STK_BASE+0x0))
#define STK_LOAD    ((volatile unsigned int *) (STK_BASE+0x4))
#define STK_VAL     ((volatile unsigned int *) (STK_BASE+0x8))

#define GPIO_D_BASE     0x40020C00
#define GPIO_D_MODER    ((volatile unsigned int *) (GPIO_D_BASE+0x0))
#define GPIO_D_ODR_LOW  ((volatile unsigned short *) (GPIO_D_BASE+0x14))
 
__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

// 0=fördröjning aktiv
// 1=fördröjning oaktiv
unsigned char systick_flag = 1;

void delay_1micro(void)
{
    systick_flag = 0;
    // initiera räknarkrets för 1 us räknarintervall
    *STK_CTRL = 0;
    *STK_LOAD = 168 - 1;
    *STK_VAL  = 0;
    // aktivera räknaren
    *STK_CTRL = 7;
}

unsigned int delay_count = 0;
void systick_irq_handler(void)
{
    *STK_CTRL = 0;
    delay_count--;
    if (delay_count > 0)
    {
        delay_1micro();
    } else {
        systick_flag = 1;
    }
}

void init_app(void)
{
    *SCB_VTOR &= 0;
    *SCB_VTOR |= 0x2001C000;
    *((void (**)(void)) 0x2001C03C) = systick_irq_handler;
    
    // initiera port D
    *GPIO_D_MODER = 0x00005555;
}

void delay(unsigned int count)
{
    delay_count = count;
    delay_1micro();
}

void main(void)
{
    init_app();
    *GPIO_D_ODR_LOW = 0;
    delay(DELAY_COUNT);
    *GPIO_D_ODR_LOW = 0xFF;
    
    int counter = 0;
    while (1)
    {
        if (systick_flag)
            break;
            
        // kod som ska utföras under väntetiden
        counter++;
    }
    
    *GPIO_D_ODR_LOW = 0;
}

