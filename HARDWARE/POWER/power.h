#ifndef __POWER_H
#define __POWER_H	 
#include "sys.h"

//180
#define VOL_5V_S  PBout(13)// PB13  控制  串口1附近5V电源   Shang
#define VOL_12V_S PBout(14)// PB14  控制  串口1附近12V电源  Shang
#define VOL_5V_X  PAout(1) // PA1   控制  电源附近 5V电源   Xia
#define VOL_12V_X PAout(4) // PA4   控制  电源附近 12V电源  Xia
//280
#define VOL_5V_UART5 PBout(9) // PA4   控制  电源附近 12V电源  Xia

void POWER_Init(void);//初始化

		    
#endif
