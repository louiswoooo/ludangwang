#ifndef _DEBUG_H_
#define _DEBUG_H_
#include "config.h"

#define __DEBUG__
#ifdef __DEBUG__
	#define debug(x)		UsartSendString(x)
	#define debug_var(var)			UsartSendVar(var)
#else
	#define debug(x)	
	#define debug_var(var)
#endif

void UsartInit(void);		//9600bps@6.000MHz
void UsartSendString(u8 *str);
void UsartSendVar(u16 var);

#endif