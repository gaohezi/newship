//#ifndef UART5_H_
//#define UART5_H_

//#include "sys.h"
//#include "stdio.h"	
//#include "usart.h"

//#define UART5_REC_LEN  200  	//�����������ֽ��� 200


//extern u8  UART5_RX_BUF[UART5_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�
//extern u16 UART5_RX_STA;         		//����״̬���	 0-14λΪ�����ֽ�����15λ������ɱ�־λ
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

#define UART5_MAX_RECV_LEN		600					//�����ջ����ֽ���
#define UART5_MAX_SEND_LEN		600					//����ͻ����ֽ���
#define UART5_RX_EN 			1					//0,������;1,����.


#define UART5_REC_LEN  2000  	//�����������ֽ��� 200

///extern u8  UART5_RX_BUF[UART5_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�


extern u8  UART5_RX_BUF[UART5_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern u8  UART5_TX_BUF[UART5_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern u16 UART5_RX_STA;         		//����״̬���	 0-14λΪ�����ֽ�����15λ������ɱ�־λ
void uart5_init(u32 bound);



void UART5_send_byte(char data);
void Uart5_SendStr(char*SendBuf);
void CopeSerial2Data(unsigned char ucData);



#endif
