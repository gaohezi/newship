
#include "sys.h"
#include "usart3.h"
#include "usart.h"
#include "ml302.h"
 
/*  ML302ͨѶ����  */

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

//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
char RxBuffer[900];     //���ջ���,���USART_REC_LEN���ֽ�.
u16 RxCounter;						//
char RxCounter1,RxBuffer1[100];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ 
extern u8 Timeout;

u8 USART3_RX_BUF[USART3_REC_LEN]; //���������洢���յ������ݣ���USART3_REC_LENΪ����ܽ��յ��ֽ��޶�
u16 USART3_RX_STA=0;         		//����״̬���	 0-14λΪ�����ֽ�����15λ������ɱ�־λ

void usart3_init(u32 bound)
{ 
	  GPIO_InitTypeDef GPIO_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;
	  USART_InitTypeDef USART_InitStructure;
	 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ�ܴ���5
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //ʹ��GPIOC.D
	  USART_DeInit(USART3);//����5��λ ���Ǳ�Ҫ��һ��
	
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	  GPIO_Init(GPIOB,&GPIO_InitStructure);    //TX PC12����Ϊ�����������

	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	  GPIO_Init(GPIOB,&GPIO_InitStructure);  //RX PB11����Ϊ��������	 
  
	  NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;
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
    USART_Init(USART3, &USART_InitStructure); //��ʼ������
		
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 
		  
}
//void USART3_IRQHandler()//����5���ж�
//{ 
//	 u8 res;//��ʱ������յ�����
//   if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)//�ж��Ƿ�Ϊ����5�����ж�
//	 {		  
//	     res=USART_ReceiveData(USART3);//���յ����ݷŽ�res
//		  USART3_RX_BUF[USART3_RX_STA&0x7FFF]=res;//���ݷŽ������У�������õ�main��������
//		   USART3_RX_STA++;                         //�ֽڳ���++
//		   if(USART3_RX_STA>(USART3_REC_LEN-1))USART3_RX_STA=0;//�������ݴ���,���¿�ʼ����	+; 
//		   USART3_RX_STA|=0x8000;//����5�������
//		   USART_ClearITPendingBit(USART3,USART_IT_RXNE);//��������жϱ�־

//				RxBuffer[RxCounter++] =USART_ReceiveData(USART3);//����ģ�������
//       if(RxCounter >9999)//���������趨
//       RxCounter=0; 			
//  }   

//}

void USART3_IRQHandler()//����5���ж�
{ 
	 u8 res;//��ʱ������յ�����

   if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)//�ж��Ƿ�Ϊ����5�����ж�
	 {		  
	     res = USART_ReceiveData(USART3);//���յ����ݷŽ�res
		  USART3_RX_BUF[USART3_RX_STA&0x7FFF]=res;//���ݷŽ������У�������õ�main��������
		   USART3_RX_STA++;                         //�ֽڳ���++
		   if(USART3_RX_STA>(USART3_REC_LEN-1))USART3_RX_STA=0;//�������ݴ���,���¿�ʼ����	+; 
		   USART3_RX_STA|=0x8000;//����5�������
		   USART_ClearITPendingBit(USART3,USART_IT_RXNE);//��������жϱ�־

				RxBuffer[RxCounter++] = USART_ReceiveData(USART3);//����ģ�������
//       if(RxCounter >9999)//���������趨
//       RxCounter=0; 


  }   
}


void Uart1_SendStr(char*SendBuf)//����1��ӡ����
{
	while(*SendBuf)
	{
        while((USART1->SR&0X40)==0);//�ȴ�������� 
        USART1->DR = (u8) *SendBuf; 
        SendBuf++;
	}
//	
//	memset(SendBuf,0,sizeof*(SendBuf)	);
}
void USART3_SendStr(char*SendBuf)//����1��ӡ����
{
	while(*SendBuf)
	{
        while((USART3->SR&0X40)==0);//�ȴ�������� 
        USART3->DR = (u8) *SendBuf; 
        SendBuf++;
	}
}







