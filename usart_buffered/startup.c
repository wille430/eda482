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

#define USART_BUFF_SIZE 64
typedef struct {
	unsigned char in;
	unsigned char out;
	unsigned char count;
	unsigned char buff[USART_BUFF_SIZE];
} STACK;

static STACK TxBuffer, RxBuffer;

#define USART1 ((volatile USART *) 0x40011000)
typedef enum USART_BITS {
	B_UE     = (1<<13),
	B_TE     = (1<<3),
	B_RE     = (1<<2),
	B_TXE    = (1<<7),
	B_RXNE   = (1<<5),
	B_RXNEIE = (1<<5),
	B_TXEIE  = (1<<7),
	B_TCIE   = (1<<6),
	B_TC  = (1<<7),
};

#define NVIC_BASE  0xE000E100
#define NVIC_ISER1 ((volatile unsigned int *) (NVIC_BASE+0x004))
#define NVIC_USART1_IRQ_BPOS (1<<5)

#define SCB_VTOR      ((volatile unsigned int *) 0xE000ED08)
#define USART1_IRQVEC ((void (**)(void)) 0x2001C0D4)

void buffer_init(STACK *buffer)
{
	buffer->in = 0;
	buffer->out = 0;
	buffer->count = 0;
}

char buffer_put(STACK *buffer, char c)
{
	if (buffer->count == USART_BUFF_SIZE) return 0;

	buffer->buff[buffer->in] = c;
	buffer->in++;
	buffer->count++;

	if (buffer->in == USART_BUFF_SIZE)
		buffer->in = 0;

	return 1;
}

char buffer_get(STACK *buffer, char * out)
{
	if (buffer->count == 0) return 0;

	*out = buffer->buff[buffer->out];
	buffer->count--;
    buffer->out++;
	if (buffer->out == USART_BUFF_SIZE)
		buffer->out = 0;

	return 1;
}



void usart_outchar(char c)
{
	buffer_put(&TxBuffer, c);
	USART1->cr1 |= B_TXEIE;
}

void usart_irq_routine(void)
{
	if ( (USART1->cr1 & B_TXEIE) != 0 && (USART1->sr & B_TXE) != 0)
	{
		char isNotEmpty = buffer_get(&TxBuffer, &USART1->dr);
        
        if (!isNotEmpty)
            USART1->cr1 &= ~B_TXEIE; // deaktivera abvrott f??r TXE
	}

	if (USART1->sr & B_RXNE)
		buffer_put(&RxBuffer, (char) USART1->dr);
}

char usart_tstchar(void)
{
	char c = 0;
	buffer_get(&RxBuffer, &c);
	return c;
}

void usart_init(void)
{
	// flytta p?? avbrottsvektorn
	*SCB_VTOR = 0x2001C000;

	// aktivera avbrott f??r usart1
	*NVIC_ISER1 |= NVIC_USART1_IRQ_BPOS;

	// s??tt avbrottsrutin f??r avbrott fr??n usart1
	*USART1_IRQVEC = usart_irq_routine;

	// s??tt baudrate till 115200
	USART1->brr = 0x2D9;

	// s??tt antalet stoppbitar
	USART1->cr2 = 0;

	USART1->cr3 = 0;
	
	// aktivera kresten, best??m ramens l??ngd och antal databitar
	USART1->cr1 |= B_UE | B_TE | B_RE;

	buffer_init(&RxBuffer);
	buffer_init(&TxBuffer);
}

void putstring(char *s)
{
	while (*s) usart_outchar(*s++);
}

void main(void)
{
	char c;
	usart_init();

	putstring("USART program");

	while (1)
	{
		c = usart_tstchar();

		if (c)
			usart_outchar(c);
	}
}
