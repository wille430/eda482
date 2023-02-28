
#include "registers.h"


// configures for pins x8-15, where x depends on gpio pointer
void init_keypad(PGPIO gpio);
unsigned char keyb(void);