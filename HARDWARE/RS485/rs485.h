#ifndef _rs485_H
#define _rs485_H

#include "sys.h"

#define  WDIR_ADD  0x21  //�����ַ

extern u8 RS485_RX_BUF[164]; 		//���ջ���,���64���ֽ�
extern u8 RS485_RX_CNT;   			//���յ������ݳ���

//ģʽ����
#define RS485_TX_EN		PCout(1)	//485ģʽ����.0,����;1,����.
//����봮���жϽ��գ��벻Ҫע�����º궨��
#define EN_USART2_RX 	1			//0,������;1,����.



u16 Modbus_Crc(u8 *puchMsg,u16  usDataLen) ;
void RS485_Init(u32 bound);
void RS485_Send_Data(u8 *buf,u8 len);
void RS485_Receive_Data(u8 *buf,u8 *len);

void rs485_sensor(void);
	void RS485_sensor_data(void);
#endif	   





