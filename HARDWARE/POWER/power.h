#ifndef __POWER_H
#define __POWER_H	 
#include "sys.h"

//180
#define VOL_5V_S  PBout(13)// PB13  ����  ����1����5V��Դ   Shang
#define VOL_12V_S PBout(14)// PB14  ����  ����1����12V��Դ  Shang
#define VOL_5V_X  PAout(1) // PA1   ����  ��Դ���� 5V��Դ   Xia
#define VOL_12V_X PAout(4) // PA4   ����  ��Դ���� 12V��Դ  Xia
//280
#define VOL_5V_UART5 PBout(9) // PA4   ����  ��Դ���� 12V��Դ  Xia

void POWER_Init(void);//��ʼ��

		    
#endif