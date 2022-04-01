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

extern char Longitude[40];//经度
extern char Lattitude[40];//纬度
extern u32 WDIR_data;		//风向数据

volatile u8 MQTTClosedFlag = 0;
volatile u8 MQTTReceiveFlag = 0;
//struct STRUCT_USART_Fram ESP8266_Fram_Record_Struct = { 0 };  //定义了一个数据帧结构体
////struct STRUCT_USART_Fram USART3_Fram_Record_Struct = { 0 };  //定义了一个数据帧结构体
//struct STRUCT_USART_Fram USART1_Fram_Record_Struct = { 0 };  //定义了一个数据帧结构体
//struct STRUCT_USART_Fram UART5_Fram_Record_Struct = { 0 };  //定义了一个数据帧结构体

char *strx,*extstrx;
//extern char  RxBuffer[600],RxCounter;
extern char  RxBuffer[900]; 
extern u32 RxCounter;
ML302 ML302_Status;
//需要用户改的 （其他mqtt）
#define ProductID  "399901"  //产品名称
#define DeviceID  "mmm"  //设备名称
#define Token    "version=2018-10-31&res=products%2F399901&et=1642561719&method=md5&sign=KxOIfudO9DCevSbUqOQ3Bg%3D%3D"  //token值

#define SUCCESS 0
//测试用影子topic
#define shadow_topic 	"{\"method\":\"update\",\"state\":{\"reported\":{\"2right_control\":69,\"1left_control\":96}},\"version\":24488}"
//阿里云三元组(自己的)
//#define ProductKey "gckbkat1Fdy"  //产品名称
//#define DeviceName  "PDL180"  //设备名称
//#define DeviceSecret    "5a4e03e8e6ffc514a9208429024ebc30"  //token值
//#define User_MQTTServer_IP     "gckbkat1Fdy.iot-as-mqtt.cn-shanghai.aliyuncs.com"     		//MQTT本地服务器IP或者域名
//#define User_MQTTServer_PORT   1883     																									//服务器端口号
//#define User_client_id    	"PDL180|securemode=3,signmethod=hmacsha1,timestamp=60|"   	//MQTTclientID 用于标志client身份  最长256字节
//#define User_username     	"PDL180&gckbkat1Fdy"																			//用于登录 MQTT 服务器 的 username, 最长 64 字节	
//#define User_password				"03515AD9E24B03525108F22E68BE299FE0892F80"          			//用于登录 MQTT 服务器 的 password, 最长 64 字节

//阿里云三元组(公司的)	三元组转换密码连接【http://mqtt.lovemcu.cn/】
#define ProductKey "gbysTFFDuqt"  //产品名称
#define DeviceName  "PDL180"  //设备名称
//#define DeviceSecret    "9a431448f76b17adf8a7948e9eefc792"  //token值

#define User_MQTTServer_IP     "gbysTFFDuqt.iot-as-mqtt.cn-shanghai.aliyuncs.com"     		//MQTT本地服务器IP或者域名
#define User_MQTTServer_PORT   1883     																									//服务器端口号
#define User_client_id    	"PDL180|securemode=3,signmethod=hmacmd5|"   	//MQTTclientID 用于标志client身份  最长256字节
#define User_username     	"PDL180&gbysTFFDuqt"																			//用于登录 MQTT 服务器 的 username, 最长 64 字节	
#define User_password				"FCF547A6B5A044863A0E2F094983C0D1"          			//用于登录 MQTT 服务器 的 password, 最长 64 字节

//以下topic暂未用到
//自定义Topic
#define User_MQTTServer_Topic  						"/a1TGuTWhjR8/TestDevice/user/update"  	

//属性上报	AT+MQTTSUB=<topic>,<qos>
#define User_MQTTServer_Topic_Property_Post			"/sys/gbysTFFDuqt/PDL180/thing/event/property/post"
#define User_MQTTServer_Topic_Property_Post_Reply	"/sys/gbysTFFDuqt/PDL180/thing/event/property/post_reply"

//设备影子	
#define User_MQTTshadow_Topic_Property_Post_update	"/shadow/update/gbysTFFDuqt/PDL180"
#define User_MQTTshadow_Topic_Property_Post_get	 "/shadow/get/gbysTFFDuqt/PDL180"

#define User_MQTTshadow_Topic_Property_Post_push "/sys/gbysTFFDuqt/PDL180/thing/config/push"
//事件上报
#define User_MQTTServer_Topic_Event_Post			"/sys/gckblNBG4eP/PDL180/thing/event/{tsl.event.identifer}/post"
#define User_MQTTServer_Topic_Event_Post_Reply		"/sys/gckblNBG4eP/PDL180/thing/event/{tsl.event.identifer}/post_reply"

//属性设置
#define User_MQTTServer_Topic_Property_Set			"/sys/gckblNBG4eP/PDL180/thing/service/property/set"
#define User_MQTTServer_Topic_Property_Set_Reply	"/sys/gckblNBG4eP/PDL180/thing/service/property/set_reply"

//服务调用
#define User_MQTTServer_Topic_Service_Post			"/sys/gckblNBG4eP/PDL180/thing/service/{tsl.service.identifer}"
#define User_MQTTServer_Topic_Service_Post_Reply	"/sys/gckblNBG4eP/PDL180/thing/service/{tsl.service.identifer}_reply"

void Clear_Buffer(void)//清空缓存
{
		u16 i;
		Uart1_SendStr(RxBuffer);
		for(i=0;i<900;i++)
		RxBuffer[i]=0;//缓存
		
		RxCounter=0;
		//memset(RxBuffer,0,sizeof(RxBuffer));//为什么不行
  	IWDG_Feed();//喂狗
}
void Clear_USART1_Buffer(void)//清空缓存
{
		u32 i;
		for(i=0;i<2000;i++)
		USART_RX_BUF[i]=0;
			USART_RX_STA=0;	
}
void ML302_Init(void)
{
		Uart1_SendStr("本次测试是发数据到MQTT服务器功能\r\n");
	  delay_ms(300);
    printf("AT\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT\r\n"); 
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK			
    }
    printf("ATE0&W\r\n"); //关闭回显
    delay_ms(300);
		printf("AT+MQTTDISC\r\n");
		delay_ms(300);
		printf("AT+MQTTDEL\r\n");//释放资源
		delay_ms(300);
    printf("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"460");//返460，表明识别到卡了
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"460");//返回OK,说明卡是存在的
    }
        printf("AT+CGDCONT=2,\"IP\",\"CMNET\"\r\n");//设置APN参数
        delay_ms(300);
        Clear_Buffer();	
				printf("AT+CGACT=1,1\r\n");//激活网络，PDP
		    delay_ms(300);
        Clear_Buffer();	
				printf("AT+CSTT=\"CMNET\",\"\",\"\" \r\n");//开启网络
	      delay_ms(300);
        Clear_Buffer();	
		    printf("AT+CIICR\r\n");//启动网络接入
			  delay_ms(300);
        Clear_Buffer();	
        printf("AT+CGATT?\r\n");//查询激活状态
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//返1
        Clear_Buffer();	
			while(strx==NULL)
			{
							Clear_Buffer();	
							printf("AT+CGATT?\r\n");//获取激活状态
							delay_ms(300);
							strx=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//返回1,表明注网成功
			}
				 printf("AT+CSQ\r\n");//查询信号
				 delay_ms(300);
				 Clear_Buffer();	 
}


//初始化MQTT
void ML302_MQTTInit(void)
{
	printf("AT+MQTTCFG=\"%s\",1883,\"%s\",60,\"%s\",\"%s\",0,\"\",0\r\n",User_MQTTServer_IP,User_client_id,User_username,User_password);
    delay_ms(300);
    Clear_Buffer();	
    printf("AT+MQTTOPEN=1,1,0,0,0,\"\",\"\"\r\n");//连接到阿里云平台
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+MQTTOPEN:OK");//返登录成功
    while(strx==NULL)
    { 
				Uart1_SendStr("未查询到 MQTTOPEN:OK,正在重新查询...\r\n");
			 delay_ms(1000);
       strx=strstr((const char*)RxBuffer,(const char*)"+MQTTOPEN:OK");//登录到阿里云成功 此时设备会显示在线  
    }
    Clear_Buffer();	
		
//		printf("AT+MQTTSUB=\"%s\",1\r\n",User_MQTTServer_Topic );//连接到阿里云平台，订阅事件
//    delay_ms(300);
//		strx=strstr((const char*)RxBuffer,(const char*)"+MQTTSUBACK:2,1,dev/gsm/u OK");//返回OK
//		Clear_Buffer();	

//ML302_MQTT_Topic_Init();
}

void ML302_MQTT_Topic_Init(void)
{
		printf("AT+MQTTSUB=\"%s\",2\r\n",User_MQTTServer_Topic_Property_Post_Reply );//连接到阿里云平台，订阅事件
    delay_ms(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+MQTTSUBACK:2,2,dev/gsm/u OK");//返回OK
		Clear_Buffer();	

//		printf("AT+MQTTSUB=\"%s\",1",User_MQTTshadow_Topic_Property_Post_get );//连接到阿里云平台，订阅事件
//    delay_ms(300);
//		strx=strstr((const char*)RxBuffer,(const char*)"+MQTTSUBACK:2,2,dev/gsm/u OK");//返回OK
//		Clear_Buffer();	
//				printf("AT+MQTTSUB=\"%s\",1",User_MQTTServer_Topic_Property_Post );//连接到阿里云平台，订阅事件
//    delay_ms(300);
//		strx=strstr((const char*)RxBuffer,(const char*)"+MQTTSUBACK:2,2,dev/gsm/u OK");//返回OK
//		Clear_Buffer();	
//				printf("AT+MQTTSUB=\"%s\",1",User_MQTTServer_Topic_Property_Post_Reply );//连接到阿里云平台，订阅事件
//    delay_ms(300);
//		strx=strstr((const char*)RxBuffer,(const char*)"+MQTTSUBACK:2,2,dev/gsm/u OK");//返回OK
//		Clear_Buffer();	
}

void ML302_CMD(void)
{
	if (strstr ((const char*)USART_RX_BUF,(const char*)"AT" )	)
	{
		u1_printf ("AT\r\n");
    printf("AT\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    Clear_Buffer();	
		Clear_USART1_Buffer();
	}
		if(strstr ((const char*)USART_RX_BUF,(const char*)"shadow_update" )	)	//有问题
	{	
		printf("AT+MQTTSUB=\"%s\",1\r\n",User_MQTTshadow_Topic_Property_Post_update );//连接到阿里云平台，订阅事件
		delay_ms(300);
		Clear_Buffer();
		Clear_USART1_Buffer();
	}
	
	if(strstr ((const char*)USART_RX_BUF,(const char*)"shadow_get" )	)
	{	
		printf("AT+MQTTSUB=\"%s\",1\r\n",User_MQTTshadow_Topic_Property_Post_get );//连接到阿里云平台，订阅事件
		delay_ms(300);
		Clear_Buffer();
		Clear_USART1_Buffer();
	}

	if(strstr ((const char*)USART_RX_BUF,(const char*)"thing_config_push" )	)
	{	
		printf("AT+MQTTSUB=\"%s\",1\r\n",User_MQTTshadow_Topic_Property_Post_push );//连接到阿里云平台，订阅事件
		delay_ms(300);
		Clear_Buffer();
		Clear_USART1_Buffer();
	}
	
		if(strstr ((const char*)USART_RX_BUF,(const char*)"ext_request" )	)
	{	
		printf("AT+MQTTSUB=\"%s\",1\r\n",User_MQTTclock_Topic_Property_Post_request );//连接到阿里云平台，订阅事件
		delay_ms(300);
		Clear_Buffer();
		Clear_USART1_Buffer();
	}

	if(strstr ((const char*)USART_RX_BUF,(const char*)"ext_response" )	)
	{	
		printf("AT+MQTTSUB=\"%s\",1\r\n",User_MQTTclock_Topic_Property_Post_response );//连接到阿里云平台，订阅事件
		delay_ms(300);
		Clear_Buffer();
		Clear_USART1_Buffer();
	}
	
	if (strstr ((const char*)USART_RX_BUF,(const char*)"+MQTTSUB" )	)
	{
			u1_printf ("查询订阅情况");
			printf("AT+MQTTSUB?\r\n");//连接到阿里云平台，订阅事件
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
/*函数名：ML302_shadow       				               */
/*函数名：设备影子								                 */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void ML302_shadow(void)
{
//#define User_MQTTshadow_Topic_Property_Post_update	"/shadow/update/gbysTFFDuqt/PDL180"
//#define User_MQTTshadow_Topic_Property_Post_get	 "/shadow/get/gbysTFFDuqt/PDL180"

//	User_MQTTshadow_Topic_Property_Post_update	User_MQTTshadow_Topic_Property_Post_get
u16 right_data,left_data;
		static u16 version=24480;
	//AT+MQTTPUB="/shadow/update/gbysTFFDuqt/PDL180",1,0,0,"{\"method\":\"update\",\"state\":{\"reported\":{\"2right_control\":88}},\"version\":24270}"
		char shadow_buf_json_first[] = "AT+MQTTPUB=\"/shadow/update/%s/%s\",1,0,0,%s\r\n";		//上报状态到影子
		char topic_buf_json_first[400];//shadow_buf_json_first包
		char shadow_buf_json_secend[]	=	"{\"method\":\"update\",\"state\":{\"reported\":{\"2right_control\":%d,\"1left_control\":%d}},\"version\":%d}";		
		char topic_buf_json_secend[400];//shadow_buf_json_secend包
	//char shadow_buf_json_secend[]="{\"method\":\"control\",\"payload\":{\"state\":{\"desired\":{\"2right_control\":0,\"1left_control\":10},\"reported\":{\"2right_control\":30,\"1left_control\":10}},\"metadata\":{\"desired\":{\"2right_control\":{\"timestamp\":1642338876},\"1left_control\":{\"timestamp\":1642338876}},\"reported\":{\"2right_control\":{\"timestamp\":1642338876},\"1left_control\":{\"timestamp\":1642338876}}}},\"timestamp\":1642338876,\"version\":28}";
		
	//char shadow_buf_json_secend[]	=	"{\"method\": \"update\", \"state\": {\"reported\": {\"color\": \"red\"}}, \"version\":%d}";

//		char t_json[400];//↑字符串拼接
//		char topic_buf[400];	
//		char json_len;
		
				sprintf(topic_buf_json_secend,shadow_buf_json_secend,right_data,left_data,version);//
				
				sprintf(topic_buf_json_first,shadow_buf_json_first,ProductKey,DeviceName,topic_buf_json_secend);	//topic_buf_json_first	:		
//				strcat(topic_buf,t_json);								//拼接两个字符串

/*发送*/	printf("AT+MQTTPUB=\"/shadow/update/%s/%s\",1,0,0,%s\r\n",ProductKey,DeviceName,shadow_topic);
				u1_printf("AT+MQTTPUB=\"/shadow/update/%s/%s\",1,0,0,%s\r\n",ProductKey,DeviceName,shadow_topic);
				//printf("%s\r\n",topic_buf);
				version++;
				
//			AT+MQTTPUB="/sys/gckblNBG4eP/ML302_/thing/event/property/post",1,0,0,"{\"method\":\"thing.event.property.post\",\"id\":\"2012934115\",\"params\":{\"PM25\":55},\"version\":\"1.0.0\"}"
//			AT+MQTTPUB="/shadow/update/gbysTFFDuqt/PDL180",1,0,0,0{"method":"get"}	
//			AT+MQTTPUB="/shadow/update/gbysTFFDuqt/PDL180",1,0,0,"{\"method\":\"update\",\"state\":{\"reported\":{\"2right_control\":88}},\"version\":24285}"
//[发送]AT+MQTTPUB="/shadow/update/gbysTFFDuqt/PDL180",1,0,0,"{\"method\":\"update\",\"state\":{\"reported\":{\"2right_control\":88}},\"version\":24470}"
//[接收]+MQTTPUBLISH: 0,1,0,61111,"/shadow/get/gbysTFFDuqt/PDL180",109,{"method":"reply","payload":{"status":"success","version":24470},"clientToken":"null","timestamp":1642487387}
//[接收]+MQTTPUBLISH: 0,1,0,6800,"/shadow/get/gbysTFFDuqt/PDL180",328,{"method":"control","payload":{"state":{"desired":{"2right_control":50,"1left_control":40},"reported":{"2right_control":88}},"metadata":{"desired":{"2right_control":{"timestamp":1642487775},"1left_control":{"timestamp":1642487775}},"reported":{"2right_control":{"timestamp":1642487775}}}},"timestamp":1642487775,"version":24471}
//AT+MQTTPUB="/shadow/update/gbysTFFDuqt/PDL180",1,0,0,"{\"method\":\"update\",\"state\":{\"reported\":{\"2right_control\":88}},\"version\":24480}"
}

/*-------------------------------------------------*/			
/*函数名：ML302_MQTTPUBMessage                     */
/*函数名：向服务器发送数据				                 */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
//发布数据到服务器

			 //	 	itoa(value,str,10);//数字转字符串函数itoa,10代表十进制。要包含头文件<stdio.h>
			 // 	/sys/gckbkat1Fdy/${deviceName}/thing/event/property/post
			 //		AT+MQTTPUB="/sys/gckblNBG4eP/ML302_/thing/event/property/post",1,0,0,"{\"method\":\"thing.event.property.post\",\"id\":\"2012934115\",\"params\":{\"PM25\":55},\"version\":\"1.0.0\"}"

void ML302_MQTTPUBMessage(u32 temp)
{
	float ADC1_CH9_PB1_DATA;		//	ADC采集的电压值
	float input_voltage;				//数采仪输入电压
	u16 ADC1_CH9_PB1_REV;				//ADC采集数据
	float atemp;
	float arh;
	
		char json[]= "{\"method\":\"thing.event.property.post\",\"id\":\"2012934115\",\"params\":{\"voltage\":%.2f,\"temp\":%.2f,\"rh\":%.2f,\"WDIR\":%d,\"WSPD\":3},\"version\":\"1.0.0\"}";//数据以json格式打包
		char t_json[400];
		char json_len;

		char t_buf[]= "AT+MQTTPUB=\"/sys/%s/%s/thing/event/property/post\",1,0,0,%d\r\n";		//topic buf 
		char topic_buf[400];
					
			VOL_5V_UART5=1;	

			Read_sht31();
			Convert_sht31();

//监测12v口电压  PB1  并打包数据
				ADC1_CH9_PB1_REV=Get_Adc_Average(ADC_Channel_9,100);
				ADC1_CH9_PB1_DATA=(float)ADC1_CH9_PB1_REV*(3.3/4096);	//获得adc值，12位最大为4096	
//				input_voltage = ADC1_CH9_PB1_DATA / 0.047;							//计算实际输入电压值，采集电压值与实际输入电压值比例为0.047	（PDL180）
				input_voltage = ADC1_CH9_PB1_DATA / 0.166;							//计算实际输入电压值，采集电压值与实际输入电压值比例为0.166	（PDL280）
				
				atemp=tem;
				arh=hum;

		RS485_sensor_data();		
//							u1_printf("longitude:%f\r\n",longitude);//发布数据到平台
//							
//							u1_printf("lattitude:%f\r\n",latitude);	//发布数据到平台		
				sprintf(t_json, json, input_voltage,  atemp, arh ,WDIR_data);

				json_len = strlen(t_json)/sizeof(char);	

				sprintf(topic_buf,t_buf,ProductKey,DeviceName,json_len);
				strcat(topic_buf,t_json);								//拼接两个字符串
	 
			printf("%s",topic_buf);//发布数据到平台
			u1_printf("u1_printf:%s",topic_buf);//发布数据到平台
			
      delay_ms(300);
//			strx=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBACK:0");//返回发布成功
//    while(strx==NULL)
//    { 
//       Uart1_SendStr("未查询到 MQTTPUBACK:0,正在重新查询...\r\n");
//			 delay_ms(1000);
//			 strx=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBACK:0");//发布数据成功
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
		char json[]= "{\"method\":\"thing.event.property.post\",\"id\":\"2012934115\",\"params\":{\"Speed\":%.2f,\"gps\":%d,\"beidou\":%d,\"jingdu\":%.5f,\"weidu\":%.5f,\"high\":%d,\"Remote\":%d},\"version\":\"1.0.0\"}";//数据以json格式打包
		char t_json[400];
		char json_len;

		char t_buf[]= "AT+MQTTPUB=\"/sys/%s/%s/thing/event/property/post\",1,0,0,%d\r\n";		//topic buf 
		char topic_buf[400];
		
				VOL_5V_UART5=1;	
					

				sprintf(t_json, json, Speed,  gps_qty, bd_qty ,longitude,latitude,high,Remote);

				json_len = strlen(t_json)/sizeof(char);	

				sprintf(topic_buf,t_buf,ProductKey,DeviceName,json_len);
				strcat(topic_buf,t_json);								//拼接两个字符串
	 
			printf("%s",topic_buf);//发布数据到平台
			u1_printf("u1_printf:%s",topic_buf);//发布数据到平台
			
//			strx=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBACK:0");//返回发布成功
//    while(strx==NULL)
//    { 
//       Uart1_SendStr("未查询到 MQTTPUBACK:0,正在重新查询...\r\n");
//			 delay_ms(1000);
//			 strx=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBACK:0");//发布数据成功
//    }
			Clear_Buffer();	

}



//___________________________________________________________________________________________________________________________________
//在线调试控制方式
extern	bool mqttput_float;
/*-------------------------------------------------*/			
/*函数名：ML302_Right_Message                      */
/*函数名：接收阿里云右通道命令				    		     */
/*参  数：无                                       */
/*返回值：ch2_temp                                 */
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
//					u1_printf("ML302_Right_Message在执行\r\n");
			if(strstr((const char*)RxBuffer,(const char*)"control"))
				{	mqttput_float=0;
							strx=strstr((const char*)RxBuffer,(const char*)"{");//获取到第一个{
							for(i=0;;i++)
							{
									if(strx[i+1]==0x0d)
									break;
									ML302_Status.recdata[i]=strx[i+1];//获取数据内容		
							}

							strx=strstr((const char*)(strx+1),(const char*)"{");	//获取到第二个{
							for(i=0;;i++)
							{
										if(strx[i+1]==0x7d)			//7D为}
									break;
									ML302_Status.sensordata[i]=strx[i+1];//获取数据内容
																																			//ML302_Status.sensordata[i]内容为	VerticalSwitch":0,"AdasSwitch":1							
							}
					if(strstr((const char*)ML302_Status.sensordata,(const char*)"\"right_control\":"))
							{
							//提取字符串中的数字================================								
									buf =	strstr(ML302_Status.sensordata,cmd_buf);
									
									cmd_buf_len	=	strlen(cmd_buf);							//字符串长度
										 
									strncpy(right_data_buf,buf+cmd_buf_len,3);			//截取"\"right_control\":"后面3位字符
									
									right_data = atoi(right_data_buf);//将buf里的数值转换为十进制输出
									
						  		u1_printf("\r\n right=%d\r\n",right_data);

									if( 0<=right_data && right_data<=100)
										{
											ch2_temp=right_data*10+18000;
										}
									else
									u1_printf("CH2输出值错误。。。");		
//									u1_printf("ch2_temp%d\r\n",ch2_temp);												
							}							
					}										
						u1_printf("ch2_temp::%d\r\n",ch2_temp);		
					return	ch2_temp;
	}

/*-------------------------------------------------*/			
/*函数名：ML302_Left_Message                       */
/*函数名：接收阿里云左通道命令				    		     */
/*参  数：无                                       */
/*返回值：ch3_temp                                 */
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
	
//			u1_printf("ML302_Left_Message在执行\r\n");
//params
				if(strstr((const char*)RxBuffer,(const char*)"control"))
					{
						mqttput_float=0;
						strx=strstr((const char*)RxBuffer,(const char*)"{");//获取到第一个{
						for(i=0;;i++)
						{
								if(strx[i+1]==0x0d)
								break;
								ML302_Status.recdata[i]=strx[i+1];//获取数据内容
						}
						strx=strstr((const char*)(strx+1),(const char*)"{");	//获取到第二个{
						for(i=0;;i++)
						{
								if(strx[i+1]==0x7d)			//7D为}
								break;
								ML302_Status.sensordata[i]=strx[i+1];//获取数据内容
																																			//ML302_Status.sensordata[i]内容为	VerticalSwitch":0,"AdasSwitch":1	
						}		
				//提取字符串中的数字================================
							if(strstr((const char*)ML302_Status.sensordata,(const char*)"\"left_control\":"))
						{
											buf =	strstr(ML302_Status.sensordata,cmd_buf);
										
											cmd_buf_len	=	strlen(cmd_buf);							//字符串长度
												 
											strncpy(left_data_buf,buf+cmd_buf_len,3);			//截取"\"right_control\":"后面3位字符
																									
											left_data = atoi(left_data_buf);
											
											u1_printf("left=%d\r\n",left_data);			
											if( 0<=left_data && left_data<=100)
											{
												ch3_temp=left_data*10+18000;
											}
											else
											u1_printf("CH1输出值错误。。。");		
						}
			}
						u1_printf("ch3_temp::%d\r\n",ch3_temp);		
					return	ch3_temp;
	}
//___________________________________________________________________________________________________________________________________
//更新设备影子控制方式
/*-------------------------------------------------*/			
/*函数名：ML302_Right_Message                      */
/*函数名：接收阿里云右通道命令				    		     */
/*参  数：无                                       */
/*返回值：ch2_temp                                 */
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
///		pos = strstr((const char*)RxBuffer,"desired");//state;{		期望值	//desired":{"right_control":10,"left_control":10},"reported":{"right_control":2,"left_control":22}},"metadata":{"desired":{"	
					
					if(strstr((const char*)buff,(const char*)"\"right_control\":"))
							{
							//提取字符串中的数字================================								
									buf =	strstr(buff,cmd_buf);
									
									cmd_buf_len	=	strlen(cmd_buf);							//字符串长度
										 
									strncpy(right_data_buf,buf+cmd_buf_len,3);			//截取"\"right_control\":"后面3位字符
									
									right_data = atoi(right_data_buf);//将buf里的数值转换为十进制输出
									
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
									u1_printf("CH2输出值错误。。。");		
//									u1_printf("ch2_temp%d\r\n",ch2_temp);												
							}							
					}										
			///			u1_printf("ch2_temp::%d\r\n",ch2_temp);		
					return	ch2_temp;
	}

/*-------------------------------------------------*/			
/*函数名：ML302_Left_Message                       */
/*函数名：接收阿里云左通道命令				    		     */
/*参  数：无                                       */
/*返回值：ch3_temp                                 */
/*-------------------------------------------------*/
u16 State_ML302_Left_Message(char *buff)
{
	char left_data_buf[200];	
	char cmd_buf[]={"\"left_control\":"};
	char *buf;
	u32 cmd_buf_len;
	int left_data;
	static	u32 ch3_temp=flycolor_stop;
	
				//提取字符串中的数字================================
							if(strstr((const char*)buff,(const char*)"\"left_control\":"))	//strstr返回字符串中首次出现子串的地址。
								{
										buf =	strstr(buff,cmd_buf);	
									
										cmd_buf_len	=	strlen(cmd_buf);							//字符串长度
											 
										strncpy(left_data_buf,buf+cmd_buf_len,3);			//截取"\"right_control\":"后面3位字符
																								
										left_data = atoi(left_data_buf);
										
				///						u1_printf("left=%d\r\n",left_data);			
										if( -100<=left_data && left_data<=100)
										{
												if( left_data <0)
												{
													left_data = abs(left_data);
													ch3_temp = flycolor_stop -(left_data*(flycolor_stop - flycolor_low )/100);///200的原因是这里的范围为-100到100
												}
												else
												{
													ch3_temp = flycolor_stop +(left_data*(flycolor_high - flycolor_stop)/100);///200的原因是这里的范围为-100到100
												}	
										}
										else
										u1_printf("CH1输出值错误。。。");		
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
	
				//提取字符串中的数字================================
							if(strstr((const char*)buff,(const char*)"\"control_time\":"))
						{
											buf =	strstr(buff,cmd_buf);
										
											cmd_buf_len	=	strlen(cmd_buf);							//字符串长度
												 
											strncpy(time_data_buf,buf+cmd_buf_len,2);			//截取"\"right_control\":"后面3位字符
																									
											time_data = atoi(time_data_buf);
											
					///						u1_printf("time_data = %d\r\n",time_data);			
											if( 0<time_data && time_data<=Iot_Contorl_Time)
											{
												time=time_data;
											}
											else
											u1_printf("时间输出值错误。。。");		
						}
				///		u1_printf("time::%d\r\n",time);		
					return	time;
	}
	
	//字符串提取函数
	void char_data(void)
	{
	
	int length;
	int length_reav;

	char *pos ;

	if(strstr((const char*)RxBuffer,(const char*)"shadow"))
		{
	/*检索两个字符间的字符串*/	
	//		pos = strstr((const char*)RxBuffer,"desired");//state;{
	//		length = strlen(pos);

	//		pos_reav = strstr((const char*)pos,"metadata");//metadata
	//		length_reav = strlen(pos_reav);

	//		pos[length-length_reav] = '\0';
	//		pos ++;
	//		u1_printf("POS：%s\r\n",pos);
	//		u1_printf("BUF:	%s\r\n",RxBuffer);
	//		Clear_Buffer();	

/*检索出需要的字符串位置*/
		pos = strstr((const char*)RxBuffer,"reported");//desired	state;{		期望值	//desired":{"right_control":10,"left_control":10},"reported":{"right_control":2,"left_control":22}},"metadata":{"desired":{"
				length= State_ML302_Right_Message(pos);
				length_reav = State_ML302_Left_Message(pos);
//		pos_reav = strstr((const char*)pos,"reported");//metadata		下发值	reported":{"right_control":2,"left_control":22}},"metadata":{"desired":{"
//				State_ML302_Right_Message(pos_reav);
//				State_ML302_Left_Message(pos_reav);
//		u1_printf("POS：%s\r\n",pos);	
//		u1_printf("POS——REAV：%s\r\n",pos_reav);
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
/*检索出需要的字符串位置*/
////				pos = strstr((const char*)RxBuffer,"desired");//state;{		期望值	//desired":{"right_control":10,"left_control":10},"reported":{"right_control":2,"left_control":22}},"metadata":{"desired":{"
////				State_ML302_Right_Message(pos);
				pos_reav = strstr((const char*)RxBuffer,"desired");//metadata		下发值	reported":{"right_control":2,"left_control":22}},"metadata":{"desired":{"

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
/*检索出需要的字符串位置*/
				
////				pos = strstr((const char*)RxBuffer,"desired");//state;{		期望值	//desired":{"right_control":10,"left_control":10},"reported":{"right_control":2,"left_control":22}},"metadata":{"desired":{"
////				State_ML302_Left_Message(pos);				
				pos_reav = strstr((const char*)RxBuffer,"desired");//metadata		下发值	reported":{"right_control":2,"left_control":22}},"metadata":{"desired":{"
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
		/*检索出需要的字符串位置*/
				pos = strstr((const char*)RxBuffer,"desired");//metadata		下发值	reported":{"right_control":2,"left_control":22}},"metadata":{"desired":{"
				time_data = ML302_Time_Message(pos);
			}

			return time_data;
}

/*自写字符串提取函数，提取“”里内容*/

//char buff_json(const char *buff)
//{
//	if(strstr((const char*)RxBuffer,(const char*)" \"\ "))
//		u1_printf("	%s\r\n",);
//}

