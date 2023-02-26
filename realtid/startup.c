/*
 * 	startup.c
 *
 */
 #include "registers.h"
 
__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

#define TIM6    (*((volatile PTIMX) 0x40001000))
#define GPIO_D  (*((volatile PGPIO) 0x40020C00))

#define B_CEN   (1<<0)
#define B_UDIS  (1<<1)

#define B_UIE   (1<<0)
#define B_UIF   (1<<0)

#define SCB_BASE    0xE000ED00
#define SCB_VTOR    ((volatile unsigned int *) (SCB_BASE+0x8))

#define TIM6_IRQVEC         ((void (**)(void)) (0x2001C000+0x118))
#define NVIC_BASE           0xE000E100
#define NVIC_TIM6_ISER      ((unsigned int *) (NVIC_BASE+0x4))
#define NVIC_TIM6_IRQ_BPOS  (1<<22)

volatile int ticks;
volatile int seconds;

void timer6_interrupt(void)
{
    TIM6.SR &= ~B_UIF;
    ticks++;
    if (ticks > 9)
    {
        ticks = 0;
        seconds++;
    }
}

void timer6_init(void)
{
    ticks = 0;
    seconds = 0;
    TIM6.CR1        &= ~B_CEN;
    *TIM6_IRQVEC    = timer6_interrupt;
    *NVIC_TIM6_ISER |= NVIC_TIM6_IRQ_BPOS;
    
    /* 100ms tidbas */
    TIM6.PSC    = 839*6;
    TIM6.ARR    = 9999*4;
    TIM6.DIER   |= B_UIE;
    TIM6.CR1    |= B_CEN;
}

void gpio_init()
{
    GPIO_D.MODER = 0x00005555;
}

void app_init()
{
    *SCB_VTOR &= 0x0;
    *SCB_VTOR = 0x2001C000;
}

void main(void)
{
    char nbcd;
    app_init();
    gpio_init();
    timer6_init();
    
    while (1)
    {
        nbcd = (seconds/10) << 4;
        nbcd |= (seconds % 10) & 0xF;
        GPIO_D.ODR_LOW = nbcd;
    }
}

