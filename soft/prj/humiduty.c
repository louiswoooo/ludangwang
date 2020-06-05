#include "config.h"
#include "debug.h"
#include "humidity.h"

#define ADC_POWER_BIT	0x80
#define ADC_FLAG_BIT	0x10
#define ADC_START_BIT	0x08
#define ADC_SPEEDLL_BIT		0x00		//1080 clocks
#define ADC_SPEEDL_BIT		0x20		//810 clocks
#define ADC_SPEEDLH_BIT		0x40		//540 clocks
#define ADC_SPEEDHHBIT		0x50		//270 clocks

#define HUMI_ADC_CH	0x00

u8 task_humidity_timer;

void task_HumidityInit(void)
{
	P1M0 |=0x01;
	P1M1 |=0x01;
	DAT_pin = 1;
	ADC_DATA = 0;
	ADC_CONTR =ADC_POWER_BIT | ADC_SPEEDLL_BIT;
	task_humidity_timer = TASK_HUMIDITY_TIMER;
	
}

void HumiDisplay(u8 humi)
{
	if(humi>=0 && humi <=9)
	{
		LED9_off();
		LED8_off();
		LED7_off();
		LED6_off();
		LED5_off();
		LED4_off();
		LED3_off();
		LED2_off();
		LED1_off();
		LED0_off();
		switch(humi)
		{
			case 9:
				LED9_on();
			case 8:
				LED8_on();
			case 7:
				LED7_on();
			case 6:
				LED6_on();
			case 5:
				LED5_on();
			case 4:
				LED4_on();
			case 3:
				LED3_on();
			case 2:
				LED2_on();
			case 1:
				LED1_on();
			case 0:
				LED0_on();
			default:
				break;
		}
	}
}

u8 HumiGet(void)
{
	ADC_CONTR = ADC_POWER_BIT | ADC_SPEEDLL_BIT	| HUMI_ADC_CH | ADC_START_BIT;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	while(!(ADC_CONTR & ADC_FLAG_BIT));
	ADC_CONTR &= ~ADC_FLAG_BIT;
	
	return ADC_DATA;
}

void task1_Humidity(void)
{
	u8 humi_grade, humi_sense;
	if(task_humidity_timer == 0)
	{
		task_humidity_timer=TASK_HUMIDITY_TIMER;

		if(SENSE_pin == 1)
			humi_sense = 1;
		else
			humi_sense = 0;
		//humi_grade = (u8)((10*HumiGet())/0xff + 0.5);
		debug_var((u16)(humi_grade));
		debug(":");
		debug_var((u16)(humi_sense));
		debug("\r\n");
		HumiDisplay(humi_grade);
	}
}