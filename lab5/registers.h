
#ifndef REGISTERS_H
#define REGISTERS_H

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

typedef struct
{
    unsigned int ctrl;
    unsigned int load;
    unsigned int val;
    unsigned int calib;
} SYSTICK;

#endif