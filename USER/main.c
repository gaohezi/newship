#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "usart3.h"
#include "timer.h"
#include "led.h"
#include "key.h"
#include "led.h" 
#include "math.h"			
#include "stdio.h"
#include "stm32f10x_flash.h"
#include "stdlib.h"
#include "string.h"
#include "wdg.h"
#include "stm32f10x_tim.h"
#include "ml302.h"	
#include "adc.h"	 
#include "remote.h"	
#include "power.h"	
#include "rtc.h" 
#include "flash.h" 
#include <stdio.h>
#include "exti.h"
#include "stdlib.h"
#include "sht31.h"
#include "uart5.h"
#include "gps.h"
#include "rs485.h"
#include "systick.h"

#define contorl_time2 	2
#define contorl_time5 	5
#define contorl_time10  10
bool mqttput_float=1;
extern	u8 data_Send_Time,GPS_Data_Send_Time;
//extern u8 	USART_RX_BUF[USART_REC_LEN]; 
//状态1：断电重启，遥控器未配对时为，采样值为0
//状态2：断电重启，遥控器配对时，采样值为正常值
//状态3：右通道为18500，左通道为18000时，设置为断电状态		
//左摇杆即CH2	采集中间值为 1469-1470-1471
//左摇杆即CH2	采集最大值（摇杆上推）为 1849-1850
//左摇杆即CH2	采集最小值（摇杆上推）为 1150-1149
//.
//左摇杆即CH2	采集中间值为 1456
//左摇杆即CH2	采集最大值（摇杆上推）为 1997-2000
//左摇杆即CH2	采集最小值（摇杆上推）为 999-1002

//右摇杆即CH3	采集中间值为 1508
//右摇杆即CH3	采集最大值（摇杆上推）为 1989-1990
//右摇杆即CH3	采集最小值（摇杆上推）为 1010-1011

//左通道 采集 TIM5CH1	PA0	数采仪A3
//右通道 采集 TIM3CH3	PB0	数采仪A6-
//PWM输出	D1
//PWM输出 D3

bool Timeflag = false;
extern bool MQTTFlag ;
bool send_data_float=0;//发送数据状态标志位

extern u16 temp3;
/*
TIM2作为PWM输出
*/
extern bool RemoteFlag;
bool  stop_flag=0;

//u32 TEXT_Buffer[200];
//#define SIZE sizeof(TEXT_Buffer)		//数组长度
//#define FLASH_SAVE_ADDR  0X08070000		//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)

extern char *strx,*extstrx;
extern char  RxBuffer[900]; 
extern u16 RxCounter;

//extern char UART5_RxBuffer[900];     //接收缓冲,最大USART_REC_LEN个字节.
//extern u32 UART5_RxCounter;

extern	float longitude;
extern	float latitude;
extern	float Speed;
extern	float gps;
extern	float beidou;
extern	float high;
extern	int gps_qty;//gps数量
extern	int bd_qty;//北斗数量
	u16 len=0;
/***
捕获状态参数标定
***/


	u16 	CH2_OUT=flycolor_ch2_stop;
	u16	  CH3_OUT=flycolor_ch3_stop;
	u8 time;

int main(void)
{				
	delay_init();	    	 //延时函数初始化	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级  
	LED_Init();			     //LED端口初始化
	POWER_Init();				 //电压控制初始化
	VOL_5V_S=1;			   //开启上部分5V	电压
	
/* IIC */	
	  IIC_Init();			//IIC初始化 
/*USART1*/	
	  uart_init(115200);	 //串口初始化为115200	打印到电脑调试助手 
/*JY901*/	
  	uart5_init(115200);
/* ADC */	
		Adc_Init();
/* 485 */	
		RS485_Init(9600);
//		TIM6_Init(); 
/*ML302*/		
		usart3_init(115200);//对接ML302
		
	IWDG_Init(7,625);    //8S一次    Tout=((4*2^prer)*rlr)/40

/***********************PWM输出*************************/

	TIM4_PWM_Init(19999,71);		//720000/2*72=5000

/***********************PWM采样*************************/	
	
	TIM3_Cap_Init(0XFFFF,72-1);	//以1Mhz的频率计数 
	TIM5_Cap_Init(0XFFFF,72-1);
			
/***********************控制ML302发送数据到MQTT服务器，可以使用mqtt.fx软件进行订阅*************************/

	ML302PWR_Init();//对ML302开机
 
	PWRKEY=1;//对ML302开机
	ML302_Init();//对设备初始化
	ML302_MQTTInit();//初始化MQTT参数

/***********************控制ML302发送数据到MQTT服务器，可以使用mqtt.fx软件进行订阅*************************/
/// TIM6_Int_Init(59999,11999);
 
  TIM6_Int_Init(9999,7199);//10Khz的计数频率,  延时1s == 1000 000us 
	TIM2_Int_Init(5999,7199);//10Khz的计数频率，计数到1000为10ms(必须放上面)
	//	TIM2_Int_Init(5999,11999);//Tout= ((arr+1)*(psc+1))/Tclk；arr：计数重装值，psc分频数，Tclk系统时钟频率，Tout一个周期的时间。
/********************解锁电调**********************/
/*电调第一次使用要设置量程，解锁两个电调步骤，先拉到最高油门，再拉到最低油门，这一步电机会动，一定要注意安全*/

/*监测船飞盈佳乐电调PWM*/
	TIM_SetCompare2(TIM4,flycolor_low);
	TIM_SetCompare3(TIM4,flycolor_low);	 
	delay_ms(1000);  //先延时
	TIM_SetCompare2(TIM4,flycolor_high);
	TIM_SetCompare3(TIM4,flycolor_high);//18800 最低占空比，1ms   18000最高占空比，2.0ms  18500 50%占空比，1.5ms（需要1  1.5  2）
	delay_ms(1000);		//再延时

/********************解锁电调**********************/		 
////10Khz的计数频率，计数到1000为10ms(必须放上面)
//【代办事宜】
/*1、找到buf问题关键，char与u32取值区别			*/
/*2、找到rxbuff中的数组长度，便于以后使用			*/
/*3、整理代码，仔细罗列每一个参数			*/
/*4、json转换部分代码编写			*/

/*	20220225		*/
/*1、物联网左右转弯存在问题		*/
/*2、左右电机吃水量不足，前进时会有偏移			*/
/*3、物联网有持续下发的现象，刷新页面，停止发送		*/
/*4、要做命令抢占，就			*/
	while(1)
	{
			IOT_contorl_status();		
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
			dective_contorl();

			
////				u1_printf("主函数中CH2_OUT：%d\r		CH3_OUT：%d\r\n		\r\n",CH2_OUT,CH3_OUT);	

			GPS_data ();
//			if(strstr((const char*)USART_RX_BUF,(const char*)"AT"))
//			{
//				a=0;
//				TIM_Cmd(TIM2,ENABLE);//开启定时器 这样就能实现你需要的功能了
//				Clear_USART1_Buffer();
//			}

		////	delay_ms(1000);

			TIM_SetCompare1(TIM4,CH2_OUT);//TIM2_CH1   右电机
			TIM_SetCompare3(TIM4,CH3_OUT);//TIM2_CH3   右电机	
			IWDG_Feed();//喂狗
			
	u1_printf("主函数中CH2_OUT：%d\r		CH3_OUT：%d\r\n		\r\n",CH2_OUT,CH3_OUT);		
	}

}
bool  Remote=0;
void dective_contorl(void)
{
 
	int data1,data2;

	u16 pect2_CH2,pect3_CH3;

	int ch2,ch3;				
	pect2_CH2=TIM3CH2_collect( );//百分比
	pect3_CH3=TIM3CH3_collect( );//百分比
	
			data1 =	abs(pect2_CH2	-	Remote_ch2_Med_Val);		//中间值	
			data2 =	abs(pect3_CH3	-	Remote_ch3_Med_Val);		//中间值
			
						//遥控器控制
						////遥控器通道2					（最小值）1000 1002		1457		1912 2000（最大值）
						////遥控器通道3（油门）	（最小值）1069 1120		1575  	2030 2068（最大值）
		///					u1_printf("pect2_CH2:%d		pect3_CH3:%d	\r\n",pect2_CH2,pect3_CH3);			
		if(pect2_CH2 < Remote_CH2_Max_Val && pect2_CH2	>	Remote_CH2_Min_Val && pect3_CH3	<	Remote_CH3_Max_Val && pect3_CH3	>	Remote_CH3_Min_Val)//遥控器采集值超出1120-2120之间
		{
				///u1_printf("--------------------------遥控器控制中------------------------\r\n");
			
			Remote=1;
							if(pect2_CH2 > Remote_CH2_Out_Max_Val)
								pect2_CH2 = Remote_CH2_Out_Max_Val;
							if(pect2_CH2 < Remote_CH2_Out_Min_Val)
								pect2_CH2 = Remote_CH2_Out_Min_Val;
												
							if(pect3_CH3 > Remote_CH3_Out_Max_Val)
								pect3_CH3 = Remote_CH3_Out_Max_Val;
							if(pect3_CH3 < Remote_CH3_Out_Min_Val)
								pect3_CH3 = Remote_CH3_Out_Min_Val;
			send_data_float=0;
			ch2	=	(pect2_CH2 - Remote_CH2_Out_Min_Val)*100/(Remote_CH2_Out_Max_Val - Remote_CH2_Out_Min_Val);	//百分比
///		u1_printf("ch2:%d		\r\n",ch2);
							
			ch3	=	(pect3_CH3 - Remote_CH3_Out_Min_Val)*100/(Remote_CH3_Out_Max_Val - Remote_CH3_Out_Min_Val);	//百分比
///		u1_printf("ch3:%d		\r\n",ch3);	
						
						CH2_OUT =	flycolor_high -(ch2*(flycolor_high-flycolor_low)/100);	
						CH3_OUT =	flycolor_high -(ch3*(flycolor_high-flycolor_low)/100);	
				
//			if (pect1==1580&&pect2==1580)//遥控器配对后开机但未操作1580
				if (data1<10&&data2<10)			 //遥控器配对后开机但未操作1580
			{		
					Remote=0;
					stop();
					RemoteFlag=0;
					iot_contorl();
			}
		}
				else //物联网控制
				{
						Remote=0;
						stop();
						RemoteFlag=0;
						iot_contorl();
				}				
}

extern u8  tim2_time;
void iot_contorl(void)
{
		if(MQTTFlag==1 && RemoteFlag==0)				//	iot
			{	
			 	MQTTFlag=0;
			send_data_float=0;
				u1_printf("物联网主控程序-------------------\r\n");

					tim2_time=0;
//				TIM_Cmd(TIM2,DISABLE);
					NEW_IOT_Contorl_TIM2();
				
					TIM_Cmd(TIM2,ENABLE);				//开启定时器 这样就能实现你需要的功能了
					TIM_SetCounter(TIM2, 0);
						Clear_Buffer();	
			}		
			else	
			{
				stop();
				send_data_float=1;
			}
} 
volatile u8 a=0;
void NEW_IOT_Contorl_TIM2(void )
{

		u16 con_CH2, con_CH3;
		u16 CH2_correct_data, CH3_correct_data;
		
		CH2_OUT = IOT_contorl_left ( con_CH2,	 CH2_correct_data )	;		
		CH3_OUT = IOT_contorl_right( con_CH3,	 CH3_correct_data )	;

}

void stop (void)
{
	CH2_OUT = flycolor_ch2_stop;
	CH3_OUT = flycolor_ch3_stop ;
}


/********************************************************************************************
  SHT3x-DIS温湿度模块，采用STM32F407ZET6做主控芯片，模拟IIC通信

  引脚：   SDA-->PB9
           SCL-->PB8
					 AL -->FLOATING
					 AD -->VSS

********************************************************************************************/



