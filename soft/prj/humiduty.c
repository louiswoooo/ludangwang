/*****************************************************************************
  * @file:    	humidity.c
  * @author:  	louiswoo
  * @version: 	V1.0
  * @date:	2020-6-6
  * @brief:	ʪ��ģ�飬
*******************************************************************************/
#include "config.h"
#include "debug.h"
#include "humidity.h"

#define TASK_HUMIDITY_TIMER	100			//ʪ�����������ʱ��Ƭ��ʱֵ
u8 task_humidity_timer;				//ʪ���������������ʼ�趨Ϊ��ʱֵ

#define Valve_ON_THRESHHOLD		3			//���巧�Ŵ򿪵����ޣ�С��3���ƴ�
#define Valve_OFF_THRESHHOLD	6			//���巧�Źرյ����ޣ�����8���ƹر�

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
  * @brief:	ʪ�������ʼ��
  * @param:	none
  * @retval:	none
*****************************************************************************************/

void task_HumidityInit(void)
{
	P3M0 &= ~0x08;	//P33ǿ����0
	P3M1 |=	0x08;	//1
	RLY_pin = 0;
	
	P1M0 |=0x01;				//����adc�ɼ���Ϊ����̬
	P1M1 |=0x01;
	DAT_pin = 1;
	ADC_DATA = 0;
	ADC_CONTR =ADC_POWER_BIT | ADC_SPEEDLL_BIT;
	task_humidity_timer = TASK_HUMIDITY_TIMER;
	ValveStatus = 0;
	ValveOff();
	
}
/****************************************************************************************
  * @brief:	��ʪ��ָʾ��ȫ��
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
  * @brief:	����ʪ�ȱ���0~10������ָʾ����ʾ
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
  * @brief:	����ʪ�ȱ���0~10�����Ʒ��Ŵ򿪹ر�
  * @param:	humi
  * @retval:	�ɹ�1��ʧ��0
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
  * @brief:	ͨ��HUMI_ADC_CH ����AD��������ȡʪ������
  * @param:	none
  * @retval:	ADC����
*****************************************************************************************/

u8 HumiGet(void)
{
	ADC_CONTR = ADC_POWER_BIT | ADC_SPEEDLL_BIT	| HUMI_ADC_CH | ADC_START_BIT;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	while(!(ADC_CONTR & ADC_FLAG_BIT));			//�ȴ�adcת�����
	ADC_CONTR &= ~ADC_FLAG_BIT;					//��adc��־
	
	return ADC_DATA;
}
/****************************************************************************************
  * @brief:	ʪ������ʱ��Ƭ��������ʱ���ִ������
	��ȡʪ����Ϣ����ӡ����ʾʪ��ָʾ��
  * @param:	none
  * @retval:	
*****************************************************************************************/
void task1_Humidity(void)
{
	u8 humi_grade, humi_sense;
	u8 dat;
	if(task_humidity_timer == 0)		//�����������ʱ��ִ������
	{
		task_humidity_timer=TASK_HUMIDITY_TIMER;		//����װ�ؼ�����

		if(SENSE_pin == 1)
			humi_sense = 1;
		else
			humi_sense = 0;
		dat = 0xff-HumiGet();
		humi_grade = (u8)((10*dat)/0xff + 0.5);			//����ʪ�ȵȼ�
		debug("		��ǰʪ�ȣ�");
		debug_var((u16)(dat));
		if(humi_sense)
			debug("		������ˣ�����\r\n");
		else
			debug("		ˮ�����㣬������\r\n");
		HumiDisplay(humi_grade);								//��ʾʪ�ȵ�
		ValveControl(humi_grade);
		
	}
}