//#ifndef UART5_H_
//#define UART5_H_

//#include "sys.h"
//#include "stdio.h"	
//#include "usart.h"

//#define UART5_REC_LEN  200  	//定义最大接收字节数 200


//extern u8  UART5_RX_BUF[UART5_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节
//extern u16 UART5_RX_STA;         		//接收状态标记	 0-14位为接收字节数，15位接收完成标志位
//void uart5_init(u32 bound);

//void UART5_send_byte(char data);
//void Uart5_SendStr(char*SendBuf);

//void CopeSerial2Data(unsigned char ucData);
//#endif

#ifndef UART5_H_
#define UART5_H_

#include "sys.h"
#include "stdio.h"	
#include "usart.h"
#include "usart3.h"

#define UART5_MAX_RECV_LEN		600					//最大接收缓存字节数
#define UART5_MAX_SEND_LEN		600					//最大发送缓存字节数
#define UART5_RX_EN 			1					//0,不接收;1,接收.


#define UART5_REC_LEN  2000  	//定义最大接收字节数 200

///extern u8  UART5_RX_BUF[UART5_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节


extern u8  UART5_RX_BUF[UART5_MAX_RECV_LEN]; 		//接收缓冲,最大USART3_MAX_RECV_LEN字节
extern u8  UART5_TX_BUF[UART5_MAX_SEND_LEN]; 		//发送缓冲,最大USART3_MAX_SEND_LEN字节
extern u16 UART5_RX_STA;         		//接收状态标记	 0-14位为接收字节数，15位接收完成标志位
void uart5_init(u32 bound);



void UART5_send_byte(char data);
void Uart5_SendStr(char*SendBuf);
void CopeSerial2Data(unsigned char ucData);



#endif

