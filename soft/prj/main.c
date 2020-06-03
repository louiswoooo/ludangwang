#include "config.h"

u8 sys_tick;
u8 second,minute,hour;

void Timer0Init(void)		//10ms@6.000MHz
{
	AUXR |= 0x80;		//?????1T??
	TMOD &= 0xF0;		//???????
	TMOD |= 0x01;		//???????
	TL0 = 0xA0;		//??????
	TH0 = 0x15;		//??????
	TF0 = 0;		//??TF0??
	TR0 = 1;		//???0????
	
	ET0=1;
}

void UartInit(void)		//9600bps@6.000MHz
{
	PCON |= 0x80;		//????????SMOD
	SCON = 0x50;		//8???,?????
	AUXR |= 0x40;		//???1???Fosc,?1T
	AUXR &= 0xFE;		//??1?????1???????
	TMOD &= 0x0F;		//?????1???
	TMOD |= 0x20;		//?????1?8???????
	TL1 = 0xD9;		//??????
	TH1 = 0xD9;		//????????
	ET1 = 0;		//?????1??
	TR1 = 1;		//?????1
}
void sys_clock_int(void) interrupt 1
{
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

void UsartSendString(u8 *str)
{
	while(*str)
	{
		SBUF = *str;
		while(!TI)	;
		TI=0;
		str++;
	}
}

void Debug_Var(u16 var)
{
	u8 num[6];
	num[0]=var/10000+0x30;
	num[1]=var%10000/1000+0x30;
	num[2]=var%1000/100+0x30;
	num[3]=var%100/10+0x30;
	num[4]=var%10+0x30;
	num[5]='\0';
	debug(num);
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

void main(void)
{
	EA=0;
	Timer0Init();
	UartInit();
	
	sys_tick = TICK_PER_SECOND;
	second = 0;
	minute = 0;
	hour = 0;
	EA=1;

	while(1)
	{
		task0_Clock();
	}
}