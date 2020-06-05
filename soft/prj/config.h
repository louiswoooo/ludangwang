#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "stc12c2052ad.h"
#include "intrins.h"


typedef unsigned char u8;
typedef unsigned char u16;
typedef signed char s8;
typedef signed char s16;

void delay_10us(u8 count);
void delay_ms(u8 ms);

#endif