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

typedef struct 
{
	volatile unsigned short sr;
	volatile unsigned short Unused0;
	volatile unsigned short dr;
	volatile unsigned short Unused1;
	volatile unsigned short brr;
	volatile unsigned short Unused2;
	volatile unsigned short cr1;
	volatile unsigned short Unsued3;
	volatile unsigned short cr2;
	volatile unsigned short Unused4;
	volatile unsigned short cr3;
	volatile unsigned short Unused5;
	volatile unsigned short gtpr;
} USART;

#define USART1 ((volatile USART *) 0x40011000)
typedef enum USART_BITS { B_UE = (1<<13), B_TE = (1<<3), B_RE = (1<<2), B_TXE = (1<<7), B_RXNE = (1<<5)};

#define NVIC_BASE  0xE000E100
#define NVIC_ISER1 ((volatile unsigned int) (NVIC_BASE+0x004))
#define NVIC_USART1_IRQ_CPOS (1<<5)

#define SCB_VTOR      ((volatile unsigned int *) 0xE000ED08)
#define USART1_IRQVEC ((void (**)(void)) 0x2001C0D4)

void _iniuart(void)
{
	// sätt baudrate till 115200
	// 115200 = 84 Mhz / (8 * (2 - 0) * USARTDIV)
	// 115200 * (8 * (2 - 0) * USARTDIV) = 84 Mhz
	// (16 * USARTDIV) = 84 Mhz / 115200
	// USARTDIV = 84 Mhz / 115200 / 16 = 45.57
	// DIV_Mantissa = 45 bas 10 = 2D bas 16
	// DIV_Fraction = 0.57 = ca 9/16 = 9 bas 16
	// BRR = 0x2D9
	USART1->brr = 0x2D9;

	// sätt antalet stoppbitar
	USART1->cr2 = 0;
	
	// aktivera kresten, bestäm ramens längd och antal databitar
	USART1->cr1 |= B_UE | B_TE | B_RE;
}

char _tstchar(void)
{
	if ( USART1->sr & B_RXNE )
		return (char) USART1->dr; 

	return 0;
}

void _outchar(char c)
{
	while (( USART1->sr & B_TXE ) == 0);

	USART1->dr = (unsigned short) c;
}

void main(void)
{
	char c;
	_iniuart();
	while (1)
	{
		c = _tstchar();
		if (c)
			_outchar(c);
	}
}

