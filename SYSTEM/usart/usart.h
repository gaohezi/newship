#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 



#include "stdio.h"      //������Ҫ��ͷ�ļ�
#include "stdarg.h"		//������Ҫ��ͷ�ļ� 
#include "string.h"     //������Ҫ��ͷ�ļ�

#define USART1_RX_ENABLE     0      //�Ƿ������չ���  1������  0���ر�
#define USART1_TXBUFF_SIZE   256    //���崮��1 ���ͻ�������С 256�ֽ�

#if  USART1_RX_ENABLE                          //���ʹ�ܽ��չ���
#define USART1_RXBUFF_SIZE   256               //���崮��1 ���ջ�������С 256�ֽ�
extern char Usart1_RxCompleted ;               //�ⲿ�����������ļ����Ե��øñ���
extern unsigned int Usart1_RxCounter;          //�ⲿ�����������ļ����Ե��øñ���
extern char Usart1_RxBuff[USART1_RXBUFF_SIZE]; //�ⲿ�����������ļ����Ե��øñ���
#endif

void Usart1_Init(unsigned int);     //����1 ��ʼ������
void u1_printf(char*,...) ;         //����1 printf����


//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/8/18
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮������������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);

#endif

