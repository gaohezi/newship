#include "power.h"
//初始化电压控制的IO口
/*
	//180
	PB13 控制  串口1附近5V电源
	PB14 控制  串口1附近12V电源
	PA1  控制  电源附近 5V电源
	PA4  控制  电源附近 12V电源
	//280
	PB9  控制  电源UART5 5V电源
*/
void POWER_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_4;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_SetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_1);						 //PB.5 输出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14 | GPIO_Pin_9;	    		 //LED1-->PE.5 端口配置, 推挽输出
 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14 | GPIO_Pin_9); 						 //PE.5 输出高 
}

