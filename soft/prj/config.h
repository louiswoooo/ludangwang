#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "stc12c2052ad.h"

#define __DEBUG__
#ifdef __DEBUG__
	#define debug(x)		UsartSendString(x)
#else
	#define debug(x)	
#endif
#define debug_var(var)			Debug_Var(var)

#define SENSE_pin P32
#define DAT_pin		P10
#define RLY_pin 	P33

#define LED0_pin	P35
#define LED1_pin	P34
#define LED2_pin	P37
#define LED3_pin	P11
#define LED4_pin	P12
#define LED5_pin	P13
#define LED6_pin	P14
#define LED7_pin	P15
#define LED8_pin	P16
#define LED9_pin	P17

#define TICK_PER_SECOND	100

typedef unsigned char u8;
typedef unsigned char u16;
typedef signed char s8;
typedef signed char s16;

#endif