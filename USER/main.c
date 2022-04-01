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
//״̬1���ϵ�������ң����δ���ʱΪ������ֵΪ0
//״̬2���ϵ�������ң�������ʱ������ֵΪ����ֵ
//״̬3����ͨ��Ϊ18500����ͨ��Ϊ18000ʱ������Ϊ�ϵ�״̬		
//��ҡ�˼�CH2	�ɼ��м�ֵΪ 1469-1470-1471
//��ҡ�˼�CH2	�ɼ����ֵ��ҡ�����ƣ�Ϊ 1849-1850
//��ҡ�˼�CH2	�ɼ���Сֵ��ҡ�����ƣ�Ϊ 1150-1149
//.
//��ҡ�˼�CH2	�ɼ��м�ֵΪ 1456
//��ҡ�˼�CH2	�ɼ����ֵ��ҡ�����ƣ�Ϊ 1997-2000
//��ҡ�˼�CH2	�ɼ���Сֵ��ҡ�����ƣ�Ϊ 999-1002

//��ҡ�˼�CH3	�ɼ��м�ֵΪ 1508
//��ҡ�˼�CH3	�ɼ����ֵ��ҡ�����ƣ�Ϊ 1989-1990
//��ҡ�˼�CH3	�ɼ���Сֵ��ҡ�����ƣ�Ϊ 1010-1011

//��ͨ�� �ɼ� TIM5CH1	PA0	������A3
//��ͨ�� �ɼ� TIM3CH3	PB0	������A6-
//PWM���	D1
//PWM��� D3

bool Timeflag = false;
extern bool MQTTFlag ;
bool send_data_float=0;//��������״̬��־λ

extern u16 temp3;
/*
TIM2��ΪPWM���
*/
extern bool RemoteFlag;
bool  stop_flag=0;

//u32 TEXT_Buffer[200];
//#define SIZE sizeof(TEXT_Buffer)		//���鳤��
//#define FLASH_SAVE_ADDR  0X08070000		//����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)

extern char *strx,*extstrx;
extern char  RxBuffer[900]; 
extern u16 RxCounter;

//extern char UART5_RxBuffer[900];     //���ջ���,���USART_REC_LEN���ֽ�.
//extern u32 UART5_RxCounter;

extern	float longitude;
extern	float latitude;
extern	float Speed;
extern	float gps;
extern	float beidou;
extern	float high;
extern	int gps_qty;//gps����
extern	int bd_qty;//��������
	u16 len=0;
/***
����״̬�����궨
***/


	u16 	CH2_OUT=flycolor_ch2_stop;
	u16	  CH3_OUT=flycolor_ch3_stop;
	u8 time;

int main(void)
{				
	delay_init();	    	 //��ʱ������ʼ��	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�  
	LED_Init();			     //LED�˿ڳ�ʼ��
	POWER_Init();				 //��ѹ���Ƴ�ʼ��
	VOL_5V_S=1;			   //�����ϲ���5V	��ѹ
	
/* IIC */	
	  IIC_Init();			//IIC��ʼ�� 
/*USART1*/	
	  uart_init(115200);	 //���ڳ�ʼ��Ϊ115200	��ӡ�����Ե������� 
/*JY901*/	
  	uart5_init(115200);
/* ADC */	
		Adc_Init();
/* 485 */	
		RS485_Init(9600);
//		TIM6_Init(); 
/*ML302*/		
		usart3_init(115200);//�Խ�ML302
		
	IWDG_Init(7,625);    //8Sһ��    Tout=((4*2^prer)*rlr)/40

/***********************PWM���*************************/

	TIM4_PWM_Init(19999,71);		//720000/2*72=5000

/***********************PWM����*************************/	
	
	TIM3_Cap_Init(0XFFFF,72-1);	//��1Mhz��Ƶ�ʼ��� 
	TIM5_Cap_Init(0XFFFF,72-1);
			
/***********************����ML302�������ݵ�MQTT������������ʹ��mqtt.fx�������ж���*************************/

	ML302PWR_Init();//��ML302����
 
	PWRKEY=1;//��ML302����
	ML302_Init();//���豸��ʼ��
	ML302_MQTTInit();//��ʼ��MQTT����

/***********************����ML302�������ݵ�MQTT������������ʹ��mqtt.fx�������ж���*************************/
/// TIM6_Int_Init(59999,11999);
 
  TIM6_Int_Init(9999,7199);//10Khz�ļ���Ƶ��,  ��ʱ1s == 1000 000us 
	TIM2_Int_Init(5999,7199);//10Khz�ļ���Ƶ�ʣ�������1000Ϊ10ms(���������)
	//	TIM2_Int_Init(5999,11999);//Tout= ((arr+1)*(psc+1))/Tclk��arr��������װֵ��psc��Ƶ����Tclkϵͳʱ��Ƶ�ʣ�Toutһ�����ڵ�ʱ�䡣
/********************�������**********************/
/*�����һ��ʹ��Ҫ�������̣���������������裬������������ţ�������������ţ���һ������ᶯ��һ��Ҫע�ⰲȫ*/

/*��⴬��ӯ���ֵ��PWM*/
	TIM_SetCompare2(TIM4,flycolor_low);
	TIM_SetCompare3(TIM4,flycolor_low);	 
	delay_ms(1000);  //����ʱ
	TIM_SetCompare2(TIM4,flycolor_high);
	TIM_SetCompare3(TIM4,flycolor_high);//18800 ���ռ�ձȣ�1ms   18000���ռ�ձȣ�2.0ms  18500 50%ռ�ձȣ�1.5ms����Ҫ1  1.5  2��
	delay_ms(1000);		//����ʱ

/********************�������**********************/		 
////10Khz�ļ���Ƶ�ʣ�������1000Ϊ10ms(���������)
//���������ˡ�
/*1���ҵ�buf����ؼ���char��u32ȡֵ����			*/
/*2���ҵ�rxbuff�е����鳤�ȣ������Ժ�ʹ��			*/
/*3���������룬��ϸ����ÿһ������			*/
/*4��jsonת�����ִ����д			*/

/*	20220225		*/
/*1������������ת���������		*/
/*2�����ҵ����ˮ�����㣬ǰ��ʱ����ƫ��			*/
/*3���������г����·�������ˢ��ҳ�棬ֹͣ����		*/
/*4��Ҫ��������ռ����			*/
	while(1)
	{
			IOT_contorl_status();		
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
			dective_contorl();

			
////				u1_printf("��������CH2_OUT��%d\r		CH3_OUT��%d\r\n		\r\n",CH2_OUT,CH3_OUT);	

			GPS_data ();
//			if(strstr((const char*)USART_RX_BUF,(const char*)"AT"))
//			{
//				a=0;
//				TIM_Cmd(TIM2,ENABLE);//������ʱ�� ��������ʵ������Ҫ�Ĺ�����
//				Clear_USART1_Buffer();
//			}

		////	delay_ms(1000);

			TIM_SetCompare1(TIM4,CH2_OUT);//TIM2_CH1   �ҵ��
			TIM_SetCompare3(TIM4,CH3_OUT);//TIM2_CH3   �ҵ��	
			IWDG_Feed();//ι��
			
	u1_printf("��������CH2_OUT��%d\r		CH3_OUT��%d\r\n		\r\n",CH2_OUT,CH3_OUT);		
	}

}
bool  Remote=0;
void dective_contorl(void)
{
 
	int data1,data2;

	u16 pect2_CH2,pect3_CH3;

	int ch2,ch3;				
	pect2_CH2=TIM3CH2_collect( );//�ٷֱ�
	pect3_CH3=TIM3CH3_collect( );//�ٷֱ�
	
			data1 =	abs(pect2_CH2	-	Remote_ch2_Med_Val);		//�м�ֵ	
			data2 =	abs(pect3_CH3	-	Remote_ch3_Med_Val);		//�м�ֵ
			
						//ң��������
						////ң����ͨ��2					����Сֵ��1000 1002		1457		1912 2000�����ֵ��
						////ң����ͨ��3�����ţ�	����Сֵ��1069 1120		1575  	2030 2068�����ֵ��
		///					u1_printf("pect2_CH2:%d		pect3_CH3:%d	\r\n",pect2_CH2,pect3_CH3);			
		if(pect2_CH2 < Remote_CH2_Max_Val && pect2_CH2	>	Remote_CH2_Min_Val && pect3_CH3	<	Remote_CH3_Max_Val && pect3_CH3	>	Remote_CH3_Min_Val)//ң�����ɼ�ֵ����1120-2120֮��
		{
				///u1_printf("--------------------------ң����������------------------------\r\n");
			
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
			ch2	=	(pect2_CH2 - Remote_CH2_Out_Min_Val)*100/(Remote_CH2_Out_Max_Val - Remote_CH2_Out_Min_Val);	//�ٷֱ�
///		u1_printf("ch2:%d		\r\n",ch2);
							
			ch3	=	(pect3_CH3 - Remote_CH3_Out_Min_Val)*100/(Remote_CH3_Out_Max_Val - Remote_CH3_Out_Min_Val);	//�ٷֱ�
///		u1_printf("ch3:%d		\r\n",ch3);	
						
						CH2_OUT =	flycolor_high -(ch2*(flycolor_high-flycolor_low)/100);	
						CH3_OUT =	flycolor_high -(ch3*(flycolor_high-flycolor_low)/100);	
				
//			if (pect1==1580&&pect2==1580)//ң������Ժ󿪻���δ����1580
				if (data1<10&&data2<10)			 //ң������Ժ󿪻���δ����1580
			{		
					Remote=0;
					stop();
					RemoteFlag=0;
					iot_contorl();
			}
		}
				else //����������
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
				u1_printf("���������س���-------------------\r\n");

					tim2_time=0;
//				TIM_Cmd(TIM2,DISABLE);
					NEW_IOT_Contorl_TIM2();
				
					TIM_Cmd(TIM2,ENABLE);				//������ʱ�� ��������ʵ������Ҫ�Ĺ�����
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
  SHT3x-DIS��ʪ��ģ�飬����STM32F407ZET6������оƬ��ģ��IICͨ��

  ���ţ�   SDA-->PB9
           SCL-->PB8
					 AL -->FLOATING
					 AD -->VSS

********************************************************************************************/


