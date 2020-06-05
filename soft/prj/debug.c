#include "config.h"
#include "debug.h"

void UsartInit(void)		//9600bps@6.000MHz
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

void UsartSendVar(u16 var)
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