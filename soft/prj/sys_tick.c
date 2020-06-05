#include "config.h"
#include "debug.h"
#include "humidity.h"

#define TICK_PER_SECOND	100
u8 sys_tick;
u8 second,minute,hour;


void sys_tick_init(void)		//10ms@6.000MHz
{
	AUXR |= 0x80;		//?????1T??
	TMOD &= 0xF0;		//???????
	TMOD |= 0x01;		//???????
	TL0 = 0x74;		//??????
	TH0 = 0x20;		//??????
	TF0 = 0;		//??TF0??
	TR0 = 1;		//???0????
	
	ET0=1;
	
	sys_tick = TICK_PER_SECOND;
	second = 0;
	minute = 0;
	hour = 0;
}

void sys_tick_int(void) interrupt 1
{
	TR0=0;
	TL0 = 0x7a;		//??????
	TH0 = 0x20;		//??????
	TR0=1;

	if(task_humidity_timer>0)
	{
		task_humidity_timer--;
	}
	

	if(sys_tick>0)
	{
		sys_tick--;
	}
	else
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
	}
}

void task0_Clock(void)
{
	if(sys_tick == 0)
	{
		debug_var((u16)hour);
		debug(":");
		debug_var((u16)minute);
		debug(":");
		debug_var((u16)second);
		debug("\r\n");
	}
}
