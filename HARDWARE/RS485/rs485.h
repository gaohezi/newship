#ifndef _rs485_H
#define _rs485_H

#include "sys.h"

#define  WDIR_ADD  0x21  //风向地址

extern u8 RS485_RX_BUF[164]; 		//接收缓冲,最大64个字节
extern u8 RS485_RX_CNT;   			//接收到的数据长度

//模式控制
#define RS485_TX_EN		PCout(1)	//485模式控制.0,接收;1,发送.
//如果想串口中断接收，请不要注释以下宏定义
#define EN_USART2_RX 	1			//0,不接收;1,接收.



u16 Modbus_Crc(u8 *puchMsg,u16  usDataLen) ;
void RS485_Init(u32 bound);
void RS485_Send_Data(u8 *buf,u8 len);
void RS485_Receive_Data(u8 *buf,u8 *len);

void rs485_sensor(void);
	void RS485_sensor_data(void);
#endif	   






