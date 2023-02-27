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

int get_irq_number()
{
    switch(GPIO_E.IDR_LOW & 0b111)
    {
        case 1: return 0;
        case 2: return 1;
        case 4: return 2;
        default: return -1;
    }
}

void reset_irq(char num)
{
    char bit = (1<<4) << num;
    GPIO_E.ODR_LOW &= ~bit;
    GPIO_E.ODR_LOW |= bit;
    GPIO_E.ODR_LOW &= ~bit;
}

void interrupt_handler(void)
{
    *EXTI_PR |= B_EXTI3;
    switch (get_irq_number())
    {
        case 0:
            reset_irq(0);
            count++;
            GPIO_D.ODR_LOW = count;
            break;
        case 1:
            reset_irq(1);
            count = 0;
            GPIO_D.ODR_LOW = count;
            break;
        case 2:
            reset_irq(2);
            GPIO_D.ODR_LOW = GPIO_D.ODR_LOW == 0 ? 0xFF : 0x00;
            break;
        default:
            break;
    }
}

void app_init(void)
{
    // sätt D0-7 till utport
    GPIO_D.MODER = 0x00005555;
    GPIO_E.MODER = 0x00001500;
    
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
    while(1);
}
