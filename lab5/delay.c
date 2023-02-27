
#define STK_BASE 0xE000E010
#define STK_CTRL ((volatile unsigned int *) STK_BASE)
#define STK_LOAD ((volatile unsigned int *) (STK_BASE+4))
#define STK_VAL ((volatile unsigned int *) (STK_BASE+8))

void delay_250ns(void)
{
    *STK_CTRL = 0;
    *STK_LOAD = 168/4;
    *STK_VAL = 0;
    *STK_CTRL = 5;
    
    while ((*STK_CTRL & 0x10000) == 0);
    
    *STK_CTRL = 0;
}

void delay_micro(unsigned int us) {
    while (us > 0)
    {
        delay_250ns();
        delay_250ns();
        delay_250ns();
        delay_250ns();
        us--;
    }
}

void delay_milli(unsigned int ms) {
    
#ifdef SIMULATOR
    ms /= 1000;
    ms++;
#endif
    
    delay_micro(ms * 1000);
}