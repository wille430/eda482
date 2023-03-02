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
typedef enum USART_BITS { B_UE = (1<<13), B_TE = (1<<3), B_RE = (1<<2), B_TXE = (1<<7), B_RXNE = (1<<5), B_RXNEIE = (1<<5)};

#define NVIC_BASE  0xE000E100
#define NVIC_ISER1 ((volatile unsigned int *) (NVIC_BASE+0x004))
#define NVIC_USART1_IRQ_BPOS (1<<5)

#define SCB_VTOR      ((volatile unsigned int *) 0xE000ED08)
#define USART1_IRQVEC ((void (**)(void)) 0x2001C0D4)


char inbuf;

void _outchar(char c)
{
	while (( USART1->sr & B_TXE ) == 0);

	USART1->dr = (unsigned short) c;
}

void usart_irq_routine(void)
{
	if (USART1->sr & B_RXNE)
		inbuf = (char) USART1->dr;
}

char usart_tstchar(void)
{
	char c = inbuf;
	inbuf = 0;
	return c;
}

void usart_init(void)
{
	// flytta på avbrottsvektorn
	*SCB_VTOR = 0x2001C000;

	// aktivera avbrott för usart1
	*NVIC_ISER1 |= NVIC_USART1_IRQ_BPOS;

	// sätt avbrottsrutin för avbrott från usart1
	*USART1_IRQVEC = usart_irq_routine;

	inbuf = 0;
	// sätt baudrate till 115200
	USART1->brr = 0x2D9;

	// sätt antalet stoppbitar
	USART1->cr2 = 0;

	USART1->cr3 = 0;
	
	// aktivera kresten, bestäm ramens längd och antal databitar
	USART1->cr1 |= B_UE | B_TE | B_RE;
}

void main(void)
{
	char c;
	usart_init();
	while (1)
	{
		c = usart_tstchar();

		if (c)
			_outchar(c);
	}
}

