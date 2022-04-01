#ifndef __MAX44009_H
#define __MAX44009_H

/*--------------------------头文件引用--------------------------------*/
//#include "StdInc.h"
#include "system.h"

/*-----------------------------结构体定义-----------------------------*/
															
/*-----------------------------宏定义---------------------------------*/
   	   		   
//IO方向设置
#define SDA_IN()  {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=8<<12;}
#define SDA_OUT() {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=3<<12;}
#define B_LUX_V32D_SDA0_DAT  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)
//IO操作函数	 
#define IIC_SCL    PCout(10) //SCL
#define IIC_SDA    PCout(11) //SDA	 
#define READ_SDA   PCin(11)  //输入SDA 
//引脚定义
#define B_LUX_V32D_SCL0_O    {\
															GPIO_InitTypeDef  GPIO_ST; \
															GPIO_ST.GPIO_Pin = GPIO_Pin_10;\
															GPIO_ST.GPIO_Mode = GPIO_Mode_Out_OD; \
															GPIO_ST.GPIO_Speed = GPIO_Speed_50MHz;\
															GPIO_Init(GPIOB, &GPIO_ST); }									//GPIOB10 开漏输出
#define B_LUX_V32D_SCL0_H    GPIO_SetBits(GPIOB, GPIO_Pin_10)
#define B_LUX_V32D_SCL0_L    GPIO_ResetBits(GPIOB, GPIO_Pin_10)

#define B_LUX_V32D_SCL0_I    {\
															GPIO_InitTypeDef  GPIO_ST; \
															GPIO_ST.GPIO_Pin = GPIO_Pin_10;\
															GPIO_ST.GPIO_Mode = GPIO_Mode_IN_FLOATING; \
															GPIO_ST.GPIO_Speed = GPIO_Speed_50MHz;\
															GPIO_Init(GPIOB, &GPIO_ST); }									//GPIOB10 浮空输入
#define B_LUX_V32D_SCL0_DAT  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)

#define B_LUX_V32D_SDA0_O    {\
															GPIO_InitTypeDef  GPIO_ST; \
															GPIO_ST.GPIO_Pin = GPIO_Pin_11;\
															GPIO_ST.GPIO_Mode = GPIO_Mode_Out_OD; \
															GPIO_ST.GPIO_Speed = GPIO_Speed_50MHz;\
															GPIO_Init(GPIOB, &GPIO_ST); }    							//GPIOB11 开漏输出
#define B_LUX_V32D_SDA0_H    GPIO_SetBits(GPIOB, GPIO_Pin_11)
#define B_LUX_V32D_SDA0_L    GPIO_ResetBits(GPIOB, GPIO_Pin_11)

#define B_LUX_V32D_SDA0_I    {\
															GPIO_InitTypeDef  GPIO_ST; \
															GPIO_ST.GPIO_Pin = GPIO_Pin_11;\
															GPIO_ST.GPIO_Mode = GPIO_Mode_IN_FLOATING; \
															GPIO_ST.GPIO_Speed = GPIO_Speed_50MHz;\
															GPIO_Init(GPIOB, &GPIO_ST); }    							//GPIOB11 浮空输入
#define B_LUX_V32D_SDA0_DAT  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)

#define B_LUX_V32D_ADDR_O    {\
															GPIO_InitTypeDef  GPIO_ST; \
															GPIO_ST.GPIO_Pin = GPIO_Pin_13;\
															GPIO_ST.GPIO_Mode = GPIO_Mode_Out_PP; \
															GPIO_ST.GPIO_Speed = GPIO_Speed_50MHz;\
															GPIO_Init(GPIOC, &GPIO_ST); }    							//GPIOC13 推免输出
#define B_LUX_V32D_ADDR_H    GPIO_SetBits(GPIOC, GPIO_Pin_13)
#define B_LUX_V32D_ADDR_L    GPIO_ResetBits(GPIOC, GPIO_Pin_13)

#define B_LUX_V32D_INT0_I    {\
															GPIO_InitTypeDef  GPIO_ST; \
															GPIO_ST.GPIO_Pin = GPIO_Pin_5;\
															GPIO_ST.GPIO_Mode = GPIO_Mode_IN_FLOATING; \
															GPIO_ST.GPIO_Speed = GPIO_Speed_50MHz;\
															GPIO_Init(GPIOB, &GPIO_ST); }    							//GPIOB5 浮空输入
#define B_LUX_V32D_INT0_DAT  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)


#define	B_LUX_V32D_SlaveAddress	  		0x94                                                  //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
#define B_LUX_V32D_CONFIGURATION      0x00                                                  //默认模式每800MS采集一次， 自动模式

/*-----------------------------函数声明-------------------------------*/
void    B_LUX_V32D_delay_ms(u16 k);
u8    B_LUX_V32D_Init(void);
u8    B_LUX_V32D_RegCfg(u8 val);

u8   B_LUX_V32D_Single_Write(u8 REG_Address, u8 REG_data);                     //单个写入数据
u8   B_LUX_V32D_Single_Read(u8 REG_Address, u8 *vp_Dat);                       //单个读取内部寄存器数据
extern u8   B_LUX_V32D_Multi_read(u8 REG_Address1, u8 REG_Address2, u8 *vBuf);        //连续的读取内部寄存器数据
//------------------------------------
void    B_LUX_V32D_Delay5us(void);
void    B_LUX_V32D_Delay5ms(void);
void    B_LUX_V32D_Start(void);                                                                 //起始信号
void    B_LUX_V32D_Stop(void);                                                                  //停止信号
void    B_LUX_V32D_SendACK(u8 ack);                                                         //应答ACK
u8   B_LUX_V32D_RecvACK(void);                                                               //读ack
u8   B_LUX_V32D_SendByte(u8 dat);                                                        //IIC单个字节写
u8   B_LUX_V32D_RecvByte(void);                                                              //IIC单个字节读

u8   B_LUX_V32D_GetLux(u32 *vLux);
#endif
