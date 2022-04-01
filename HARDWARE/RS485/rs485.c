#include "sys.h"		    
#include "rs485.h"	 
#include "delay.h"
#include <stdio.h>
#include "usart.h"	

 u32 WDIR_data;		//��������
#ifdef EN_USART2_RX   	//���ʹ���˽���


//���ջ����� 	
 u8 RS485_RX_BUF[164];  	//���ջ���,���64���ֽ�.
//���յ������ݳ���
u8 RS485_RX_CNT=0;   		  
  
void USART2_IRQHandler(void)
{
	u8 res;	    
 
 	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //���յ�����
	{	 
	 			 
		res =USART_ReceiveData(USART2); 	//��ȡ���յ�������
//		if(RS485_RX_CNT<64)
//		{
			RS485_RX_BUF[RS485_RX_CNT]=res;		//��¼���յ���ֵ
			RS485_RX_CNT++;						//������������1 
//		}		
		//USART_SendData(USART1, RS485_RX_BUF[RS485_RX_CNT]);//�򴮿�1��������
	}  	
} 
#endif										 
//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������	  
void RS485_Init(u32 bound)
{  
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);//ʹ��GPIOA,Dʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //PD7�˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	//PA2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);  

	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,ENABLE);//��λ����2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,DISABLE);//ֹͣ��λ
 
	
 #ifdef EN_USART2_RX		  	//���ʹ���˽���
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ���ݳ���
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;///��żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ

  USART_Init(USART2, &USART_InitStructure); ; //��ʼ������
  
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; //ʹ�ܴ���2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
 
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
   
  USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 

 #endif

  RS485_TX_EN=0;			//Ĭ��Ϊ����ģʽ
 
}

//RS485����len���ֽ�.
//buf:�������׵�ַ
//len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
void RS485_Send_Data(u8 *buf,u8 len)
{
	u8 t;
	RS485_TX_EN=1;			//����Ϊ����ģʽ
  	for(t=0;t<len;t++)		//ѭ����������
	{		   
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);	  
		USART_SendData(USART2,buf[t]);
	
	}	 
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);		
	RS485_RX_CNT=0;	  
	RS485_TX_EN=0;				//����Ϊ����ģʽ	
}
//RS485��ѯ���յ�������
//buf:���ջ����׵�ַ
//len:���������ݳ���
void RS485_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen=RS485_RX_CNT;
	u8 i=0;
	*len=0;				//Ĭ��Ϊ0
	delay_ms(10);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
	if(rxlen==RS485_RX_CNT&&rxlen)//���յ�������,�ҽ��������
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
		}	
		
		*len=RS485_RX_CNT;	//��¼�������ݳ���
		RS485_RX_CNT=0;		//����
	}
	
}

		u8 rs485buf[100]; 
	 u8 rev_len;
void rs485_sensor( void)
{
u8 data_len;
	u8 l_cmd[8]={0};
	l_cmd[0]=WDIR_ADD;		//l_sensorAddr����ʮ����33|16����0x21
	l_cmd[1]=0x03;
	l_cmd[2]=0x00;
	l_cmd[3]=0x2A;
	l_cmd[4]=0x00;
	l_cmd[5]=0x01;
//	l_cmd[6]=0xA2;
//	l_cmd[7]=0xA2;
	l_cmd[6]=Modbus_Crc(l_cmd,6)/256;				//����У��
	l_cmd[7]=Modbus_Crc(l_cmd,6)%256;				//����У��
	
	data_len=sizeof(l_cmd);
	
	RS485_Send_Data(l_cmd,data_len);
 
		
	RS485_Receive_Data(rs485buf,& rev_len);
	
	USART_RX_STA=0;
}

	void RS485_sensor_data(void)
	{

			//============================================		
				/*����2����Ӧ������򴮿�1�������� */
				RS485_Receive_Data(rs485buf,& rev_len);
				
//					for(i=0;i<rev_len;i++)
//					{
//						USART_SendData(USART1, rs485buf[i]);//�򴮿�1��������
//						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
//					}
		 //============================================	�������		 
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







































////���������������������֡�Ľ��գ����ȹ��ܺ�����������ѭ���е���
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
//	/*----------�����Ч�������------------*/	
//			


//	/*------------------modbusЭ��----------------*/
//	if(flagFrame)            //֡������ɱ�־�������յ�һ֡������
//	{
//		flagFrame=0;           //֡������ɱ�־����
//		len = UartRead(buf,sizeof(buf));   //�����յ������������������
//		if(buf[0]==address)                   //�жϵ�ַ�ǲ���0x04
//		{
//			crc=GetCRC16(buf,len-2);       //����CRCУ��ֵ����ȥCRCУ��ֵ
//			crch=crc>>8;    				//crc��λ
//			crcl=crc&0xFF;					//crc��λ
//			if((buf[len-2]==crch)&&(buf[len-1]==crcl))  //�ж�CRCУ���Ƿ���ȷ
//			{
//				switch (buf[1])  //��������ִ�в���
//				{					
//					case 0x03:     //������
//						if((buf[2]==0x00)&&(buf[3]<=0x21))  //�Ĵ�����ַ֧��0x0000~0x0005
//						{	
///*------------------------- �Ĵ�����ַΪ 0x02 ----------------------------*/					
///*�����Ч*/	
//							if(buf[3]==0x20)
//							{	
//								i=buf[3];//��ȡ�Ĵ�����ַ
//								cnt=buf[5];  //��ȡ����ȡ�ļĴ�������
//								buf[2]=cnt*2;  //��ȡ���ݵ��ֽ�����Ϊ�Ĵ���*2����modbus����ļĴ���Ϊ16λ
//								len=3;

//								if(buf[5]==0x02)   //�ж����ݸ���
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
//										PARGroup[0]=f;//Modbus ͨѶЭ���е�������ʽ���ߵ�����Ϊf d c d
//										
//										i=0;

//										while(cnt--)   //cnt=	�Ĵ���������cnt-- ��cnt=cnt-1; 
//										{	
//											buf[len++]=PARGroup [i++];				//�Ĵ������ֽڲ�0
//											buf[len++]=PARGroup [i++];
//										}
//									}
//							}

//							break;
//						}
//							else  //�Ĵ�����ַ����֧��ʱ�����ش�����
//							{   
//								buf[1]=0x83;  //���������λ��1
//								buf[2]=0x02;  //�����쳣��Ϊ02-��Ч��ַ
//								len=3;
//								break;
//							}
//				/*д�Ĵ�����ĩβ�����*/
//					case 0x06:           //д�뵥���Ĵ���
//						if((buf[2]==0x00)&&(buf[3]<=0x66))   //�Ĵ�����ַ֧��0x0000-0x0005
//						{
//							if(buf[3]==0x32)
//							{
//								i=buf[3];				//��ȡ�Ĵ�����ַ
//								PARGroup[i]=buf[5];		//����Ĵ�������
//								
//								address = PARGroup[i];			///////��ν���ַ�����ȥ�Ǹ�����******************************���ѳɹ�����Ѱ���ƽ�֮����
//							}
//							len -=2;                 //����-2�����¼���CRC������ԭ֡
//							break;
//						}
//							////////////////////////////////////////////////////////////////////////////////////////////
////						if((buf[2]==0x00)&&(buf[3]==0x66))   //�Ĵ�����ַ֧��0x0000-0x0005
////						{
////							if(buf[3]==0x66)
////							{
////								i=buf[3];							//��ȡ�Ĵ�����ַ 
////						//		address = buf[3];
////								//cnt=buf[5];  				//��ȡ����ȡ�ļĴ�������
////								
////								address = buf[5];							
////								
////								buf[0]  = address;  			
////				
////							}
////							len -=2;                 //����-2�����¼���CRC������ԭ֡
////							break;
////						}
//	
//						

//							
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////						
//						else  
//						{							//�Ĵ�����ַ����֧�֣����ش�����
//							buf[1]=0x86;           //���������λ��1
//							buf[2]=0x02;           //�����쳣��Ϊ02-��Ч��ַ
//							len=3;
//							break;
//						}
//					default:    //������֧�ֵĹ�����
//						  buf[1]=0x80;     //���������λ��1
//							buf[2]=0x01;     //�����쳣��Ϊ01����Ч����
//							len=3;
//							break;
//				}
//			  crc=GetCRC16(buf,len);        //����CRCУ��ֵ
//				buf[len++]=crc>>8;           //CRC���ֽ�
//				buf[len++]=crc&0xff;        //CRC���ֽ�
//				rs485_UartWrite(buf,len);  //������Ӧ֡
//			}
//		}
//					

//			
///*----------------------------------------------------*/
//	}
//	
//	
//}


//void UartRxMonitor(u8 ms) //���ڽ��ռ��
//{
//	static u8 USART2_RX_BKP=0;  //����USART2_RC_BKP��ʱ�洢ʫ�䳤����ʵ�ʳ��ȱȽ�
//	static u8 idletmr=0;        //������ʱ��
//	if(USART2_RX_CNT>0)//���ռ�����������ʱ��������߿���ʱ��
//	{
//		if(USART2_RX_BKP!=USART2_RX_CNT) //���ռ������ı䣬���ս��յ�����ʱ��������м�ʱ
//		{
//			USART2_RX_BKP=USART2_RX_CNT;  //��ֵ��������ʵ�ʳ��ȸ�USART2_RX_BKP
//			idletmr=0;                    //�����ʱ������
//		}
//		else                              ////���ռ�����δ�ı䣬�����߿���ʱ���ۼƿ���ʱ��
//		{
//			//�����һ֡�������֮ǰ�г���3.5���ֽ�ʱ���ͣ�٣������豸��ˢ�µ�ǰ����Ϣ���ٶ���һ���ֽ���һ���µ�����֡�Ŀ�ʼ
//			if(idletmr<5)                  //����ʱ��С��1msʱ�������ۼ�
//			{
//				idletmr +=ms;
//				if(idletmr>=5)             //����ʱ��ﵽ1msʱ�����ж�Ϊ1֡�������
//				{
//					flagFrame=1;//����������־��֡������ϱ�־
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
//* �� �� ��         : USART2_IRQHandler
//* ��������		   : USART2�жϺ���
//* ��    ��         : ��
//* ��    ��         : ��
//*******************************************************************************/ 
//void USART2_IRQHandler(void)
//{
//	u8 res;	                                    //�������ݻ������
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//���յ�����
//	{	 	
//		res =USART_ReceiveData(USART2);//;��ȡ���յ�������USART2->DR
//		if(USART2_RX_CNT<sizeof(USART2_RX_BUF))    //һ��ֻ�ܽ���64���ֽڣ���Ϊ�趨�����Ը��󣬵��˷�ʱ��
//		{
//			USART2_RX_BUF[USART2_RX_CNT]=res;  //��¼���յ���ֵ
//			USART2_RX_CNT++;        //ʹ����������1 
//		}
//	}
//}
//			 
///*******************************************************************************
//* �� �� ��         : TIM2_IRQHandler
//* ��������		   : TIM2�жϺ���
//* ��    ��         : ��
//* ��    ��         : ��
//*******************************************************************************/
//void TIM6_IRQHandler(void)
//{

//		UartRxMonitor(1); ////���ڽ��ռ��
//	//delay_ms(100);
//	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);	//�����ж�
//}
//	





			 



