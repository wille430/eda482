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

void main(void) {
  char a = 107;
  char b = 183;
  char c = 158;
  a = c * b;
  b = a * c;
  a = b ^ c;
  b = a | c;
  a = c + b;
  b = a + c;
  c = a * b;
  c = a * b;
  b = c * a;
  a = c - b;
  a += b;
  a += c;
  char result = (char)(a & 0xFF);
}

