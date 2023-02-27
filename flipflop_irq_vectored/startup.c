/*
 * 	startup.c
 *
 */
#include "registers.h"
 
#define GPIO_D (*(volatile PGPIO) 0x40020C00)
#define GPIO_E (*(volatile PGPIO) 0x40021000)

#define SYSCFG_BASE 0x40013800
#define SYSCFG_EXTICR1 ((volatile unsigned int *) (SYSCFG_BASE+0x8))

#define EXTI_BASE 0x40013C00
#define EXTI_IMR ((volatile unsigned int *) EXTI_BASE)
#define EXTI_RTSR ((volatile unsigned int *) (EXTI_BASE+0x8))
#define EXTI_FTSR ((volatile unsigned int *) (EXTI_BASE+0xC))
#define EXTI_PR ((volatile unsigned int *) (EXTI_BASE+0x14))

#define VECTOR_TABLE_BASE 0x2001C000
#define EXTI3_IRQVEC ((void (**)(void)) (VECTOR_TABLE_BASE+0x64))
#define EXTI2_IRQVEC ((void (**)(void)) (VECTOR_TABLE_BASE+0x60))
#define EXTI1_IRQVEC ((void (**)(void)) (VECTOR_TABLE_BASE+0x5C))
#define EXTI0_IRQVEC ((void (**)(void)) (VECTOR_TABLE_BASE+0x58))

#define NVIC_EXTI3_IRQ_BPOS (1<<9)
#define NVIC_EXTI2_IRQ_BPOS (1<<8)
#define NVIC_EXTI1_IRQ_BPOS (1<<7)
#define NVIC_EXTI0_IRQ_BPOS (1<<6)

#define EXIT3_IRQ_BPOS (1<<3)
#define EXIT2_IRQ_BPOS (1<<2)
#define EXIT1_IRQ_BPOS (1<<1)
#define EXIT0_IRQ_BPOS (1<<0)

#define SCB_VTOR ((volatile unsigned int *) (0xE000ED00+0x8))

#define NVIC_BASE 0xE000E100
#define NVIC_ISER0 ((volatile unsigned int *) NVIC_BASE)
 
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

void interrupt_handler_exti0(void)
{
    *EXTI_PR |= EXIT0_IRQ_BPOS;
    reset_irq(0);
    count++;
    GPIO_D.ODR_LOW = count;
}

void interrupt_handler_exti1(void)
{
    *EXTI_PR |= EXIT1_IRQ_BPOS;
    reset_irq(1);
    count = 0;
    GPIO_D.ODR_LOW = count;
}

void interrupt_handler_exti2(void)
{
   *EXTI_PR |= EXIT2_IRQ_BPOS;
   reset_irq(2);
   GPIO_D.ODR_LOW = GPIO_D.ODR_LOW == 0 ? 0xFF : 0x00;
}

void app_init(void)
{
    // sätt D0-7 till utport
    GPIO_D.MODER = 0x00005555;
    GPIO_E.MODER = 0x00001500;
    
    // koppla PEx till EXTIx
    *SYSCFG_EXTICR1 &= ~0xFFF;
    *SYSCFG_EXTICR1 |= 0x444;
    
    // konfigurera EXTIx för avbrott
    *EXTI_IMR |=    EXIT0_IRQ_BPOS |
                    EXIT1_IRQ_BPOS |
                    EXIT2_IRQ_BPOS;
    // konfigurera avbrott på positiv flank
    *EXTI_RTSR |=   EXIT0_IRQ_BPOS |
                    EXIT1_IRQ_BPOS |
                    EXIT2_IRQ_BPOS;
                    
    *EXTI_FTSR &= ( EXIT0_IRQ_BPOS |
                    EXIT1_IRQ_BPOS |
                    EXIT2_IRQ_BPOS );
    
    *EXTI0_IRQVEC = interrupt_handler_exti0;
    *EXTI1_IRQVEC = interrupt_handler_exti1;
    *EXTI2_IRQVEC = interrupt_handler_exti2;
    
    // flytta på avbrottsvektor
    *SCB_VTOR = VECTOR_TABLE_BASE;
    
    // konfigurera NVIC
    *NVIC_ISER0 |= NVIC_EXTI0_IRQ_BPOS |
                   NVIC_EXTI1_IRQ_BPOS |
                   NVIC_EXTI2_IRQ_BPOS;
}

void main(void)
{
    app_init();
    while(1);
}
