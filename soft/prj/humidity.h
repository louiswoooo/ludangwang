#ifndef	_HUMIDITY_H_
#define _HUMIDITY_H_


#include "config.h"


#define SENSE_pin P32
#define DAT_pin		P10
#define RLY_pin 	P33

#define ValveOn()		RLY_pin = 1
#define ValveOff()		RLY_pin = 0

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

#define LED0_on()		LED0_pin = 0;
#define LED0_off()	LED0_pin = 1;
#define LED1_on()		LED1_pin = 0;
#define LED1_off()	LED1_pin = 1;
#define LED2_on()		LED2_pin = 0;
#define LED2_off()	LED2_pin = 1;
#define LED3_on()		LED3_pin = 0;
#define LED3_off()	LED3_pin = 1;
#define LED4_on()		LED4_pin = 0;
#define LED4_off()	LED4_pin = 1;
#define LED5_on()		LED5_pin = 0;
#define LED5_off()	LED5_pin = 1;
#define LED6_on()		LED6_pin = 0;
#define LED6_off()	LED6_pin = 1;
#define LED7_on()		LED7_pin = 0;
#define LED7_off()	LED7_pin = 1;
#define LED8_on()		LED8_pin = 0;
#define LED8_off()	LED8_pin = 1;
#define LED9_on()		LED9_pin = 0;
#define LED9_off()	LED9_pin = 1;

extern u8 task_humidity_timer;
void task1_Humidity(void);
void task_HumidityInit(void);

#endif