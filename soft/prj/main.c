/*****************************************************************************
  * @file:    	main.c
  * @author:  	louiswoo
  * @version: 	V1.0
  * @date:	2020-6-6
  * @brief:	�����򣬳�ʼ��ϵͳ����ģ��͸�������
	while(1),ִ�и���������������ִ�е�ʱ����Ҫ���ƺ��Լ���
	ʱ������������ƺ������ִ��ʱ�䣬��Ҫ��ʱִ��
*******************************************************************************/
#include "config.h"
#include "sys_tick.h"
#include "debug.h"
#include "humidity.h"


void main(void)
{
	EA=0;
	sys_tick_init();
	UsartInit();
	task_HumidityInit();
	dog_init();
	
	EA=1;

	while(1)
	{
		task0_Clock();
		task1_Humidity();
		dog_clear();
		sys_idle();
	}
}