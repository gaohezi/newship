#ifndef __MAX44009_H
#define __MAX44009_H

/*--------------------------ͷ�ļ�����--------------------------------*/
//#include "StdInc.h"
#include "system.h"

/*-----------------------------�ṹ�嶨��-----------------------------*/
															
/*-----------------------------�궨��---------------------------------*/
   	   		   
//IO��������
#define SDA_IN()  {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=8<<12;}
#define SDA_OUT() {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=3<<12;}
#define B_LUX_V32D_SDA0_DAT  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)
//IO��������	 
#define IIC_SCL    PCout(10) //SCL
#define IIC_SDA    PCout(11) //SDA	 
#define READ_SDA   PCin(11)  //����SDA 
//���Ŷ���
#define B_LUX_V32D_SCL0_O    {\
															GPIO_InitTypeDef  GPIO_ST; \
															GPIO_ST.GPIO_Pin = GPIO_Pin_10;\
															GPIO_ST.GPIO_Mode = GPIO_Mode_Out_OD; \
															GPIO_ST.GPIO_Speed = GPIO_Speed_50MHz;\
															GPIO_Init(GPIOB, &GPIO_ST); }									//GPIOB10 ��©���
#define B_LUX_V32D_SCL0_H    GPIO_SetBits(GPIOB, GPIO_Pin_10)
#define B_LUX_V32D_SCL0_L    GPIO_ResetBits(GPIOB, GPIO_Pin_10)

#define B_LUX_V32D_SCL0_I    {\
															GPIO_InitTypeDef  GPIO_ST; \
															GPIO_ST.GPIO_Pin = GPIO_Pin_10;\
															GPIO_ST.GPIO_Mode = GPIO_Mode_IN_FLOATING; \
															GPIO_ST.GPIO_Speed = GPIO_Speed_50MHz;\
															GPIO_Init(GPIOB, &GPIO_ST); }									//GPIOB10 ��������
#define B_LUX_V32D_SCL0_DAT  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)

#define B_LUX_V32D_SDA0_O    {\
															GPIO_InitTypeDef  GPIO_ST; \
															GPIO_ST.GPIO_Pin = GPIO_Pin_11;\
															GPIO_ST.GPIO_Mode = GPIO_Mode_Out_OD; \
															GPIO_ST.GPIO_Speed = GPIO_Speed_50MHz;\
															GPIO_Init(GPIOB, &GPIO_ST); }    							//GPIOB11 ��©���
#define B_LUX_V32D_SDA0_H    GPIO_SetBits(GPIOB, GPIO_Pin_11)
#define B_LUX_V32D_SDA0_L    GPIO_ResetBits(GPIOB, GPIO_Pin_11)

#define B_LUX_V32D_SDA0_I    {\
															GPIO_InitTypeDef  GPIO_ST; \
															GPIO_ST.GPIO_Pin = GPIO_Pin_11;\
															GPIO_ST.GPIO_Mode = GPIO_Mode_IN_FLOATING; \
															GPIO_ST.GPIO_Speed = GPIO_Speed_50MHz;\
															GPIO_Init(GPIOB, &GPIO_ST); }    							//GPIOB11 ��������
#define B_LUX_V32D_SDA0_DAT  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)

#define B_LUX_V32D_ADDR_O    {\
															GPIO_InitTypeDef  GPIO_ST; \
															GPIO_ST.GPIO_Pin = GPIO_Pin_13;\
															GPIO_ST.GPIO_Mode = GPIO_Mode_Out_PP; \
															GPIO_ST.GPIO_Speed = GPIO_Speed_50MHz;\
															GPIO_Init(GPIOC, &GPIO_ST); }    							//GPIOC13 �������
#define B_LUX_V32D_ADDR_H    GPIO_SetBits(GPIOC, GPIO_Pin_13)
#define B_LUX_V32D_ADDR_L    GPIO_ResetBits(GPIOC, GPIO_Pin_13)

#define B_LUX_V32D_INT0_I    {\
															GPIO_InitTypeDef  GPIO_ST; \
															GPIO_ST.GPIO_Pin = GPIO_Pin_5;\
															GPIO_ST.GPIO_Mode = GPIO_Mode_IN_FLOATING; \
															GPIO_ST.GPIO_Speed = GPIO_Speed_50MHz;\
															GPIO_Init(GPIOB, &GPIO_ST); }    							//GPIOB5 ��������
#define B_LUX_V32D_INT0_DAT  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)


#define	B_LUX_V32D_SlaveAddress	  		0x94                                                  //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�
#define B_LUX_V32D_CONFIGURATION      0x00                                                  //Ĭ��ģʽÿ800MS�ɼ�һ�Σ� �Զ�ģʽ

/*-----------------------------��������-------------------------------*/
void    B_LUX_V32D_delay_ms(u16 k);
u8    B_LUX_V32D_Init(void);
u8    B_LUX_V32D_RegCfg(u8 val);

u8   B_LUX_V32D_Single_Write(u8 REG_Address, u8 REG_data);                     //����д������
u8   B_LUX_V32D_Single_Read(u8 REG_Address, u8 *vp_Dat);                       //������ȡ�ڲ��Ĵ�������
extern u8   B_LUX_V32D_Multi_read(u8 REG_Address1, u8 REG_Address2, u8 *vBuf);        //�����Ķ�ȡ�ڲ��Ĵ�������
//------------------------------------
void    B_LUX_V32D_Delay5us(void);
void    B_LUX_V32D_Delay5ms(void);
void    B_LUX_V32D_Start(void);                                                                 //��ʼ�ź�
void    B_LUX_V32D_Stop(void);                                                                  //ֹͣ�ź�
void    B_LUX_V32D_SendACK(u8 ack);                                                         //Ӧ��ACK
u8   B_LUX_V32D_RecvACK(void);                                                               //��ack
u8   B_LUX_V32D_SendByte(u8 dat);                                                        //IIC�����ֽ�д
u8   B_LUX_V32D_RecvByte(void);                                                              //IIC�����ֽڶ�

u8   B_LUX_V32D_GetLux(u32 *vLux);
#endif