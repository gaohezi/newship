#include "sys.h"		    
#include "rs485.h"	 
#include "delay.h"
#include <stdio.h>
#include "usart.h"	

 u32 WDIR_data;		//风向数据
#ifdef EN_USART2_RX   	//如果使能了接收


//接收缓存区 	
 u8 RS485_RX_BUF[164];  	//接收缓冲,最大64个字节.
//接收到的数据长度
u8 RS485_RX_CNT=0;   		  
  
void USART2_IRQHandler(void)
{
	u8 res;	    
 
 	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //接收到数据
	{	 
	 			 
		res =USART_ReceiveData(USART2); 	//读取接收到的数据
//		if(RS485_RX_CNT<64)
//		{
			RS485_RX_BUF[RS485_RX_CNT]=res;		//记录接收到的值
			RS485_RX_CNT++;						//接收数据增加1 
//		}		
		//USART_SendData(USART1, RS485_RX_BUF[RS485_RX_CNT]);//向串口1发送数据
	}  	
} 
#endif										 
//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率	  
void RS485_Init(u32 bound)
{  
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);//使能GPIOA,D时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //PD7端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	//PA2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
  GPIO_Init(GPIOA, &GPIO_InitStructure);
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);  

	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,ENABLE);//复位串口2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,DISABLE);//停止复位
 
	
 #ifdef EN_USART2_RX		  	//如果使能了接收
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据长度
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;///奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式

  USART_Init(USART2, &USART_InitStructure); ; //初始化串口
  
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; //使能串口2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
 
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
   
  USART_Cmd(USART2, ENABLE);                    //使能串口 

 #endif

  RS485_TX_EN=0;			//默认为接收模式
 
}

//RS485发送len个字节.
//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void RS485_Send_Data(u8 *buf,u8 len)
{
	u8 t;
	RS485_TX_EN=1;			//设置为发送模式
  	for(t=0;t<len;t++)		//循环发送数据
	{		   
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);	  
		USART_SendData(USART2,buf[t]);
	
	}	 
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);		
	RS485_RX_CNT=0;	  
	RS485_TX_EN=0;				//设置为接收模式	
}
//RS485查询接收到的数据
//buf:接收缓存首地址
//len:读到的数据长度
void RS485_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen=RS485_RX_CNT;
	u8 i=0;
	*len=0;				//默认为0
	delay_ms(10);		//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束
	if(rxlen==RS485_RX_CNT&&rxlen)//接收到了数据,且接收完成了
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
		}	
		
		*len=RS485_RX_CNT;	//记录本次数据长度
		RS485_RX_CNT=0;		//清零
	}
	
}

		u8 rs485buf[100]; 
	 u8 rev_len;
void rs485_sensor( void)
{
u8 data_len;
	u8 l_cmd[8]={0};
	l_cmd[0]=WDIR_ADD;		//l_sensorAddr、、十进制33|16进制0x21
	l_cmd[1]=0x03;
	l_cmd[2]=0x00;
	l_cmd[3]=0x2A;
	l_cmd[4]=0x00;
	l_cmd[5]=0x01;
//	l_cmd[6]=0xA2;
//	l_cmd[7]=0xA2;
	l_cmd[6]=Modbus_Crc(l_cmd,6)/256;				//计算校验
	l_cmd[7]=Modbus_Crc(l_cmd,6)%256;				//计算校验
	
	data_len=sizeof(l_cmd);
	
	RS485_Send_Data(l_cmd,data_len);
 
		
	RS485_Receive_Data(rs485buf,& rev_len);
	
	USART_RX_STA=0;
}

	void RS485_sensor_data(void)
	{

			//============================================		
				/*串口2接收应答命令并向串口1发送数据 */
				RS485_Receive_Data(rs485buf,& rev_len);
				
//					for(i=0;i<rev_len;i++)
//					{
//						USART_SendData(USART1, rs485buf[i]);//向串口1发送数据
//						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
//					}
		 //============================================	计算风速		 
			WDIR_data = (rs485buf[3]*256+rs485buf[4])/10;

//				u1_printf("data:%d\r\n",WDIR_data);
	}
		
					


u16 Modbus_Crc(u8 *buf,u16  datalen)  
{

	u8 auchCRCHi[]=  
	{  
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,  
		0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,  
		0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,  
		0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,  
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,  
		0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,  
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,  
		0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,  
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,  
		0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,  
		0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,  
		0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,  
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,  
		0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,  
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,  
		0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,  
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,  
		0x40  
	};  
  
	u8 auchCRCLo[] =  
	{  
		0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,  
		0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,  
		0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,  
		0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,  
		0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,  
		0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,  
		0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,  
		0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,  
		0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,  
		0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,  
		0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,  
		0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,  
		0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,  
		0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,  
		0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,  
		0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,  
		0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,  
		0x40  
	};    
    u8 uchCRCHi,uchCRCLo;  
    u8 uIndex=0 ;                             /* will index into CRC lookup table */  
    u8 tempp=0;  
    u16 CRCHL=0;  
    uchCRCHi=0xFF ;                             // high byte of CRC initialized   
    uchCRCLo=0xFF ;   
    tempp=0;                           // low byte of CRC initialized   
    while(datalen--)                       // pass through message buffer   
    {  
        uIndex=uchCRCHi^*buf++ ; // calculate the CRC high byte           
        tempp++;  
        uchCRCHi=uchCRCLo^auchCRCHi[uIndex] ;//01-auchCRCHi[254]:0x81;02-0x01;03-0x80;04-0xC0  
        uchCRCLo=auchCRCLo[uIndex] ;                 
    }  
    CRCHL=uchCRCHi;  
    CRCHL=CRCHL<<8;  
    CRCHL|=uchCRCLo;          
    return CRCHL;//(uchCRCHi<<8|uchCRCLo) ;//uchCRCHi:0xa1;uchCRCLo:2B  
} 







































////串口驱动函数，检测数据帧的接收，调度功能函数，需在主循环中调用
//void UartDriver()
//{  

////	unsigned char address=1;
//	
//	unsigned char i=0,cnt;
//	unsigned int crc;
//	unsigned char crch,crcl;
//	static u8 len;
//	static u8 buf[60];

//	u16 adcx_a;
//									
//	/*----------光合有效辐射计算------------*/	
//			


//	/*------------------modbus协议----------------*/
//	if(flagFrame)            //帧接收完成标志，即接收到一帧新数据
//	{
//		flagFrame=0;           //帧接收完成标志清零
//		len = UartRead(buf,sizeof(buf));   //将接收到的命令读到缓冲区中
//		if(buf[0]==address)                   //判断地址是不是0x04
//		{
//			crc=GetCRC16(buf,len-2);       //计算CRC校验值，出去CRC校验值
//			crch=crc>>8;    				//crc高位
//			crcl=crc&0xFF;					//crc低位
//			if((buf[len-2]==crch)&&(buf[len-1]==crcl))  //判断CRC校验是否正确
//			{
//				switch (buf[1])  //按功能码执行操作
//				{					
//					case 0x03:     //读数据
//						if((buf[2]==0x00)&&(buf[3]<=0x21))  //寄存器地址支持0x0000~0x0005
//						{	
///*------------------------- 寄存器地址为 0x02 ----------------------------*/					
///*光合有效*/	
//							if(buf[3]==0x20)
//							{	
//								i=buf[3];//提取寄存器地址
//								cnt=buf[5];  //提取待读取的寄存器数量
//								buf[2]=cnt*2;  //读取数据的字节数，为寄存器*2，因modbus定义的寄存器为16位
//								len=3;

//								if(buf[5]==0x02)   //判断数据个数
//									{	
//										u16 a=*(u32 *)PARGroup[4]>>16;
//										u16 b=*(u32 *)PARGroup[4]&0xffff;
//										u8 c=a>>8;
//										u8 d=a&0XFF;
//										u8 e=b>>8;
//										u8 f=b&0XFF;
//					
//										PARGroup[2]=c;
//										PARGroup[3]=d;
//										PARGroup[1]=e;
//										PARGroup[0]=f;//Modbus 通讯协议中的数组形式，高低排列为f d c d
//										
//										i=0;

//										while(cnt--)   //cnt=	寄存器数量，cnt-- 即cnt=cnt-1; 
//										{	
//											buf[len++]=PARGroup [i++];				//寄存器高字节补0
//											buf[len++]=PARGroup [i++];
//										}
//									}
//							}

//							break;
//						}
//							else  //寄存器地址不被支持时，返回错误码
//							{   
//								buf[1]=0x83;  //功能码最高位置1
//								buf[2]=0x02;  //设置异常码为02-无效地址
//								len=3;
//								break;
//							}
//				/*写寄存器在末尾，后加*/
//					case 0x06:           //写入单个寄存器
//						if((buf[2]==0x00)&&(buf[3]<=0x66))   //寄存器地址支持0x0000-0x0005
//						{
//							if(buf[3]==0x32)
//							{
//								i=buf[3];				//提取寄存器地址
//								PARGroup[i]=buf[5];		//保存寄存器数据
//								
//								address = PARGroup[i];			///////如何将地址保存进去是个问题******************************（已成功，现寻找破解之道）
//							}
//							len -=2;                 //长度-2以重新计算CRC并返回原帧
//							break;
//						}
//							////////////////////////////////////////////////////////////////////////////////////////////
////						if((buf[2]==0x00)&&(buf[3]==0x66))   //寄存器地址支持0x0000-0x0005
////						{
////							if(buf[3]==0x66)
////							{
////								i=buf[3];							//提取寄存器地址 
////						//		address = buf[3];
////								//cnt=buf[5];  				//提取待读取的寄存器数量
////								
////								address = buf[5];							
////								
////								buf[0]  = address;  			
////				
////							}
////							len -=2;                 //长度-2以重新计算CRC并返回原帧
////							break;
////						}
//	
//						

//							
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////						
//						else  
//						{							//寄存器地址不被支持，返回错误码
//							buf[1]=0x86;           //功能码最高位置1
//							buf[2]=0x02;           //设置异常码为02-无效地址
//							len=3;
//							break;
//						}
//					default:    //其他不支持的功能码
//						  buf[1]=0x80;     //功能码最高位置1
//							buf[2]=0x01;     //设置异常码为01—无效功能
//							len=3;
//							break;
//				}
//			  crc=GetCRC16(buf,len);        //计算CRC校验值
//				buf[len++]=crc>>8;           //CRC高字节
//				buf[len++]=crc&0xff;        //CRC低字节
//				rs485_UartWrite(buf,len);  //发送响应帧
//			}
//		}
//					

//			
///*----------------------------------------------------*/
//	}
//	
//	
//}


//void UartRxMonitor(u8 ms) //串口接收监控
//{
//	static u8 USART2_RX_BKP=0;  //定义USART2_RC_BKP暂时存储诗句长度与实际长度比较
//	static u8 idletmr=0;        //定义监控时间
//	if(USART2_RX_CNT>0)//接收计数器大于零时，监控总线空闲时间
//	{
//		if(USART2_RX_BKP!=USART2_RX_CNT) //接收计数器改变，即刚接收到数据时，清零空闲计时
//		{
//			USART2_RX_BKP=USART2_RX_CNT;  //赋值操作，将实际长度给USART2_RX_BKP
//			idletmr=0;                    //将监控时间清零
//		}
//		else                              ////接收计数器未改变，即总线空闲时，累计空闲时间
//		{
//			//如果在一帧数据完成之前有超过3.5个字节时间的停顿，接收设备将刷新当前的消息并假定下一个字节是一个新的数据帧的开始
//			if(idletmr<5)                  //空闲时间小于1ms时，持续累加
//			{
//				idletmr +=ms;
//				if(idletmr>=5)             //空闲时间达到1ms时，即判定为1帧接收完毕
//				{
//					flagFrame=1;//设置命令到达标志，帧接收完毕标志
//				}
//			}
//		}
//	}
//	else
//	{
//		USART2_RX_BKP=0;
//	}
//}							
//						
///*******************************************************************************
//* 函 数 名         : USART2_IRQHandler
//* 函数功能		   : USART2中断函数
//* 输    入         : 无
//* 输    出         : 无
//*******************************************************************************/ 
//void USART2_IRQHandler(void)
//{
//	u8 res;	                                    //定义数据缓存变量
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//接收到数据
//	{	 	
//		res =USART_ReceiveData(USART2);//;读取接收到的数据USART2->DR
//		if(USART2_RX_CNT<sizeof(USART2_RX_BUF))    //一次只能接收64个字节，人为设定，可以更大，但浪费时间
//		{
//			USART2_RX_BUF[USART2_RX_CNT]=res;  //记录接收到的值
//			USART2_RX_CNT++;        //使收数据增加1 
//		}
//	}
//}
//			 
///*******************************************************************************
//* 函 数 名         : TIM2_IRQHandler
//* 函数功能		   : TIM2中断函数
//* 输    入         : 无
//* 输    出         : 无
//*******************************************************************************/
//void TIM6_IRQHandler(void)
//{

//		UartRxMonitor(1); ////串口接收监控
//	//delay_ms(100);
//	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);	//更新中断
//}
//	





			 




