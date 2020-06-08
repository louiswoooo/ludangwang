/*****************************************************************************
  * @file:    	sys_tick.c
  * @author:  	louiswoo
  * @version: 	V1.0
  * @date:	2020-6-6
  * @brief:	系统时间片模块，为整个系统提供时间片，
	本项目使用11.0592Mhz外部晶振，定时器t0每隔10ms产生一次中断，
	中断函数会对每个任务时间片计数器-1，从而驱动任务调度
	每秒经历100个时间片，这些时间片都可以供任务进行调度
	另外还提供了一个时间任务供调用
*******************************************************************************/
#include "config.h"
#include "debug.h"
#include "humidity.h"


#define TICK_PER_SECOND	100			//定义每秒的时间片sys_tick数
u8 sys_tick;										//时间片计数器
u8 second,minute,hour;					//时钟任务变量

/****************************************************************************************
  * @brief:	初始化时间片定时器t0，初始化时钟任务变量 
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
  * @brief:	时间片定时器中断服务程序，每个任务计数器-1
  * @param:	none
  * @retval:	none
*****************************************************************************************/
void sys_tick_int(void) interrupt 1
{
	TR0=0;
	TL0 = 0x00;		//??????
	TH0 = 0xDC;		//??????	
	TR0=1;

	/*此处每个时间片终端对每个任务时间片计数器清零
	有多少个任务就需要对多少个任务时间片计数器清零*/
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
  * @brief:	时钟任务，每隔1s打印时间
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
		debug("时间：");
		debug_var((u16)hour);
		debug(":");
		debug_var((u16)minute);
		debug(":");
		debug_var((u16)second);
//		debug("\r\n");
	}
}
//进入空闲
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

//看门狗配置
//使能，清看门狗，空闲计数
void dog_init(void)
{
	WDT_CONTR =0x00;
	WDT_CONTR = DOG_ENABLE_MASK | DOG_CLEAR_MASK | DOG_IDLECOUNT_MASK;
	WDT_CONTR |= DOG_PRESCALER;
	
}
//清看门狗
void dog_clear(void)
{
	WDT_CONTR |= DOG_CLEAR_MASK;
}
