
#define GPIO_BASE 0x40021000
#define GPIO_MODER ((volatile unsigned int *) (GPIO_BASE+0x0))
#define GPIO_OTYPER ((volatile unsigned short *) (GPIO_BASE+0x4))
#define GPIO_PUPDR ((volatile unsigned int *) (GPIO_BASE+0xC))
#define GPIO_ODR ((volatile unsigned short *) (GPIO_BASE+0x14))

#define EXTI_BASE 0x40013C00
#define EXTI_IMR ((volatile unsigned short *) (EXTI_BASE+0x0))
#define EXTI_RTSR ((volatile unsigned short *) (EXTI_BASE+0x8))
#define EXTI_FTSR ((volatile unsigned short *) (EXTI_BASE+0xC))
#define EXTI_PR ((volatile unsigned short *) (EXTI_BASE+0x14))

#define SYSCFG_BASE 0x40013800
#define SYSCFG_EXTICR3 ((volatile unsigned short *) (SYSCFG_BASE+0x10))

#define B_EXTI8 (1<<8);

#define NVIC_BASE 0xE000E100
#define NVIC_ISER0 ((volatile unsigned int *) (NVIC_BASE+0x000))
#define B_NVIC_EXTI9_5 (1<<23)

#define SCB_BASE ((unsigned int) 0xE000ED00)
#define SCB_VTOR ((volatile unsigned int *) (SCB_BASE+0x8))
#define VEC_TABLE_ADR ((unsigned int) 0x2001C0000) // delbart med 512

void at_interrupt(void)
{
    if (*EXTI_PR & (1<<8))
    {
        *GPIO_ODR |= (1<<7);
        *GPIO_ODR &= ~(1<<7);
        *EXTI_PR |= B_EXTI8; //kvitera avbrott
    }
}

void init(void)
{
    // behöver plugga mer på vad som är utport vs inport
    *GPIO_MODER &= ~0x3C000;
    *GPIO_MODER |= 0x4000;

    *GPIO_OTYPER &= ~(1<<7);
    *GPIO_PUPDR &= ~0x3C000;

    // konfigurerar så att pe8 kan generara avbrott
    *SYSCFG_EXTICR3 &= ~0xF;
    *SYSCFG_EXTICR3 |= 0x4;

    *EXTI_IMR |= B_EXTI8; // aktivera avbrott för EXTI8
    *EXTI_RTSR &= ~B_EXTI8; // rising trigger
    *EXTI_FTSR |= B_EXTI8; // stäng av falling trigger
    
    // aktivera nvic för EXTI0
    *NVIC_ISER0 |= B_NVIC_EXTI9_5;

    *SCB_VTOR = VEC_TABLE_ADR;
    *((void (**)(void)) (VEC_TABLE_ADR+0x9C)) = at_interrupt;
}
