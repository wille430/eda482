/*
 * 	startup.c
 *
 */
#include "registers.h"
 
#define GPIO_D (*(volatile PGPIO) 0x40020C00)
#define GPIO_E (*(volatile PGPIO) 0x40021000)

#define SYSCFG_EXTICR1 ((volatile unsigned int *) 0x40013808)

#define EXTI_BASE 0x40013C00
#define EXTI_IMR ((volatile unsigned int *) EXTI_BASE)
#define EXTI_RTSR ((volatile unsigned int *) (EXTI_BASE+0x8))
#define EXTI_FTSR ((volatile unsigned int *) (EXTI_BASE+0xC))
#define EXTI_PR ((volatile unsigned int *) (EXTI_BASE+0x14))

#define B_EXTI3 (1<<3)

#define SCB_VTOR ((volatile unsigned int *) (0xE000ED00+0x8))
#define VECTOR_TABLE_BASE 0x2001C000

#define NVIC_BASE 0xE000E100
#define NVIC_ISER ((volatile unsigned int *) NVIC_BASE)
 
__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

static unsigned int count = 0;

void interrupt_handler(void)
{
    // kolla om avbrott från EXTI3
    count++;
    *EXTI_PR &= ~B_EXTI3;
    // isåfall, inkrementera count och kvitera EXTI3
}

void app_init(void)
{
    // sätt D0-7 till utport
    GPIO_D.MODER = 0x00005555;
    
    // koppla PE3 till EXTI3
    *SYSCFG_EXTICR1 &= ~0xF000;
    *SYSCFG_EXTICR1 |= (1<<14);
    
    // konfigurera EXTI3 för avbrott
    *EXTI_IMR |= B_EXTI3;
    // konfigurera avbrott på positiv flank
    *EXTI_RTSR |= B_EXTI3;
    *EXTI_FTSR &= ~B_EXTI3;
    
    (*(void (**)(void)) (VECTOR_TABLE_BASE+0x64)) = interrupt_handler;
    
    // flytta på avbrottsvektor
    *SCB_VTOR = VECTOR_TABLE_BASE;
    
    // konfigurera NVIC
    *NVIC_ISER |= (1<<9);
}

void main(void)
{
    app_init();
    while(1)
    {
        GPIO_D.ODR_LOW = count;
    }
}
