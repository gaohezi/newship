#ifndef __ML302_H
#define __ML302_H	
#include "usart.h"
#include "usart3.h"
#include <stm32f10x.h>
#include "delay.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "stm32f10x.h"
#include "rtc.h" 	
#include "timer.h"
#include "remote.h"

#define Iot_Contorl_Time  10		//定义等待时间是10s
																							//				/shadow/update/gbysTFFDuqt/${deviceName}
																							//			 	/shadow/get/gbysTFFDuqt/${deviceName}
#define		User_MQTTshadow_Topic_Property_Post_get_reply				"/sys/gbysTFFDuqt/PDL180/thing/property/desired/get_reply"
#define		User_MQTTshadow_Topic_Property_Post_delete_reply			"	/sys/gbysTFFDuqt/PDL180/thing/property/desired/delete_reply"
#define 	User_MQTTshadow_Topic_Property_Post_update_new	"/shadow/update/gbysTFFDuqt/PDL180"	//	gckbkat1Fdy不影响
#define 	User_MQTTshadow_Topic_Property_Post_get	 				"/shadow/get/gbysTFFDuqt/PDL180"


#define User_MQTTclock_Topic_Property_Post_request	"/ext/ntp/gckbkat1Fdy/PDL180/request	"//发布
#define User_MQTTclock_Topic_Property_Post_response	 "/ext/ntp/gckbkat1Fdy/PDL180/response	"//订阅

#define User_MQTTServer_Topic  						"/a1TGuTWhjR8/TestDevice/user/update"  	

void cmd_delay(u32 cnt1);//定时器开始计数
void count(u32 t);//定时器开始计数

void Clear_USART1_Buffer(void);//清空缓存
void ML302_shadow(void);

void Clear_Buffer(void);//清空缓存	
void ML302_Init(void);
void ML302_MQTTInit(void);
void ML302_MQTTPUBMessage(u32 temp);

void ML302_MQTTshadowPUBMessage(u32 temp);


void ML302_GPS_MQTTPUBMessage(char temp);
void ML302_CMD(void);

void Clear_Buffer(void);
typedef struct
{
    uint8_t CSQ;    
    uint8_t Socketnum;   //编号
    uint8_t reclen;   //获取到数据的长度
    uint8_t res;      
//    uint8_t recdatalen[10];
//    uint8_t recdata[200];
    uint8_t netstatus;//网络指示灯
		
		 char recdatalen[10];
			char recdata[2000];
		 char sensordata[1000];//网络指示灯
} ML302;

//-----------------------在线调试
u16 ML302_Right_Message(void);	//右电机
u16 ML302_Left_Message(void);	//左电机

//-----------------------设备影子
//u32	State_ML302_Right_Message(void);	//右电机
//u32 State_ML302_Left_Message(void);	//左电机
u16	State_ML302_Right_Message(char *buff);	//右电机数据解析
u16 State_ML302_Left_Message(char *buff);	//左电机数据解析
u8 ML302_Time_Message(char *buff);//控制时间数据

u16 ML302_Right_Message_shadow(void);//右电机数据
u16 ML302_Left_Message_shadow(void);//左电机数据
u8 ML302_Time_Message_shadow(void);//控制时间数据

void ML302_MQTT_Topic_Init(void);
//-----------------------

void cnt (u8 t);
	void char_data(void);

	void ML302_Receive_Data(void);
	void Boat_Corturl_Cmd(void);
//	void M5L302_RECData(void);
u32 ML302_ResMQTTPUBMessage(char temp);
//	void BC26_RECData(void);
	
	u8 test(char *pcBuf, char *pcRes);


//#define RX_BUF_MAX_LEN 1024       //最大字节数，最大可以设为2^14
//extern struct STRUCT_USART_Fram   //数据帧结构体
//{
//    char Data_RX_BUF[RX_BUF_MAX_LEN];
//    union 
//    {
//        __IO u16 InfAll;						//所有信息
//        struct 
//        {
//            __IO u16 FramLength       :15;     	// 接收字节长度  14:0 
//            __IO u16 FramFinishFlag   :1;      	// 接收完成标志  15 
//        }InfBit;								//按位信息
//    }; 
//}ESP8266_Fram_Record_Struct;

//extern volatile uint8_t MQTTClosedFlag;  	//连接状态标志
//extern volatile uint8_t MQTTReceiveFlag; 	//接收数据标志
//extern struct STRUCT_USART_Fram  USART1_Fram_Record_Struct;
//extern struct STRUCT_USART_Fram UART5_Fram_Record_Struct ;
#endif





