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

#define Iot_Contorl_Time  10		//����ȴ�ʱ����10s
																							//				/shadow/update/gbysTFFDuqt/${deviceName}
																							//			 	/shadow/get/gbysTFFDuqt/${deviceName}
#define		User_MQTTshadow_Topic_Property_Post_get_reply				"/sys/gbysTFFDuqt/PDL180/thing/property/desired/get_reply"
#define		User_MQTTshadow_Topic_Property_Post_delete_reply			"	/sys/gbysTFFDuqt/PDL180/thing/property/desired/delete_reply"
#define 	User_MQTTshadow_Topic_Property_Post_update_new	"/shadow/update/gbysTFFDuqt/PDL180"	//	gckbkat1Fdy��Ӱ��
#define 	User_MQTTshadow_Topic_Property_Post_get	 				"/shadow/get/gbysTFFDuqt/PDL180"


#define User_MQTTclock_Topic_Property_Post_request	"/ext/ntp/gckbkat1Fdy/PDL180/request	"//����
#define User_MQTTclock_Topic_Property_Post_response	 "/ext/ntp/gckbkat1Fdy/PDL180/response	"//����

#define User_MQTTServer_Topic  						"/a1TGuTWhjR8/TestDevice/user/update"  	

void cmd_delay(u32 cnt1);//��ʱ����ʼ����
void count(u32 t);//��ʱ����ʼ����

void Clear_USART1_Buffer(void);//��ջ���
void ML302_shadow(void);

void Clear_Buffer(void);//��ջ���	
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
    uint8_t Socketnum;   //���
    uint8_t reclen;   //��ȡ�����ݵĳ���
    uint8_t res;      
//    uint8_t recdatalen[10];
//    uint8_t recdata[200];
    uint8_t netstatus;//����ָʾ��
		
		 char recdatalen[10];
			char recdata[2000];
		 char sensordata[1000];//����ָʾ��
} ML302;

//-----------------------���ߵ���
u16 ML302_Right_Message(void);	//�ҵ��
u16 ML302_Left_Message(void);	//����

//-----------------------�豸Ӱ��
//u32	State_ML302_Right_Message(void);	//�ҵ��
//u32 State_ML302_Left_Message(void);	//����
u16	State_ML302_Right_Message(char *buff);	//�ҵ�����ݽ���
u16 State_ML302_Left_Message(char *buff);	//�������ݽ���
u8 ML302_Time_Message(char *buff);//����ʱ������

u16 ML302_Right_Message_shadow(void);//�ҵ������
u16 ML302_Left_Message_shadow(void);//��������
u8 ML302_Time_Message_shadow(void);//����ʱ������

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


//#define RX_BUF_MAX_LEN 1024       //����ֽ�������������Ϊ2^14
//extern struct STRUCT_USART_Fram   //����֡�ṹ��
//{
//    char Data_RX_BUF[RX_BUF_MAX_LEN];
//    union 
//    {
//        __IO u16 InfAll;						//������Ϣ
//        struct 
//        {
//            __IO u16 FramLength       :15;     	// �����ֽڳ���  14:0 
//            __IO u16 FramFinishFlag   :1;      	// ������ɱ�־  15 
//        }InfBit;								//��λ��Ϣ
//    }; 
//}ESP8266_Fram_Record_Struct;

//extern volatile uint8_t MQTTClosedFlag;  	//����״̬��־
//extern volatile uint8_t MQTTReceiveFlag; 	//�������ݱ�־
//extern struct STRUCT_USART_Fram  USART1_Fram_Record_Struct;
//extern struct STRUCT_USART_Fram UART5_Fram_Record_Struct ;
#endif




