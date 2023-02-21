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

__attribute__((naked)) unsigned int getPsrReg(void)
{
    __asm__ volatile("  MRS   R0,PSR\n");
    __asm__ volatile("  BX    LR\n");
}

__attribute__((naked)) void setPsrReg(unsigned int apsr)
{
    __asm__ volatile("  MSR   PSR_nvzcq,R0\n");
    __asm__ volatile("  BX    LR\n");
}

void main(void)
{
    unsigned int psr;
    psr = getPsrReg();
    setPsrReg(0);
    psr = getPsrReg();
}

