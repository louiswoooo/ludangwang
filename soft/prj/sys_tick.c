/*****************************************************************************
  * @file:    	sys_tick.c
  * @author:  	louiswoo
  * @version: 	V1.0
  * @date:	2020-6-6
  * @brief:	ϵͳʱ��Ƭģ�飬Ϊ����ϵͳ�ṩʱ��Ƭ��
	����Ŀʹ��11.0592Mhz�ⲿ���񣬶�ʱ��t0ÿ��10ms����һ���жϣ�
	�жϺ������ÿ������ʱ��Ƭ������-1���Ӷ������������
	ÿ�뾭��100��ʱ��Ƭ����Щʱ��Ƭ�����Թ�������е���
	���⻹�ṩ��һ��ʱ�����񹩵���
*******************************************************************************/
#include "config.h"
#include "debug.h"
#include "humidity.h"


#define TICK_PER_SECOND	100			//����ÿ���ʱ��Ƭsys_tick��
u8 sys_tick;										//ʱ��Ƭ������
u8 second,minute,hour;					//ʱ���������

/****************************************************************************************
  * @brief:	��ʼ��ʱ��Ƭ��ʱ��t0����ʼ��ʱ��������� 
  * @param:	none
  * @retval:	none
*****************************************************************************************/
void sys_tick_init(void)		//10ms@11.0592MHz
{
	AUXR &= 0x7F;		//?????12T??
	TMOD &= 0xF0;		//???????
	TMOD |= 0x01;		//???????
	TL0 = 0x00;		//??????
	TH0 = 0xDC;		//??????
	TF0 = 0;		//??TF0??
	TR0 = 1;		//???0????	
	ET0=1;
	
	sys_tick = TICK_PER_SECOND;
	second = 0;
	minute = 0;
	hour = 0;
}

/****************************************************************************************
  * @brief:	ʱ��Ƭ��ʱ���жϷ������ÿ�����������-1
  * @param:	none
  * @retval:	none
*****************************************************************************************/
void sys_tick_int(void) interrupt 1
{
	TR0=0;
	TL0 = 0x00;		//??????
	TH0 = 0xDC;		//??????	
	TR0=1;

	/*�˴�ÿ��ʱ��Ƭ�ն˶�ÿ������ʱ��Ƭ����������
	�ж��ٸ��������Ҫ�Զ��ٸ�����ʱ��Ƭ����������*/
	if(task_humidity_timer>0)		
	{
		task_humidity_timer--;
	}
	if(sys_tick>0)
	{
		sys_tick--;
	}
}
/****************************************************************************************
  * @brief:	ʱ������ÿ��1s��ӡʱ��
  * @param:	none
  * @retval:	none
*****************************************************************************************/
void task0_Clock(void)
{
	if(sys_tick == 0)
	{
		sys_tick=TICK_PER_SECOND;
		second++;
		if(second>=60)
		{
			second=0;
			minute++;
			if(minute>=60)
			{
				minute=0;
				hour++;
			}
		}
		debug("ʱ�䣺");
		debug_var((u16)hour);
		debug(":");
		debug_var((u16)minute);
		debug(":");
		debug_var((u16)second);
//		debug("\r\n");
	}
}
//�������
void sys_idle(void)
{
	PCON = 0x01;
}

#define DOG_FLAG_MASK			0x80
#define DOG_ENABLE_MASK		0x20
#define	DOG_CLEAR_MASK		0x10
#define	DOG_IDLECOUNT_MASK 0x08
#define DOG_PRESCALER_MASK			0xf8
#define DOG_PRESCALER			6

//���Ź�����
//ʹ�ܣ��忴�Ź������м���
void dog_init(void)
{
	WDT_CONTR =0x00;
	WDT_CONTR = DOG_ENABLE_MASK | DOG_CLEAR_MASK | DOG_IDLECOUNT_MASK;
	WDT_CONTR |= DOG_PRESCALER;
	
}
//�忴�Ź�
void dog_clear(void)
{
	WDT_CONTR |= DOG_CLEAR_MASK;
}
