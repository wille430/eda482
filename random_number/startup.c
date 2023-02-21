/*
 * 	startup.c
 *
 */
__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

#define B_UDIS    (1<<1)
#define B_CEN     (1<<0)

typedef volatile struct {
    union {
        unsigned int CR1;
        struct {
            unsigned int CEN:1;
            unsigned int UDIS:1;
            unsigned int URS:1;
            unsigned int OPM:1;
            unsigned int :3;
            unsigned int ARPE:1;
        } CR1_BITS;
    };
    unsigned int CR2;
    unsigned int reserved_1;
    unsigned int DIER;
    unsigned int SR;
    unsigned int EGR;
    unsigned int reserved_2;
    unsigned int reserved_3;
    unsigned int reserved_4;
    unsigned short CNT;
    unsigned short reserved_5;
    unsigned short PSC;
    unsigned short reserved_6;
    unsigned short ARR;
    unsigned short reserved_7;
} TIMX, *PTIMX;

typedef volatile struct {
    unsigned int   MODER;
    unsigned short OTYPER;
    unsigned short OTYPER_RESERVED;
    unsigned int   OSPEEDR;
    unsigned int   PUPDR;
    union {
        unsigned short IDR;
        struct {
                unsigned char IDR_LOW;
                unsigned char IDR_HIGH;
        };
    };
    unsigned short IDR_RESERVED;
    union {
        unsigned short ODR;
        struct {
                unsigned char ODR_LOW;
                unsigned char ODR_HIGH;
        };
    };
    unsigned short ODR_RESERVED;
    unsigned int   BSRR;
    unsigned short LCKR;
    unsigned int   LCK:1;
    unsigned int   :7;
    unsigned int   AFRL;
    unsigned int   AFRH;
} GPIO, *PGPIO;

#define GPIO_D (*((volatile PGPIO) 0x40020C00))
#define TIM6 (*((volatile PTIMX) 0x40001000))

#define TIM6_CR1 ((volatile unsigned short *) 0x40001000)
#define TIM6_CNT ((volatile unsigned short *) 0x40001024)
#define TIM6_ARR ((volatile unsigned short *) 0x4000102C)

void timer6_init(void)
{
    *TIM6_CR1 &= ~B_CEN;
    *TIM6_ARR  = 0xFFFF;
    GPIO_D.ODR_LOW = *TIM6_ARR;
    *TIM6_CR1 |= (B_UDIS | B_CEN);

    /*
    TIM6.ARR  = 0xFFFF;
    GPIO_D.ODR_LOW = TIM6.ARR;
    TIM6.CR1 |= (B_CEN | B_UDIS);
     */
}

void gpio_init(void)
{
    GPIO_D.MODER = 0x00005555;
}

void main(void)
{
    char random = 0;
    gpio_init();
    timer6_init();
    
    while (1)
    {
        random = (char) *TIM6_CNT;
        GPIO_D.ODR_LOW = random;
    }
}

