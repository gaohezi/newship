
#include "sys.h"
#include "usart3.h"
#include "usart.h"
#include "ml302.h"
 
/*  ML302通讯串口  */

void UART1_send_byte(char data)
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	USART_SendData(USART1, data);
}
void UART3_send_byte(char data)
{
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	USART_SendData(USART3, data);
}

//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
char RxBuffer[900];     //接收缓冲,最大USART_REC_LEN个字节.
u16 RxCounter;						//
char RxCounter1,RxBuffer1[100];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目 
extern u8 Timeout;

u8 USART3_RX_BUF[USART3_REC_LEN]; //数组用来存储接收到的数据，而USART3_REC_LEN为最多能接收的字节限度
u16 USART3_RX_STA=0;         		//接收状态标记	 0-14位为接收字节数，15位接收完成标志位

void usart3_init(u32 bound)
{ 
	  GPIO_InitTypeDef GPIO_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;
	  USART_InitTypeDef USART_InitStructure;
	 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能串口5
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //使能GPIOC.D
	  USART_DeInit(USART3);//串口5复位 不是必要的一步
	
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	  GPIO_Init(GPIOB,&GPIO_InitStructure);    //TX PC12设置为复用推挽输出

	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	  GPIO_Init(GPIOB,&GPIO_InitStructure);  //RX PB11设置为浮空输入	 
  
	  NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;
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
    USART_Init(USART3, &USART_InitStructure); //初始化串口
		
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART3, ENABLE);                    //使能串口 
		  
}
//void USART3_IRQHandler()//串口5的中断
//{ 
//	 u8 res;//暂时缓存接收的数据
//   if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)//判断是否为串口5接收中断
//	 {		  
//	     res=USART_ReceiveData(USART3);//接收到数据放进res
//		  USART3_RX_BUF[USART3_RX_STA&0x7FFF]=res;//数据放进数组中，则可以用到main函数中了
//		   USART3_RX_STA++;                         //字节长度++
//		   if(USART3_RX_STA>(USART3_REC_LEN-1))USART3_RX_STA=0;//接收数据错误,重新开始接收	+; 
//		   USART3_RX_STA|=0x8000;//串口5接收完成
//		   USART_ClearITPendingBit(USART3,USART_IT_RXNE);//清除接收中断标志

//				RxBuffer[RxCounter++] =USART_ReceiveData(USART3);//接收模块的数据
//       if(RxCounter >9999)//长度自行设定
//       RxCounter=0; 			
//  }   

//}

void USART3_IRQHandler()//串口5的中断
{ 
	 u8 res;//暂时缓存接收的数据

   if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)//判断是否为串口5接收中断
	 {		  
	     res = USART_ReceiveData(USART3);//接收到数据放进res
		  USART3_RX_BUF[USART3_RX_STA&0x7FFF]=res;//数据放进数组中，则可以用到main函数中了
		   USART3_RX_STA++;                         //字节长度++
		   if(USART3_RX_STA>(USART3_REC_LEN-1))USART3_RX_STA=0;//接收数据错误,重新开始接收	+; 
		   USART3_RX_STA|=0x8000;//串口5接收完成
		   USART_ClearITPendingBit(USART3,USART_IT_RXNE);//清除接收中断标志

				RxBuffer[RxCounter++] = USART_ReceiveData(USART3);//接收模块的数据
//       if(RxCounter >9999)//长度自行设定
//       RxCounter=0; 


  }   
}


void Uart1_SendStr(char*SendBuf)//串口1打印数据
{
	while(*SendBuf)
	{
        while((USART1->SR&0X40)==0);//等待发送完成 
        USART1->DR = (u8) *SendBuf; 
        SendBuf++;
	}
//	
//	memset(SendBuf,0,sizeof*(SendBuf)	);
}
void USART3_SendStr(char*SendBuf)//串口1打印数据
{
	while(*SendBuf)
	{
        while((USART3->SR&0X40)==0);//等待发送完成 
        USART3->DR = (u8) *SendBuf; 
        SendBuf++;
	}
}








