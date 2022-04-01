#include "ml302.h"
#include "usart3.h"
#include "string.h"
#include "usart.h"
#include "wdg.h"
#include "led.h"
#include <stdio.h>
#include "adc.h"
#include "stdlib.h"
#include "timer.h"
#include "flash.h"
#include "sht31.h"	
#include "power.h"	
#include "gps.h"
#include "rs485.h"
extern  uint8_t tem ;
extern  uint8_t hum ;

extern float longitude;
extern float latitude; 

extern  bool IOT_DATA_Flag;

extern char Longitude[40];//����
extern char Lattitude[40];//γ��
extern u32 WDIR_data;		//��������

volatile u8 MQTTClosedFlag = 0;
volatile u8 MQTTReceiveFlag = 0;
//struct STRUCT_USART_Fram ESP8266_Fram_Record_Struct = { 0 };  //������һ������֡�ṹ��
////struct STRUCT_USART_Fram USART3_Fram_Record_Struct = { 0 };  //������һ������֡�ṹ��
//struct STRUCT_USART_Fram USART1_Fram_Record_Struct = { 0 };  //������һ������֡�ṹ��
//struct STRUCT_USART_Fram UART5_Fram_Record_Struct = { 0 };  //������һ������֡�ṹ��

char *strx,*extstrx;
//extern char  RxBuffer[600],RxCounter;
extern char  RxBuffer[900]; 
extern u32 RxCounter;
ML302 ML302_Status;
//��Ҫ�û��ĵ� ������mqtt��
#define ProductID  "399901"  //��Ʒ����
#define DeviceID  "mmm"  //�豸����
#define Token    "version=2018-10-31&res=products%2F399901&et=1642561719&method=md5&sign=KxOIfudO9DCevSbUqOQ3Bg%3D%3D"  //tokenֵ

#define SUCCESS 0
//������Ӱ��topic
#define shadow_topic 	"{\"method\":\"update\",\"state\":{\"reported\":{\"2right_control\":69,\"1left_control\":96}},\"version\":24488}"
//��������Ԫ��(�Լ���)
//#define ProductKey "gckbkat1Fdy"  //��Ʒ����
//#define DeviceName  "PDL180"  //�豸����
//#define DeviceSecret    "5a4e03e8e6ffc514a9208429024ebc30"  //tokenֵ
//#define User_MQTTServer_IP     "gckbkat1Fdy.iot-as-mqtt.cn-shanghai.aliyuncs.com"     		//MQTT���ط�����IP��������
//#define User_MQTTServer_PORT   1883     																									//�������˿ں�
//#define User_client_id    	"PDL180|securemode=3,signmethod=hmacsha1,timestamp=60|"   	//MQTTclientID ���ڱ�־client����  �256�ֽ�
//#define User_username     	"PDL180&gckbkat1Fdy"																			//���ڵ�¼ MQTT ������ �� username, � 64 �ֽ�	
//#define User_password				"03515AD9E24B03525108F22E68BE299FE0892F80"          			//���ڵ�¼ MQTT ������ �� password, � 64 �ֽ�

//��������Ԫ��(��˾��)	��Ԫ��ת���������ӡ�http://mqtt.lovemcu.cn/��
#define ProductKey "gbysTFFDuqt"  //��Ʒ����
#define DeviceName  "PDL180"  //�豸����
//#define DeviceSecret    "9a431448f76b17adf8a7948e9eefc792"  //tokenֵ

#define User_MQTTServer_IP     "gbysTFFDuqt.iot-as-mqtt.cn-shanghai.aliyuncs.com"     		//MQTT���ط�����IP��������
#define User_MQTTServer_PORT   1883     																									//�������˿ں�
#define User_client_id    	"PDL180|securemode=3,signmethod=hmacmd5|"   	//MQTTclientID ���ڱ�־client����  �256�ֽ�
#define User_username     	"PDL180&gbysTFFDuqt"																			//���ڵ�¼ MQTT ������ �� username, � 64 �ֽ�	
#define User_password				"FCF547A6B5A044863A0E2F094983C0D1"          			//���ڵ�¼ MQTT ������ �� password, � 64 �ֽ�

//����topic��δ�õ�
//�Զ���Topic
#define User_MQTTServer_Topic  						"/a1TGuTWhjR8/TestDevice/user/update"  	

//�����ϱ�	AT+MQTTSUB=<topic>,<qos>
#define User_MQTTServer_Topic_Property_Post			"/sys/gbysTFFDuqt/PDL180/thing/event/property/post"
#define User_MQTTServer_Topic_Property_Post_Reply	"/sys/gbysTFFDuqt/PDL180/thing/event/property/post_reply"

//�豸Ӱ��	
#define User_MQTTshadow_Topic_Property_Post_update	"/shadow/update/gbysTFFDuqt/PDL180"
#define User_MQTTshadow_Topic_Property_Post_get	 "/shadow/get/gbysTFFDuqt/PDL180"

#define User_MQTTshadow_Topic_Property_Post_push "/sys/gbysTFFDuqt/PDL180/thing/config/push"
//�¼��ϱ�
#define User_MQTTServer_Topic_Event_Post			"/sys/gckblNBG4eP/PDL180/thing/event/{tsl.event.identifer}/post"
#define User_MQTTServer_Topic_Event_Post_Reply		"/sys/gckblNBG4eP/PDL180/thing/event/{tsl.event.identifer}/post_reply"

//��������
#define User_MQTTServer_Topic_Property_Set			"/sys/gckblNBG4eP/PDL180/thing/service/property/set"
#define User_MQTTServer_Topic_Property_Set_Reply	"/sys/gckblNBG4eP/PDL180/thing/service/property/set_reply"

//�������
#define User_MQTTServer_Topic_Service_Post			"/sys/gckblNBG4eP/PDL180/thing/service/{tsl.service.identifer}"
#define User_MQTTServer_Topic_Service_Post_Reply	"/sys/gckblNBG4eP/PDL180/thing/service/{tsl.service.identifer}_reply"

void Clear_Buffer(void)//��ջ���
{
		u16 i;
		Uart1_SendStr(RxBuffer);
		for(i=0;i<900;i++)
		RxBuffer[i]=0;//����
		
		RxCounter=0;
		//memset(RxBuffer,0,sizeof(RxBuffer));//Ϊʲô����
  	IWDG_Feed();//ι��
}
void Clear_USART1_Buffer(void)//��ջ���
{
		u32 i;
		for(i=0;i<2000;i++)
		USART_RX_BUF[i]=0;
			USART_RX_STA=0;	
}
void ML302_Init(void)
{
		Uart1_SendStr("���β����Ƿ����ݵ�MQTT����������\r\n");
	  delay_ms(300);
    printf("AT\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT\r\n"); 
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK			
    }
    printf("ATE0&W\r\n"); //�رջ���
    delay_ms(300);
		printf("AT+MQTTDISC\r\n");
		delay_ms(300);
		printf("AT+MQTTDEL\r\n");//�ͷ���Դ
		delay_ms(300);
    printf("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"460");//��460������ʶ�𵽿���
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"460");//����OK,˵�����Ǵ��ڵ�
    }
        printf("AT+CGDCONT=2,\"IP\",\"CMNET\"\r\n");//����APN����
        delay_ms(300);
        Clear_Buffer();	
				printf("AT+CGACT=1,1\r\n");//�������磬PDP
		    delay_ms(300);
        Clear_Buffer();	
				printf("AT+CSTT=\"CMNET\",\"\",\"\" \r\n");//��������
	      delay_ms(300);
        Clear_Buffer();	
		    printf("AT+CIICR\r\n");//�����������
			  delay_ms(300);
        Clear_Buffer();	
        printf("AT+CGATT?\r\n");//��ѯ����״̬
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//��1
        Clear_Buffer();	
			while(strx==NULL)
			{
							Clear_Buffer();	
							printf("AT+CGATT?\r\n");//��ȡ����״̬
							delay_ms(300);
							strx=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//����1,����ע���ɹ�
			}
				 printf("AT+CSQ\r\n");//��ѯ�ź�
				 delay_ms(300);
				 Clear_Buffer();	 
}


//��ʼ��MQTT
void ML302_MQTTInit(void)
{
	printf("AT+MQTTCFG=\"%s\",1883,\"%s\",60,\"%s\",\"%s\",0,\"\",0\r\n",User_MQTTServer_IP,User_client_id,User_username,User_password);
    delay_ms(300);
    Clear_Buffer();	
    printf("AT+MQTTOPEN=1,1,0,0,0,\"\",\"\"\r\n");//���ӵ�������ƽ̨
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+MQTTOPEN:OK");//����¼�ɹ�
    while(strx==NULL)
    { 
				Uart1_SendStr("δ��ѯ�� MQTTOPEN:OK,�������²�ѯ...\r\n");
			 delay_ms(1000);
       strx=strstr((const char*)RxBuffer,(const char*)"+MQTTOPEN:OK");//��¼�������Ƴɹ� ��ʱ�豸����ʾ����  
    }
    Clear_Buffer();	
		
//		printf("AT+MQTTSUB=\"%s\",1\r\n",User_MQTTServer_Topic );//���ӵ�������ƽ̨�������¼�
//    delay_ms(300);
//		strx=strstr((const char*)RxBuffer,(const char*)"+MQTTSUBACK:2,1,dev/gsm/u OK");//����OK
//		Clear_Buffer();	

//ML302_MQTT_Topic_Init();
}

void ML302_MQTT_Topic_Init(void)
{
		printf("AT+MQTTSUB=\"%s\",2\r\n",User_MQTTServer_Topic_Property_Post_Reply );//���ӵ�������ƽ̨�������¼�
    delay_ms(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+MQTTSUBACK:2,2,dev/gsm/u OK");//����OK
		Clear_Buffer();	

//		printf("AT+MQTTSUB=\"%s\",1",User_MQTTshadow_Topic_Property_Post_get );//���ӵ�������ƽ̨�������¼�
//    delay_ms(300);
//		strx=strstr((const char*)RxBuffer,(const char*)"+MQTTSUBACK:2,2,dev/gsm/u OK");//����OK
//		Clear_Buffer();	
//				printf("AT+MQTTSUB=\"%s\",1",User_MQTTServer_Topic_Property_Post );//���ӵ�������ƽ̨�������¼�
//    delay_ms(300);
//		strx=strstr((const char*)RxBuffer,(const char*)"+MQTTSUBACK:2,2,dev/gsm/u OK");//����OK
//		Clear_Buffer();	
//				printf("AT+MQTTSUB=\"%s\",1",User_MQTTServer_Topic_Property_Post_Reply );//���ӵ�������ƽ̨�������¼�
//    delay_ms(300);
//		strx=strstr((const char*)RxBuffer,(const char*)"+MQTTSUBACK:2,2,dev/gsm/u OK");//����OK
//		Clear_Buffer();	
}

void ML302_CMD(void)
{
	if (strstr ((const char*)USART_RX_BUF,(const char*)"AT" )	)
	{
		u1_printf ("AT\r\n");
    printf("AT\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    Clear_Buffer();	
		Clear_USART1_Buffer();
	}
		if(strstr ((const char*)USART_RX_BUF,(const char*)"shadow_update" )	)	//������
	{	
		printf("AT+MQTTSUB=\"%s\",1\r\n",User_MQTTshadow_Topic_Property_Post_update );//���ӵ�������ƽ̨�������¼�
		delay_ms(300);
		Clear_Buffer();
		Clear_USART1_Buffer();
	}
	
	if(strstr ((const char*)USART_RX_BUF,(const char*)"shadow_get" )	)
	{	
		printf("AT+MQTTSUB=\"%s\",1\r\n",User_MQTTshadow_Topic_Property_Post_get );//���ӵ�������ƽ̨�������¼�
		delay_ms(300);
		Clear_Buffer();
		Clear_USART1_Buffer();
	}

	if(strstr ((const char*)USART_RX_BUF,(const char*)"thing_config_push" )	)
	{	
		printf("AT+MQTTSUB=\"%s\",1\r\n",User_MQTTshadow_Topic_Property_Post_push );//���ӵ�������ƽ̨�������¼�
		delay_ms(300);
		Clear_Buffer();
		Clear_USART1_Buffer();
	}
	
		if(strstr ((const char*)USART_RX_BUF,(const char*)"ext_request" )	)
	{	
		printf("AT+MQTTSUB=\"%s\",1\r\n",User_MQTTclock_Topic_Property_Post_request );//���ӵ�������ƽ̨�������¼�
		delay_ms(300);
		Clear_Buffer();
		Clear_USART1_Buffer();
	}

	if(strstr ((const char*)USART_RX_BUF,(const char*)"ext_response" )	)
	{	
		printf("AT+MQTTSUB=\"%s\",1\r\n",User_MQTTclock_Topic_Property_Post_response );//���ӵ�������ƽ̨�������¼�
		delay_ms(300);
		Clear_Buffer();
		Clear_USART1_Buffer();
	}
	
	if (strstr ((const char*)USART_RX_BUF,(const char*)"+MQTTSUB" )	)
	{
			u1_printf ("��ѯ�������");
			printf("AT+MQTTSUB?\r\n");//���ӵ�������ƽ̨�������¼�
			delay_ms(300);
			Clear_Buffer();
			Clear_USART1_Buffer();
	}
}

void char_json(char buf)
{
	

}
void IOT_Data(void)
{
	u16 data_Buf[2]={0};
	u16 leftData_Out;
	u16 RightData_Out;
	
	
	
}
/*-------------------------------------------------*/			
/*��������ML302_shadow       				               */
/*���������豸Ӱ��								                 */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void ML302_shadow(void)
{
//#define User_MQTTshadow_Topic_Property_Post_update	"/shadow/update/gbysTFFDuqt/PDL180"
//#define User_MQTTshadow_Topic_Property_Post_get	 "/shadow/get/gbysTFFDuqt/PDL180"

//	User_MQTTshadow_Topic_Property_Post_update	User_MQTTshadow_Topic_Property_Post_get
u16 right_data,left_data;
		static u16 version=24480;
	//AT+MQTTPUB="/shadow/update/gbysTFFDuqt/PDL180",1,0,0,"{\"method\":\"update\",\"state\":{\"reported\":{\"2right_control\":88}},\"version\":24270}"
		char shadow_buf_json_first[] = "AT+MQTTPUB=\"/shadow/update/%s/%s\",1,0,0,%s\r\n";		//�ϱ�״̬��Ӱ��
		char topic_buf_json_first[400];//shadow_buf_json_first��
		char shadow_buf_json_secend[]	=	"{\"method\":\"update\",\"state\":{\"reported\":{\"2right_control\":%d,\"1left_control\":%d}},\"version\":%d}";		
		char topic_buf_json_secend[400];//shadow_buf_json_secend��
	//char shadow_buf_json_secend[]="{\"method\":\"control\",\"payload\":{\"state\":{\"desired\":{\"2right_control\":0,\"1left_control\":10},\"reported\":{\"2right_control\":30,\"1left_control\":10}},\"metadata\":{\"desired\":{\"2right_control\":{\"timestamp\":1642338876},\"1left_control\":{\"timestamp\":1642338876}},\"reported\":{\"2right_control\":{\"timestamp\":1642338876},\"1left_control\":{\"timestamp\":1642338876}}}},\"timestamp\":1642338876,\"version\":28}";
		
	//char shadow_buf_json_secend[]	=	"{\"method\": \"update\", \"state\": {\"reported\": {\"color\": \"red\"}}, \"version\":%d}";

//		char t_json[400];//���ַ���ƴ��
//		char topic_buf[400];	
//		char json_len;
		
				sprintf(topic_buf_json_secend,shadow_buf_json_secend,right_data,left_data,version);//
				
				sprintf(topic_buf_json_first,shadow_buf_json_first,ProductKey,DeviceName,topic_buf_json_secend);	//topic_buf_json_first	:		
//				strcat(topic_buf,t_json);								//ƴ�������ַ���

/*����*/	printf("AT+MQTTPUB=\"/shadow/update/%s/%s\",1,0,0,%s\r\n",ProductKey,DeviceName,shadow_topic);
				u1_printf("AT+MQTTPUB=\"/shadow/update/%s/%s\",1,0,0,%s\r\n",ProductKey,DeviceName,shadow_topic);
				//printf("%s\r\n",topic_buf);
				version++;
				
//			AT+MQTTPUB="/sys/gckblNBG4eP/ML302_/thing/event/property/post",1,0,0,"{\"method\":\"thing.event.property.post\",\"id\":\"2012934115\",\"params\":{\"PM25\":55},\"version\":\"1.0.0\"}"
//			AT+MQTTPUB="/shadow/update/gbysTFFDuqt/PDL180",1,0,0,0{"method":"get"}	
//			AT+MQTTPUB="/shadow/update/gbysTFFDuqt/PDL180",1,0,0,"{\"method\":\"update\",\"state\":{\"reported\":{\"2right_control\":88}},\"version\":24285}"
//[����]AT+MQTTPUB="/shadow/update/gbysTFFDuqt/PDL180",1,0,0,"{\"method\":\"update\",\"state\":{\"reported\":{\"2right_control\":88}},\"version\":24470}"
//[����]+MQTTPUBLISH: 0,1,0,61111,"/shadow/get/gbysTFFDuqt/PDL180",109,{"method":"reply","payload":{"status":"success","version":24470},"clientToken":"null","timestamp":1642487387}
//[����]+MQTTPUBLISH: 0,1,0,6800,"/shadow/get/gbysTFFDuqt/PDL180",328,{"method":"control","payload":{"state":{"desired":{"2right_control":50,"1left_control":40},"reported":{"2right_control":88}},"metadata":{"desired":{"2right_control":{"timestamp":1642487775},"1left_control":{"timestamp":1642487775}},"reported":{"2right_control":{"timestamp":1642487775}}}},"timestamp":1642487775,"version":24471}
//AT+MQTTPUB="/shadow/update/gbysTFFDuqt/PDL180",1,0,0,"{\"method\":\"update\",\"state\":{\"reported\":{\"2right_control\":88}},\"version\":24480}"
}

/*-------------------------------------------------*/			
/*��������ML302_MQTTPUBMessage                     */
/*�����������������������				                 */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
//�������ݵ�������

			 //	 	itoa(value,str,10);//����ת�ַ�������itoa,10����ʮ���ơ�Ҫ����ͷ�ļ�<stdio.h>
			 // 	/sys/gckbkat1Fdy/${deviceName}/thing/event/property/post
			 //		AT+MQTTPUB="/sys/gckblNBG4eP/ML302_/thing/event/property/post",1,0,0,"{\"method\":\"thing.event.property.post\",\"id\":\"2012934115\",\"params\":{\"PM25\":55},\"version\":\"1.0.0\"}"

void ML302_MQTTPUBMessage(u32 temp)
{
	float ADC1_CH9_PB1_DATA;		//	ADC�ɼ��ĵ�ѹֵ
	float input_voltage;				//�����������ѹ
	u16 ADC1_CH9_PB1_REV;				//ADC�ɼ�����
	float atemp;
	float arh;
	
		char json[]= "{\"method\":\"thing.event.property.post\",\"id\":\"2012934115\",\"params\":{\"voltage\":%.2f,\"temp\":%.2f,\"rh\":%.2f,\"WDIR\":%d,\"WSPD\":3},\"version\":\"1.0.0\"}";//������json��ʽ���
		char t_json[400];
		char json_len;

		char t_buf[]= "AT+MQTTPUB=\"/sys/%s/%s/thing/event/property/post\",1,0,0,%d\r\n";		//topic buf 
		char topic_buf[400];
					
			VOL_5V_UART5=1;	

			Read_sht31();
			Convert_sht31();

//���12v�ڵ�ѹ  PB1  ���������
				ADC1_CH9_PB1_REV=Get_Adc_Average(ADC_Channel_9,100);
				ADC1_CH9_PB1_DATA=(float)ADC1_CH9_PB1_REV*(3.3/4096);	//���adcֵ��12λ���Ϊ4096	
//				input_voltage = ADC1_CH9_PB1_DATA / 0.047;							//����ʵ�������ѹֵ���ɼ���ѹֵ��ʵ�������ѹֵ����Ϊ0.047	��PDL180��
				input_voltage = ADC1_CH9_PB1_DATA / 0.166;							//����ʵ�������ѹֵ���ɼ���ѹֵ��ʵ�������ѹֵ����Ϊ0.166	��PDL280��
				
				atemp=tem;
				arh=hum;

		RS485_sensor_data();		
//							u1_printf("longitude:%f\r\n",longitude);//�������ݵ�ƽ̨
//							
//							u1_printf("lattitude:%f\r\n",latitude);	//�������ݵ�ƽ̨		
				sprintf(t_json, json, input_voltage,  atemp, arh ,WDIR_data);

				json_len = strlen(t_json)/sizeof(char);	

				sprintf(topic_buf,t_buf,ProductKey,DeviceName,json_len);
				strcat(topic_buf,t_json);								//ƴ�������ַ���
	 
			printf("%s",topic_buf);//�������ݵ�ƽ̨
			u1_printf("u1_printf:%s",topic_buf);//�������ݵ�ƽ̨
			
      delay_ms(300);
//			strx=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBACK:0");//���ط����ɹ�
//    while(strx==NULL)
//    { 
//       Uart1_SendStr("δ��ѯ�� MQTTPUBACK:0,�������²�ѯ...\r\n");
//			 delay_ms(1000);
//			 strx=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBACK:0");//�������ݳɹ�
//    }
			Clear_Buffer();	

}
extern float Speed;
extern float gps;
extern float beidou;
extern float high;
extern int	gps_qty	;
extern int  bd_qty;
extern bool  Remote;

void ML302_GPS_MQTTPUBMessage(char  temp)
{
		char json[]= "{\"method\":\"thing.event.property.post\",\"id\":\"2012934115\",\"params\":{\"Speed\":%.2f,\"gps\":%d,\"beidou\":%d,\"jingdu\":%.5f,\"weidu\":%.5f,\"high\":%d,\"Remote\":%d},\"version\":\"1.0.0\"}";//������json��ʽ���
		char t_json[400];
		char json_len;

		char t_buf[]= "AT+MQTTPUB=\"/sys/%s/%s/thing/event/property/post\",1,0,0,%d\r\n";		//topic buf 
		char topic_buf[400];
		
				VOL_5V_UART5=1;	
					

				sprintf(t_json, json, Speed,  gps_qty, bd_qty ,longitude,latitude,high,Remote);

				json_len = strlen(t_json)/sizeof(char);	

				sprintf(topic_buf,t_buf,ProductKey,DeviceName,json_len);
				strcat(topic_buf,t_json);								//ƴ�������ַ���
	 
			printf("%s",topic_buf);//�������ݵ�ƽ̨
			u1_printf("u1_printf:%s",topic_buf);//�������ݵ�ƽ̨
			
//			strx=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBACK:0");//���ط����ɹ�
//    while(strx==NULL)
//    { 
//       Uart1_SendStr("δ��ѯ�� MQTTPUBACK:0,�������²�ѯ...\r\n");
//			 delay_ms(1000);
//			 strx=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBACK:0");//�������ݳɹ�
//    }
			Clear_Buffer();	

}



//___________________________________________________________________________________________________________________________________
//���ߵ��Կ��Ʒ�ʽ
extern	bool mqttput_float;
/*-------------------------------------------------*/			
/*��������ML302_Right_Message                      */
/*�����������հ�������ͨ������				    		     */
/*��  ������                                       */
/*����ֵ��ch2_temp                                 */
/*-------------------------------------------------*/
u16 ML302_Right_Message(void)
{
		int i;	
	  char right_data_buf[200];
			char cmd_buf[]={"\"right_control\":"};
			char *buf;
			u32 cmd_buf_len;
			int right_data;
			static 	u32 ch2_temp=flycolor_stop;
//					u1_printf("ML302_Right_Message��ִ��\r\n");
			if(strstr((const char*)RxBuffer,(const char*)"control"))
				{	mqttput_float=0;
							strx=strstr((const char*)RxBuffer,(const char*)"{");//��ȡ����һ��{
							for(i=0;;i++)
							{
									if(strx[i+1]==0x0d)
									break;
									ML302_Status.recdata[i]=strx[i+1];//��ȡ��������		
							}

							strx=strstr((const char*)(strx+1),(const char*)"{");	//��ȡ���ڶ���{
							for(i=0;;i++)
							{
										if(strx[i+1]==0x7d)			//7DΪ}
									break;
									ML302_Status.sensordata[i]=strx[i+1];//��ȡ��������
																																			//ML302_Status.sensordata[i]����Ϊ	VerticalSwitch":0,"AdasSwitch":1							
							}
					if(strstr((const char*)ML302_Status.sensordata,(const char*)"\"right_control\":"))
							{
							//��ȡ�ַ����е�����================================								
									buf =	strstr(ML302_Status.sensordata,cmd_buf);
									
									cmd_buf_len	=	strlen(cmd_buf);							//�ַ�������
										 
									strncpy(right_data_buf,buf+cmd_buf_len,3);			//��ȡ"\"right_control\":"����3λ�ַ�
									
									right_data = atoi(right_data_buf);//��buf�����ֵת��Ϊʮ�������
									
						  		u1_printf("\r\n right=%d\r\n",right_data);

									if( 0<=right_data && right_data<=100)
										{
											ch2_temp=right_data*10+18000;
										}
									else
									u1_printf("CH2���ֵ���󡣡���");		
//									u1_printf("ch2_temp%d\r\n",ch2_temp);												
							}							
					}										
						u1_printf("ch2_temp::%d\r\n",ch2_temp);		
					return	ch2_temp;
	}

/*-------------------------------------------------*/			
/*��������ML302_Left_Message                       */
/*�����������հ�������ͨ������				    		     */
/*��  ������                                       */
/*����ֵ��ch3_temp                                 */
/*-------------------------------------------------*/
u16 ML302_Left_Message(void)
{
	int i;
	char left_data_buf[200];	
	char cmd_buf[]={"\"left_control\":"};
	char *buf;
	u32 cmd_buf_len;
	int left_data;
	static	u32 ch3_temp=flycolor_stop;
	
//			u1_printf("ML302_Left_Message��ִ��\r\n");
//params
				if(strstr((const char*)RxBuffer,(const char*)"control"))
					{
						mqttput_float=0;
						strx=strstr((const char*)RxBuffer,(const char*)"{");//��ȡ����һ��{
						for(i=0;;i++)
						{
								if(strx[i+1]==0x0d)
								break;
								ML302_Status.recdata[i]=strx[i+1];//��ȡ��������
						}
						strx=strstr((const char*)(strx+1),(const char*)"{");	//��ȡ���ڶ���{
						for(i=0;;i++)
						{
								if(strx[i+1]==0x7d)			//7DΪ}
								break;
								ML302_Status.sensordata[i]=strx[i+1];//��ȡ��������
																																			//ML302_Status.sensordata[i]����Ϊ	VerticalSwitch":0,"AdasSwitch":1	
						}		
				//��ȡ�ַ����е�����================================
							if(strstr((const char*)ML302_Status.sensordata,(const char*)"\"left_control\":"))
						{
											buf =	strstr(ML302_Status.sensordata,cmd_buf);
										
											cmd_buf_len	=	strlen(cmd_buf);							//�ַ�������
												 
											strncpy(left_data_buf,buf+cmd_buf_len,3);			//��ȡ"\"right_control\":"����3λ�ַ�
																									
											left_data = atoi(left_data_buf);
											
											u1_printf("left=%d\r\n",left_data);			
											if( 0<=left_data && left_data<=100)
											{
												ch3_temp=left_data*10+18000;
											}
											else
											u1_printf("CH1���ֵ���󡣡���");		
						}
			}
						u1_printf("ch3_temp::%d\r\n",ch3_temp);		
					return	ch3_temp;
	}
//___________________________________________________________________________________________________________________________________
//�����豸Ӱ�ӿ��Ʒ�ʽ
/*-------------------------------------------------*/			
/*��������ML302_Right_Message                      */
/*�����������հ�������ͨ������				    		     */
/*��  ������                                       */
/*����ֵ��ch2_temp                                 */
/*-------------------------------------------------*/
u16 State_ML302_Right_Message(char *buff)
{
//		int i;	
	  char right_data_buf[200];
			char cmd_buf[]={"\"right_control\":"};
			char *buf;
			u8 cmd_buf_len;
			int right_data;
		static 	u16 ch2_temp=flycolor_stop;
//		char *pos ;
//		char *pos_reav;

		if(strstr((const char*)buff,(const char*)"control"))
				{	
///		pos = strstr((const char*)RxBuffer,"desired");//state;{		����ֵ	//desired":{"right_control":10,"left_control":10},"reported":{"right_control":2,"left_control":22}},"metadata":{"desired":{"	
					
					if(strstr((const char*)buff,(const char*)"\"right_control\":"))
							{
							//��ȡ�ַ����е�����================================								
									buf =	strstr(buff,cmd_buf);
									
									cmd_buf_len	=	strlen(cmd_buf);							//�ַ�������
										 
									strncpy(right_data_buf,buf+cmd_buf_len,3);			//��ȡ"\"right_control\":"����3λ�ַ�
									
									right_data = atoi(right_data_buf);//��buf�����ֵת��Ϊʮ�������
									
						///  		u1_printf("\r\n right=%d\r\n",right_data);

									if( -100<=right_data && right_data<=100)
										{						
											if( -100<=right_data && right_data<=100)
											{
													if( right_data <0)
													{
														right_data = abs(right_data);
														ch2_temp = flycolor_stop -(right_data*(flycolor_stop - flycolor_low)/100);///
													}
													else
													{
														ch2_temp = flycolor_stop +(right_data*(flycolor_high - flycolor_stop)/100);///
													}	
											}												
										}
									else
									u1_printf("CH2���ֵ���󡣡���");		
//									u1_printf("ch2_temp%d\r\n",ch2_temp);												
							}							
					}										
			///			u1_printf("ch2_temp::%d\r\n",ch2_temp);		
					return	ch2_temp;
	}

/*-------------------------------------------------*/			
/*��������ML302_Left_Message                       */
/*�����������հ�������ͨ������				    		     */
/*��  ������                                       */
/*����ֵ��ch3_temp                                 */
/*-------------------------------------------------*/
u16 State_ML302_Left_Message(char *buff)
{
	char left_data_buf[200];	
	char cmd_buf[]={"\"left_control\":"};
	char *buf;
	u32 cmd_buf_len;
	int left_data;
	static	u32 ch3_temp=flycolor_stop;
	
				//��ȡ�ַ����е�����================================
							if(strstr((const char*)buff,(const char*)"\"left_control\":"))	//strstr�����ַ������״γ����Ӵ��ĵ�ַ��
								{
										buf =	strstr(buff,cmd_buf);	
									
										cmd_buf_len	=	strlen(cmd_buf);							//�ַ�������
											 
										strncpy(left_data_buf,buf+cmd_buf_len,3);			//��ȡ"\"right_control\":"����3λ�ַ�
																								
										left_data = atoi(left_data_buf);
										
				///						u1_printf("left=%d\r\n",left_data);			
										if( -100<=left_data && left_data<=100)
										{
												if( left_data <0)
												{
													left_data = abs(left_data);
													ch3_temp = flycolor_stop -(left_data*(flycolor_stop - flycolor_low )/100);///200��ԭ��������ķ�ΧΪ-100��100
												}
												else
												{
													ch3_temp = flycolor_stop +(left_data*(flycolor_high - flycolor_stop)/100);///200��ԭ��������ķ�ΧΪ-100��100
												}	
										}
										else
										u1_printf("CH1���ֵ���󡣡���");		
								}
	//					u1_printf("ch3_temp::%d\r\n",ch3_temp);		
					return	ch3_temp;
	}
	
	
	u8 ML302_Time_Message(char *buff)
{
	char time_data_buf[200];	
	char cmd_buf[]={"\"control_time\":"};
	char *buf;
	u32 cmd_buf_len;
	int time_data;
	static	u32 time=10;
	
				//��ȡ�ַ����е�����================================
							if(strstr((const char*)buff,(const char*)"\"control_time\":"))
						{
											buf =	strstr(buff,cmd_buf);
										
											cmd_buf_len	=	strlen(cmd_buf);							//�ַ�������
												 
											strncpy(time_data_buf,buf+cmd_buf_len,2);			//��ȡ"\"right_control\":"����3λ�ַ�
																									
											time_data = atoi(time_data_buf);
											
					///						u1_printf("time_data = %d\r\n",time_data);			
											if( 0<time_data && time_data<=Iot_Contorl_Time)
											{
												time=time_data;
											}
											else
											u1_printf("ʱ�����ֵ���󡣡���");		
						}
				///		u1_printf("time::%d\r\n",time);		
					return	time;
	}
	
	//�ַ�����ȡ����
	void char_data(void)
	{
	
	int length;
	int length_reav;

	char *pos ;

	if(strstr((const char*)RxBuffer,(const char*)"shadow"))
		{
	/*���������ַ�����ַ���*/	
	//		pos = strstr((const char*)RxBuffer,"desired");//state;{
	//		length = strlen(pos);

	//		pos_reav = strstr((const char*)pos,"metadata");//metadata
	//		length_reav = strlen(pos_reav);

	//		pos[length-length_reav] = '\0';
	//		pos ++;
	//		u1_printf("POS��%s\r\n",pos);
	//		u1_printf("BUF:	%s\r\n",RxBuffer);
	//		Clear_Buffer();	

/*��������Ҫ���ַ���λ��*/
		pos = strstr((const char*)RxBuffer,"reported");//desired	state;{		����ֵ	//desired":{"right_control":10,"left_control":10},"reported":{"right_control":2,"left_control":22}},"metadata":{"desired":{"
				length= State_ML302_Right_Message(pos);
				length_reav = State_ML302_Left_Message(pos);
//		pos_reav = strstr((const char*)pos,"reported");//metadata		�·�ֵ	reported":{"right_control":2,"left_control":22}},"metadata":{"desired":{"
//				State_ML302_Right_Message(pos_reav);
//				State_ML302_Left_Message(pos_reav);
//		u1_printf("POS��%s\r\n",pos);	
//		u1_printf("POS����REAV��%s\r\n",pos_reav);
		u1_printf("length:	%d\r\n",length);
		u1_printf("length_reav:	%d\r\n",length_reav);
//		Clear_Buffer();	
	}
}

//
u16 ML302_Right_Message_shadow(void)
{
		static u16 right_data;
		char *pos_reav;

		if(strstr((const char*)RxBuffer,(const char*)"shadow"))
			{
/*��������Ҫ���ַ���λ��*/
////				pos = strstr((const char*)RxBuffer,"desired");//state;{		����ֵ	//desired":{"right_control":10,"left_control":10},"reported":{"right_control":2,"left_control":22}},"metadata":{"desired":{"
////				State_ML302_Right_Message(pos);
				pos_reav = strstr((const char*)RxBuffer,"desired");//metadata		�·�ֵ	reported":{"right_control":2,"left_control":22}},"metadata":{"desired":{"

				right_data = State_ML302_Right_Message(pos_reav);

			}
			
			return right_data;
					
}
//
u16 ML302_Left_Message_shadow(void)
{
		static u16 left_data;
		char *pos_reav;

		if(strstr((const char*)RxBuffer,(const char*)"shadow"))
			{
/*��������Ҫ���ַ���λ��*/
				
////				pos = strstr((const char*)RxBuffer,"desired");//state;{		����ֵ	//desired":{"right_control":10,"left_control":10},"reported":{"right_control":2,"left_control":22}},"metadata":{"desired":{"
////				State_ML302_Left_Message(pos);				
				pos_reav = strstr((const char*)RxBuffer,"desired");//metadata		�·�ֵ	reported":{"right_control":2,"left_control":22}},"metadata":{"desired":{"
				left_data = State_ML302_Left_Message(pos_reav);
				
			}

			return left_data;
}

u8 ML302_Time_Message_shadow(void)
{
		static u8 time_data;
		char *pos ;

		if(strstr((const char*)RxBuffer,(const char*)"shadow"))
			{
		/*��������Ҫ���ַ���λ��*/
				pos = strstr((const char*)RxBuffer,"desired");//metadata		�·�ֵ	reported":{"right_control":2,"left_control":22}},"metadata":{"desired":{"
				time_data = ML302_Time_Message(pos);
			}

			return time_data;
}

/*��д�ַ�����ȡ��������ȡ����������*/

//char buff_json(const char *buff)
//{
//	if(strstr((const char*)RxBuffer,(const char*)" \"\ "))
//		u1_printf("	%s\r\n",);
//}
