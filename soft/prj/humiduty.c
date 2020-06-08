/*****************************************************************************
  * @file:    	humidity.c
  * @author:  	louiswoo
  * @version: 	V1.0
  * @date:	2020-6-6
  * @brief:	湿度模块，
*******************************************************************************/
#include "config.h"
#include "debug.h"
#include "humidity.h"

#define TASK_HUMIDITY_TIMER	100			//湿度任务调兜娜时间片定时值
u8 task_humidity_timer;				//湿度任务计数器，初始设定为定时值

#define Valve_ON_THRESHHOLD		3			//定义阀门打开的下限，小于3个灯打开
#define Valve_OFF_THRESHHOLD	6			//定义阀门关闭的上限，大于8个灯关闭

#define ADC_POWER_BIT	0x80		
#define ADC_FLAG_BIT	0x10
#define ADC_START_BIT	0x08
#define ADC_SPEEDLL_BIT		0x00		//1080 clocks
#define ADC_SPEEDL_BIT		0x20		//810 clocks
#define ADC_SPEEDLH_BIT		0x40		//540 clocks
#define ADC_SPEEDHHBIT		0x50		//270 clocks

#define HUMI_ADC_CH	0x00

u8 ValveStatus;
/****************************************************************************************
  * @brief:	湿度任务初始化
  * @param:	none
  * @retval:	none
*****************************************************************************************/

void task_HumidityInit(void)
{
	P3M0 &= ~0x08;	//P33强推挽0
	P3M1 |=	0x08;	//1
	RLY_pin = 0;
	
	P1M0 |=0x01;				//设置adc采集口为高阻态
	P1M1 |=0x01;
	DAT_pin = 1;
	ADC_DATA = 0;
	ADC_CONTR =ADC_POWER_BIT | ADC_SPEEDLL_BIT;
	task_humidity_timer = TASK_HUMIDITY_TIMER;
	ValveStatus = 0;
	ValveOff();
	
}
/****************************************************************************************
  * @brief:	把湿度指示灯全灭
  * @param:	none
  * @retval:	none
*****************************************************************************************/

void LEDAllOff(void)
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
}
/****************************************************************************************
  * @brief:	根据湿度变量0~10，调整指示灯显示
  * @param:	humi
  * @retval:	none
*****************************************************************************************/

void HumiDisplay(u8 humi)
{
	LEDAllOff();
	if(humi>=0 && humi <=10)
	{
		switch(humi)
		{
			case 10:
				LED9_on();
			case 9:
				LED8_on();
			case 8:
				LED7_on();
			case 7:
				LED6_on();
			case 6:
				LED5_on();
			case 5:
				LED4_on();
			case 4:
				LED3_on();
			case 3:
				LED2_on();
			case 2:
				LED1_on();
			case 1:
				LED0_on();
				break;
			default:
				break;
		}
	}
}
/****************************************************************************************
  * @brief:	根据湿度变量0~10，控制阀门打开关闭
  * @param:	humi
  * @retval:	成功1，失败0
*****************************************************************************************/
u8	ValveControl(u8 humi)
{
	if(humi > Valve_OFF_THRESHHOLD)
	{
		ValveOff();
		ValveStatus = 0;
		return 1;
	}
	else if(humi < Valve_ON_THRESHHOLD)
	{
		ValveOn();
		ValveStatus = 1;
		return 1;
	}
	else
	{
		return 0;
	}
}
/****************************************************************************************
  * @brief:	通过HUMI_ADC_CH 进行AD测量，获取湿度数据
  * @param:	none
  * @retval:	ADC数据
*****************************************************************************************/

u8 HumiGet(void)
{
	ADC_CONTR = ADC_POWER_BIT | ADC_SPEEDLL_BIT	| HUMI_ADC_CH | ADC_START_BIT;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	while(!(ADC_CONTR & ADC_FLAG_BIT));			//等待adc转换完成
	ADC_CONTR &= ~ADC_FLAG_BIT;					//清adc标志
	
	return ADC_DATA;
}
/****************************************************************************************
  * @brief:	湿度任务，时间片计数器到时间后执行任务，
	获取湿度信息，打印，显示湿度指示灯
  * @param:	none
  * @retval:	
*****************************************************************************************/
void task1_Humidity(void)
{
	u8 humi_grade, humi_sense;
	u8 dat;
	if(task_humidity_timer == 0)		//如果计数器到时间执行任务
	{
		task_humidity_timer=TASK_HUMIDITY_TIMER;		//重新装载计数器

		if(SENSE_pin == 1)
			humi_sense = 1;
		else
			humi_sense = 0;
		dat = 0xff-HumiGet();
		humi_grade = (u8)((10*dat)/0xff + 0.5);			//计算湿度等级
		debug("		当前湿度：");
		debug_var((u16)(dat));
		if(humi_sense)
			debug("		快干死了，开阀\r\n");
		else
			debug("		水量充足，不开阀\r\n");
		HumiDisplay(humi_grade);								//显示湿度灯
		ValveControl(humi_grade);
		
	}
}