//#include "sys.h"
//#include "uart5.h"
//#include "usart.h"
//#include "string.h"
//#include <stdio.h>
///*
//��������jy901�������ľ�γ��
//*/

//void UART5_send_byte(char data)
//{
//	while(USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET);
//	USART_SendData(UART5, data);
//}

////����1�жϷ������
////ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
//char u5_RxCounter,u5_RxBuffer[600];     //���ջ���,���USART_REC_LEN���ֽ�.

////����״̬
////bit15��	������ɱ�־
////bit14��	���յ�0x0d
////bit13~0��	���յ�����Ч�ֽ���Ŀ 
//extern u8 Timeout;

/////////
//u8 UART5_RX_BUF[UART5_REC_LEN]; //���������洢���յ������ݣ���UART5_REC_LENΪ����ܽ��յ��ֽ��޶�
//u16 UART5_RX_STA=0;         		//����״̬���	 0-14λΪ�����ֽ�����15λ������ɱ�־λ

//void uart5_init(u32 bound)
//{ 
//	
//	  GPIO_InitTypeDef GPIO_InitStructure;//û����
//	  NVIC_InitTypeDef NVIC_InitStructure;//û����
//	  USART_InitTypeDef USART_InitStructure;
//	 
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC,ENABLE);//ʹ�ܴ���5
////    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE); //ʹ��GPIOC.D
//	  USART_DeInit(UART5);//����5��λ ���Ǳ�Ҫ��һ��
//	
//	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
//	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
//	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	  GPIO_Init(GPIOC,&GPIO_InitStructure);    //TX PC12����Ϊ�����������

//	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
//	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
//	  GPIO_Init(GPIOD,&GPIO_InitStructure);  //RX PD2����Ϊ��������	 
//  
//	  NVIC_InitStructure.NVIC_IRQChannel=UART5_IRQn;
//	  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
//	  NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
//	  NVIC_Init(&NVIC_InitStructure);          //����5�жϵ�����
//		 
//   //USART ��ʼ������
//	USART_InitStructure.USART_BaudRate = bound;//115200
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
//	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
//    USART_Init(UART5, &USART_InitStructure); //��ʼ������
//		
//    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//�����ж�
//    USART_Cmd(UART5, ENABLE);                    //ʹ�ܴ��� 
//		  
//}
////void UART5_IRQHandler()//����5���ж�
////{ 
////	 u8 res;//��ʱ������յ�����

////   if(USART_GetITStatus(UART5,USART_IT_RXNE)!=RESET)//�ж��Ƿ�Ϊ����5�����ж�
////	 {
////		   
////	     res=USART_ReceiveData(UART5);//���յ����ݷŽ�res
////		  UART5_RX_BUF[UART5_RX_STA&0x7FFF]=res;//���ݷŽ������У�������õ�main��������
////		   UART5_RX_STA++;                         //�ֽڳ���++
////		   if(UART5_RX_STA>(UART5_REC_LEN-1))UART5_RX_STA=0;//�������ݴ���,���¿�ʼ����	+; 
////		   UART5_RX_STA|=0x8000;//����5�������
////		   USART_ClearITPendingBit(UART5,USART_IT_RXNE);//��������жϱ�־

////				u5_RxBuffer[u5_RxCounter++] =USART_ReceiveData(UART5);//����ģ�������
//////       if(u5_RxCounter >=99)//���������趨
//////      u5_RxCounter=0; 			 
////  }    
////}



////static unsigned char TxBuffer[256];
////static unsigned char TxCounter=0;
////static unsigned char count=0; 
//extern void CopeSerial2Data(unsigned char ucData);

////void UART5_IRQHandler(void)//����
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
////		CopeSerial2Data((unsigned char)USART1->DR);//��������
////		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
////  }
////	
////	USART_ClearITPendingBit(USART1,USART_IT_ORE);
////}


//void UART5_IRQHandler(void)//��д�жϷ�����
//{
////��һ=========================
////u8 Res;
////	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
////  {
//////		CopeSerial2Data((unsigned char)UART5->DR);//��������

////static	 unsigned char ucRxBuffer[250];
////static	 unsigned char ucRxCnt = 0;	
////	Res =USART_ReceiveData(UART5);	//��ȡ���յ�������
////	
////	ucRxBuffer[ucRxCnt++]=Res;	//���յ������ݴ��뻺������
////	if (ucRxBuffer[0]!=0x55) //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
////	{
////		ucRxCnt=0;
////		return;
////	}
////	if (ucRxCnt<11) {return;}//���ݲ���11�����򷵻�

////u1_printf("%s",&ucRxBuffer);
//////u1_printf("%d",Res);
////		//USART_ClearITPendingBit(UART5, USART_IT_RXNE);
////  }
////���========================
////char Res555;
////	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //�����жϣ�������չ������
////    {
////        Res555=USART_ReceiveData(UART5);//����ģ�������;
////      u1_printf("%s",Res555);
////			USART_ClearITPendingBit(UART5, USART_IT_RXNE);
////    } 
////���������̣�================
//	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //�����жϣ�������չ������
//    {
//			CopeSerial2Data((unsigned char)UART5->DR);//��������
////			USART_ClearITPendingBit(UART5, USART_IT_RXNE);
//    } 

//}



//void Uart5_SendStr(char*SendBuf)//����1��ӡ����
//{
//	while(*SendBuf)
//	{
//        while((UART5->SR&0X40)==0);//�ȴ�������� 
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
�������ն�λ�������ľ�γ��
*/
u8 	UART5_RX_BUF[UART5_MAX_RECV_LEN]; 				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.
u8  UART5_TX_BUF[UART5_MAX_SEND_LEN]; 			//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�

//char UART5_RxBuffer[900];     //���ջ���,���USART_REC_LEN���ֽ�.
//u32 UART5_RxCounter;	
///u8 UART5_RX_BUF[UART5_REC_LEN]; //���������洢���յ������ݣ���USART3_REC_LENΪ����ܽ��յ��ֽ��޶�
        		//����״̬���	 0-14λΪ�����ֽ�����15λ������ɱ�־λ
//
void UART5_send_byte(char data)
{
	while(USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET);
	USART_SendData(UART5, data);
}

//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	

u16 UART5_RX_STA=0;         		//����״̬���	 0-14λΪ�����ֽ�����15λ������ɱ�־λ

void UART5_IRQHandler(void)
{
	u8 res;	      
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)//���յ�����
	{	 
		res =USART_ReceiveData(UART5);		 
		if((UART5_RX_STA&(1<<15))==0)//�������һ������,��û�б�����,���ٽ�����������
		{ 
			if(UART5_RX_STA<UART5_MAX_RECV_LEN)	//�����Խ�������
			{
				//TIM_SetCounter(TIM7,0);//���������          				//���������
				if(UART5_RX_STA==0) 				//ʹ�ܶ�ʱ��7���ж� 
				{
					//TIM_Cmd(TIM7,ENABLE);//ʹ�ܶ�ʱ��7
				}
				UART5_RX_BUF[UART5_RX_STA++]=res;	//��¼���յ���ֵ	 
			}else 
			{
				UART5_RX_STA|=1<<15;				//ǿ�Ʊ�ǽ������
			} 
		}
	}  				 											 
}   

void uart5_init(u32 bound)
{ 
	
	  GPIO_InitTypeDef GPIO_InitStructure;//û����
	  NVIC_InitTypeDef NVIC_InitStructure;//û����
	  USART_InitTypeDef USART_InitStructure;
	 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC,ENABLE);//ʹ�ܴ���5
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE); //ʹ��GPIOC.D
	  USART_DeInit(UART5);//����5��λ ���Ǳ�Ҫ��һ��
	
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	  GPIO_Init(GPIOC,&GPIO_InitStructure);    //TX PC12����Ϊ�����������

	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	  GPIO_Init(GPIOD,&GPIO_InitStructure);  //RX PD2����Ϊ��������	 
  
	  NVIC_InitStructure.NVIC_IRQChannel=UART5_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	  NVIC_Init(&NVIC_InitStructure);          //����5�жϵ�����
		 
   //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    USART_Init(UART5, &USART_InitStructure); //��ʼ������
		
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(UART5, ENABLE);                    //ʹ�ܴ��� 
		  
	//	TIM7_Int_Init(1000-1,7200-1);		//10ms�ж�
	UART5_RX_STA=0;		//����
	TIM_Cmd(TIM7,DISABLE);			//�رն�ʱ��7

}


//void UART5_IRQHandler()//����5���ж�
//{ 
//	 u8 res;//��ʱ������յ�����

//   if(USART_GetITStatus(UART5,USART_IT_RXNE)!=RESET)//�ж��Ƿ�Ϊ����5�����ж�
//	 {		  
//	     res=USART_ReceiveData(UART5);//���յ����ݷŽ�res
//		  UART5_RX_BUF[UART5_RX_STA&0x7FFF]=res;//���ݷŽ������У�������õ�main��������
//		   UART5_RX_STA++;                         //�ֽڳ���++
//		   if(UART5_RX_STA>(UART5_REC_LEN-1))UART5_RX_STA=0;//�������ݴ���,���¿�ʼ����	+; 
//		   UART5_RX_STA|=0x8000;//����5�������
//		   USART_ClearITPendingBit(UART5,USART_IT_RXNE);//��������жϱ�־

//				UART5_RxBuffer[UART5_RxCounter++] = USART_ReceiveData(UART5);//����ģ�������
////       if(RxCounter >9999)//���������趨
////       RxCounter=0; 			
//  }   

//}


//extern void CopeSerial2Data(unsigned char ucData);





void Uart5_SendStr(char*SendBuf)//����1��ӡ����
{
	while(*SendBuf)
	{
        while((UART5->SR&0X40)==0);//�ȴ�������� 
        UART5->DR = (u8) *SendBuf; 
        SendBuf++;
	}
}
