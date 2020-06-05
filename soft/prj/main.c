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
	
	EA=1;

	while(1)
	{
		task0_Clock();
		task1_Humidity();
	}
}