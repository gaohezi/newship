//#include "sys.h"
//#include "uart5.h"
//#include "usart.h"
//#include "string.h"
//#include <stdio.h>
///*
//用来接收jy901传感器的经纬度
//*/

//void UART5_send_byte(char data)
//{
//	while(USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET);
//	USART_SendData(UART5, data);
//}

////串口1中断服务程序
////注意,读取USARTx->SR能避免莫名其妙的错误   	
//char u5_RxCounter,u5_RxBuffer[600];     //接收缓冲,最大USART_REC_LEN个字节.

////接收状态
////bit15，	接收完成标志
////bit14，	接收到0x0d
////bit13~0，	接收到的有效字节数目 
//extern u8 Timeout;

/////////
//u8 UART5_RX_BUF[UART5_REC_LEN]; //数组用来存储接收到的数据，而UART5_REC_LEN为最多能接收的字节限度
//u16 UART5_RX_STA=0;         		//接收状态标记	 0-14位为接收字节数，15位接收完成标志位

//void uart5_init(u32 bound)
//{ 
//	
//	  GPIO_InitTypeDef GPIO_InitStructure;//没问题
//	  NVIC_InitTypeDef NVIC_InitStructure;//没问题
//	  USART_InitTypeDef USART_InitStructure;
//	 
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC,ENABLE);//使能串口5
////    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE); //使能GPIOC.D
//	  USART_DeInit(UART5);//串口5复位 不是必要的一步
//	
//	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
//	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
//	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	  GPIO_Init(GPIOC,&GPIO_InitStructure);    //TX PC12设置为复用推挽输出

//	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
//	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
//	  GPIO_Init(GPIOD,&GPIO_InitStructure);  //RX PD2设置为浮空输入	 
//  
//	  NVIC_InitStructure.NVIC_IRQChannel=UART5_IRQn;
//	  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
//	  NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
//	  NVIC_Init(&NVIC_InitStructure);          //串口5中断的设置
//		 
//   //USART 初始化设置
//	USART_InitStructure.USART_BaudRate = bound;//115200
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
//	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
//    USART_Init(UART5, &USART_InitStructure); //初始化串口
//		
//    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启中断
//    USART_Cmd(UART5, ENABLE);                    //使能串口 
//		  
//}
////void UART5_IRQHandler()//串口5的中断
////{ 
////	 u8 res;//暂时缓存接收的数据

////   if(USART_GetITStatus(UART5,USART_IT_RXNE)!=RESET)//判断是否为串口5接收中断
////	 {
////		   
////	     res=USART_ReceiveData(UART5);//接收到数据放进res
////		  UART5_RX_BUF[UART5_RX_STA&0x7FFF]=res;//数据放进数组中，则可以用到main函数中了
////		   UART5_RX_STA++;                         //字节长度++
////		   if(UART5_RX_STA>(UART5_REC_LEN-1))UART5_RX_STA=0;//接收数据错误,重新开始接收	+; 
////		   UART5_RX_STA|=0x8000;//串口5接收完成
////		   USART_ClearITPendingBit(UART5,USART_IT_RXNE);//清除接收中断标志

////				u5_RxBuffer[u5_RxCounter++] =USART_ReceiveData(UART5);//接收模块的数据
//////       if(u5_RxCounter >=99)//长度自行设定
//////      u5_RxCounter=0; 			 
////  }    
////}



////static unsigned char TxBuffer[256];
////static unsigned char TxCounter=0;
////static unsigned char count=0; 
//extern void CopeSerial2Data(unsigned char ucData);

////void UART5_IRQHandler(void)//例程
////{
////  if(USART_GetITStatus(UART5, USART_IT_TXE) != RESET)
////  {   
////    USART_SendData(UART5, TxBuffer[TxCounter++]); 
////    USART_ClearITPendingBit(UART5, USART_IT_TXE);
////    if(TxCounter == count) 
////		USART_ITConfig(UART5, USART_IT_TXE, DISABLE);
////  }
////	else if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
////  {
////		CopeSerial2Data((unsigned char)USART1->DR);//处理数据
////		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
////  }
////	
////	USART_ClearITPendingBit(USART1,USART_IT_ORE);
////}


//void UART5_IRQHandler(void)//自写中断服务函数
//{
////版一=========================
////u8 Res;
////	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
////  {
//////		CopeSerial2Data((unsigned char)UART5->DR);//处理数据

////static	 unsigned char ucRxBuffer[250];
////static	 unsigned char ucRxCnt = 0;	
////	Res =USART_ReceiveData(UART5);	//读取接收到的数据
////	
////	ucRxBuffer[ucRxCnt++]=Res;	//将收到的数据存入缓冲区中
////	if (ucRxBuffer[0]!=0x55) //数据头不对，则重新开始寻找0x55数据头
////	{
////		ucRxCnt=0;
////		return;
////	}
////	if (ucRxCnt<11) {return;}//数据不满11个，则返回

////u1_printf("%s",&ucRxBuffer);
//////u1_printf("%d",Res);
////		//USART_ClearITPendingBit(UART5, USART_IT_RXNE);
////  }
////版二========================
////char Res555;
////	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //接收中断，可以扩展来控制
////    {
////        Res555=USART_ReceiveData(UART5);//接收模块的数据;
////      u1_printf("%s",Res555);
////			USART_ClearITPendingBit(UART5, USART_IT_RXNE);
////    } 
////版三（例程）================
//	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //接收中断，可以扩展来控制
//    {
//			CopeSerial2Data((unsigned char)UART5->DR);//处理数据
////			USART_ClearITPendingBit(UART5, USART_IT_RXNE);
//    } 

//}



//void Uart5_SendStr(char*SendBuf)//串口1打印数据
//{
//	while(*SendBuf)
//	{
//        while((UART5->SR&0X40)==0);//等待发送完成 
//        UART5->DR = (u8) *SendBuf; 
//        SendBuf++;
//	}
//}


#include "sys.h"
#include "uart5.h"
#include "usart.h"
#include "string.h"
#include <stdio.h>
/*
用来接收定位传感器的经纬度
*/
u8 	UART5_RX_BUF[UART5_MAX_RECV_LEN]; 				//接收缓冲,最大USART3_MAX_RECV_LEN个字节.
u8  UART5_TX_BUF[UART5_MAX_SEND_LEN]; 			//发送缓冲,最大USART3_MAX_SEND_LEN字节

//char UART5_RxBuffer[900];     //接收缓冲,最大USART_REC_LEN个字节.
//u32 UART5_RxCounter;	
///u8 UART5_RX_BUF[UART5_REC_LEN]; //数组用来存储接收到的数据，而USART3_REC_LEN为最多能接收的字节限度
        		//接收状态标记	 0-14位为接收字节数，15位接收完成标志位
//
void UART5_send_byte(char data)
{
	while(USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET);
	USART_SendData(UART5, data);
}

//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	

u16 UART5_RX_STA=0;         		//接收状态标记	 0-14位为接收字节数，15位接收完成标志位

void UART5_IRQHandler(void)
{
	u8 res;	      
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)//接收到数据
	{	 
		res =USART_ReceiveData(UART5);		 
		if((UART5_RX_STA&(1<<15))==0)//接收完的一批数据,还没有被处理,则不再接收其他数据
		{ 
			if(UART5_RX_STA<UART5_MAX_RECV_LEN)	//还可以接收数据
			{
				//TIM_SetCounter(TIM7,0);//计数器清空          				//计数器清空
				if(UART5_RX_STA==0) 				//使能定时器7的中断 
				{
					//TIM_Cmd(TIM7,ENABLE);//使能定时器7
				}
				UART5_RX_BUF[UART5_RX_STA++]=res;	//记录接收到的值	 
			}else 
			{
				UART5_RX_STA|=1<<15;				//强制标记接收完成
			} 
		}
	}  				 											 
}   

void uart5_init(u32 bound)
{ 
	
	  GPIO_InitTypeDef GPIO_InitStructure;//没问题
	  NVIC_InitTypeDef NVIC_InitStructure;//没问题
	  USART_InitTypeDef USART_InitStructure;
	 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC,ENABLE);//使能串口5
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE); //使能GPIOC.D
	  USART_DeInit(UART5);//串口5复位 不是必要的一步
	
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	  GPIO_Init(GPIOC,&GPIO_InitStructure);    //TX PC12设置为复用推挽输出

	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	  GPIO_Init(GPIOD,&GPIO_InitStructure);  //RX PD2设置为浮空输入	 
  
	  NVIC_InitStructure.NVIC_IRQChannel=UART5_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	  NVIC_Init(&NVIC_InitStructure);          //串口5中断的设置
		 
   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(UART5, &USART_InitStructure); //初始化串口
		
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(UART5, ENABLE);                    //使能串口 
		  
	//	TIM7_Int_Init(1000-1,7200-1);		//10ms中断
	UART5_RX_STA=0;		//清零
	TIM_Cmd(TIM7,DISABLE);			//关闭定时器7

}


//void UART5_IRQHandler()//串口5的中断
//{ 
//	 u8 res;//暂时缓存接收的数据

//   if(USART_GetITStatus(UART5,USART_IT_RXNE)!=RESET)//判断是否为串口5接收中断
//	 {		  
//	     res=USART_ReceiveData(UART5);//接收到数据放进res
//		  UART5_RX_BUF[UART5_RX_STA&0x7FFF]=res;//数据放进数组中，则可以用到main函数中了
//		   UART5_RX_STA++;                         //字节长度++
//		   if(UART5_RX_STA>(UART5_REC_LEN-1))UART5_RX_STA=0;//接收数据错误,重新开始接收	+; 
//		   UART5_RX_STA|=0x8000;//串口5接收完成
//		   USART_ClearITPendingBit(UART5,USART_IT_RXNE);//清除接收中断标志

//				UART5_RxBuffer[UART5_RxCounter++] = USART_ReceiveData(UART5);//接收模块的数据
////       if(RxCounter >9999)//长度自行设定
////       RxCounter=0; 			
//  }   

//}


//extern void CopeSerial2Data(unsigned char ucData);





void Uart5_SendStr(char*SendBuf)//串口1打印数据
{
	while(*SendBuf)
	{
        while((UART5->SR&0X40)==0);//等待发送完成 
        UART5->DR = (u8) *SendBuf; 
        SendBuf++;
	}
}

