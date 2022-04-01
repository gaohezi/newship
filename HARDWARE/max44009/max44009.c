/*


��������ֲ˵��:  ������IOģ��I2Cʵ�ִ������ɼ�����Ҫ��ֲ�ļ� �������ͺŶ�Ӧ��.h�ļ���.c�����ļ���
DataType.h(��ѡ) 
			.h�ļ�(�磺B_LUX_V20.h��)������IO�ŵĺ궨�� �� ��������, ��Ҫ�޸�IO�ĺ궨�壬��Ӧ�û�ʹ�õ�IO
			.c�ļ�(��: B_LUX_V20.c��)������I2Cģ�����ʹ������ɼ�����, ������Ҫ�޸ĵط�,������ʱ�������޹�
			DataType.h							 �����Ͷ��� (��ѡ)
*/
//***************************************
// B_LUX_V32D_V30�ɼ�����
//****************************************
#include "max44009.h"
//#include "delay.h"
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "SysTick.h"

//��ʼ��IIC
//void IIC_Init(void)
//{					     
//	GPIO_InitTypeDef GPIO_InitStructure;
//	//RCC->APB2ENR|=1<<4;//��ʹ������IO PORTCʱ�� 
//	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
//	   
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
// 
//	IIC_SCL=1;//PB10
//	IIC_SDA=1;//PB11

//}
 
/*---------------------------------------------------------------------
 ��������: ��ʼ�ź�
 ����˵��: ��	
 ��������: ��
 ---------------------------------------------------------------------*/
//void B_LUX_V32D_Start()
//{
//  B_LUX_V32D_SDA0_H;                         //����������
//  B_LUX_V32D_SCL0_H;                         //����ʱ����
//  B_LUX_V32D_Delay5us();                     //��ʱ
//  B_LUX_V32D_SDA0_L;                         //�����½���
//  B_LUX_V32D_Delay5us();                     //��ʱ
//  B_LUX_V32D_SCL0_L;                         //����ʱ����
//}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	//SDA_OUT();     //sda�����
//	IIC_SDA=1;	  	  
//	IIC_SCL=1;
//	delay_us(5);
// 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
//	delay_us(5);
//	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
	 B_LUX_V32D_SDA0_H;                         //����������
  B_LUX_V32D_SCL0_H;                         //����ʱ����
  delay_us(5);                      //��ʱ
  B_LUX_V32D_SDA0_L;                         //�����½���
  delay_us(5);                     //��ʱ
  B_LUX_V32D_SCL0_L;                         //����ʱ����
}	 

/*---------------------------------------------------------------------
 ��������: ֹͣ�ź�
 ����˵��: ��	
 ��������: ��
 ---------------------------------------------------------------------*/
//void B_LUX_V32D_Stop()
//{
//  B_LUX_V32D_SDA0_L;                         //����������
//  B_LUX_V32D_SCL0_H;                         //����ʱ����
//  B_LUX_V32D_Delay5us();                     //��ʱ
//  B_LUX_V32D_SDA0_H;                         //����������
//  B_LUX_V32D_Delay5us();                     //��ʱ
//  B_LUX_V32D_SCL0_L;
//  B_LUX_V32D_Delay5us();
//}
//����IICֹͣ�ź�
void IIC_Stop(void)
{
//	SDA_OUT();//sda�����
//	IIC_SCL=0;
//	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
// 	delay_us(4);
//	IIC_SCL=1; 
//	IIC_SDA=1;//����I2C���߽����ź�
//	delay_us(4);		
  B_LUX_V32D_SDA0_L;                         //����������
  B_LUX_V32D_SCL0_H;                         //����ʱ����
 delay_us(5);                      //��ʱ
  B_LUX_V32D_SDA0_H;                         //����������
  delay_us(5);                     //��ʱ
  B_LUX_V32D_SCL0_L;
  delay_us(5); 	
}
/*---------------------------------------------------------------------
 ��������: ����Ӧ���ź�
 ����˵��: ack - Ӧ���ź�(0:ACK 1:NAK)
 ��������: ��
 ---------------------------------------------------------------------*/
void B_LUX_V32D_SendACK(u8 ack)
{
//  if (ack&0x01)	
//			IIC_SDA=1;	//B_LUX_V32D_SDA0_H;		//дӦ���ź�
//  else	
//		  IIC_SDA=0;  //B_LUX_V32D_SDA0_L;
//  
// IIC_SCL=1;				// B_LUX_V32D_SCL0_H;                         //����ʱ����
//  delay_us(5);                    //��ʱ
// IIC_SCL=0;				// B_LUX_V32D_SCL0_L;                         //����ʱ����
//  delay_us(5);
// IIC_SDA=1;				// B_LUX_V32D_SDA0_H;
//  delay_us(5);                    //��ʱ
  if (ack&0x01)	B_LUX_V32D_SDA0_H;		//дӦ���ź�
  else	B_LUX_V32D_SDA0_L;
  
  B_LUX_V32D_SCL0_H;                         //����ʱ����
 delay_us(5);                     //��ʱ
  B_LUX_V32D_SCL0_L;                         //����ʱ����
 delay_us(5); 
  B_LUX_V32D_SDA0_H;
  delay_us(5);                     //��ʱ
}
  	  
/*---------------------------------------------------------------------
 ��������: ����Ӧ���ź�
 ����˵��: ��
 ��������: ����Ӧ���ź�
 ---------------------------------------------------------------------*/
u8 B_LUX_V32D_RecvACK()
{
//  u8 CY = 0x00;
//	u16 vConter = 1000;
//  IIC_SDA=1;		//B_LUX_V32D_SDA0_H;
//		
//  SDA_IN();			//B_LUX_V32D_SDA0_I;
//   IIC_SCL=1;		//B_LUX_V32D_SCL0_H;                         //����ʱ����
//  delay_us(5);                     //��ʱ
//  
//	while (vConter)
//	{
//		vConter--;
//		CY |= B_LUX_V32D_SDA0_DAT;                 //��Ӧ���ź�
//		
//		if(!CY)	break;
//	}
//  
//  delay_us(5);                     //��ʱ
//  
//  IIC_SCL=0;		//B_LUX_V32D_SCL0_L;                         //����ʱ����
//  SDA_OUT();		//B_LUX_V32D_SDA0_O;
//  
//  return CY;
  u8 CY = 0x00;
	u16 vConter = 1000;
  B_LUX_V32D_SDA0_H;
  
  B_LUX_V32D_SDA0_I;
  
  B_LUX_V32D_SCL0_H;                         //����ʱ����
 delay_us(5);                    //��ʱ
  
	while (vConter)
	{
		vConter--;
		CY |= B_LUX_V32D_SDA0_DAT;                 //��Ӧ���ź�
		
		if(!CY)	break;
	}
  
  delay_us(5);                    //��ʱ
  
  B_LUX_V32D_SCL0_L;                         //����ʱ����
  B_LUX_V32D_SDA0_O;
  
  return CY;
}

/*---------------------------------------------------------------------
 ��������: ��IIC���߷���һ���ֽ�����
 ����˵��: dat - д�ֽ�
 ��������: �ɹ�����0  ʧ�ܷ���1
 ---------------------------------------------------------------------*/
u8 B_LUX_V32D_SendByte(u8 dat)
{
//	u8 vRval = 0;
//  u8 i;
//  
//  for (i=0; i<8; i++)         			//8λ������
//  {
//    if (dat&0x80)	
//			IIC_SDA=1;//B_LUX_V32D_SDA0_H;
//    else	
//			IIC_SDA=0;//B_LUX_V32D_SDA0_L;                   //�����ݿ�/////////////////////////////////////////////////////////////?SDA��SCL����
//    
//    delay_us(5);              		//��ʱ
//    IIC_SCL=1;//B_LUX_V32D_SCL0_H;                		//����ʱ����
//    delay_us(5);             		//��ʱ
//    IIC_SCL=0;//B_LUX_V32D_SCL0_L;                		//����ʱ����
//   delay_us(5);              		//��ʱ
//    dat <<= 1;              			//�Ƴ����ݵ����λ
//  }
//  
//  vRval = B_LUX_V32D_RecvACK();
//	
//	return vRval;
u8 vRval = 0;
  u8 i;
  
  for (i=0; i<8; i++)         			//8λ������
  {
    if (dat&0x80)	B_LUX_V32D_SDA0_H;
    else	B_LUX_V32D_SDA0_L;                   //�����ݿ�
    
    delay_us(5);            		//��ʱ
    B_LUX_V32D_SCL0_H;                		//����ʱ����
   delay_us(5);            		//��ʱ
    B_LUX_V32D_SCL0_L;                		//����ʱ����
    delay_us(5);              		//��ʱ
    dat <<= 1;              			//�Ƴ����ݵ����λ
  }
  
  vRval = B_LUX_V32D_RecvACK();
	
	return vRval;
}

/*---------------------------------------------------------------------
 ��������: ��IIC���߽���һ���ֽ�����
 ����˵��: ��
 ��������: �����ֽ�
 ---------------------------------------------------------------------*/
u8 B_LUX_V32D_RecvByte()
{
//  u8 i;
//  u8 dat = 0;
//  SDA_IN();//B_LUX_V32D_SDA0_I;
//  
//  IIC_SDA=1;//B_LUX_V32D_SDA0_H;                         //ʹ���ڲ�����,׼����ȡ����,
//  for (i=0; i<8; i++)         	        //8λ������
//  {
//    IIC_SCL=1;//B_LUX_V32D_SCL0_H;                       //����ʱ����
//    delay_us(5);             	//��ʱ
//    dat |= B_LUX_V32D_SDA0_DAT;              //������               
//    IIC_SCL=0;//B_LUX_V32D_SCL0_L;                       //����ʱ����
//    delay_us(5);            	//��ʱ
//    
//    if (i<7)
//      dat <<= 1;
//    	
//  }
//  SDA_OUT();//B_LUX_V32D_SDA0_O;
//  
//  return dat;
	u8 i;
  u8 dat = 0;
  B_LUX_V32D_SDA0_I;
  
  B_LUX_V32D_SDA0_H;                         //ʹ���ڲ�����,׼����ȡ����,
  for (i=0; i<8; i++)         	        //8λ������
  {
    B_LUX_V32D_SCL0_H;                       //����ʱ����
    delay_us(5);              	//��ʱ
    dat |= B_LUX_V32D_SDA0_DAT;              //������               
    B_LUX_V32D_SCL0_L;                       //����ʱ����
    delay_us(5);             	//��ʱ
    
    if (i<7)
      dat <<= 1;
    	
  }
  B_LUX_V32D_SDA0_O;
  
  return dat;
}

/*---------------------------------------------------------------------
 ��������: дMAX44009
 ����˵��: REG_Address - �Ĵ�����ַ
 ��������: �ɹ�����0  ʧ�ܷ���1
 ---------------------------------------------------------------------*/
u8 B_LUX_V32D_Single_Write(u8 REG_Address, u8 REG_data)
{
	u8 vRval = 0;
	
  IIC_Start();                                        //��ʼ�ź�
  vRval += B_LUX_V32D_SendByte(B_LUX_V32D_SlaveAddress);     //�����豸��ַ+д�ź�
  vRval += B_LUX_V32D_SendByte(REG_Address);                 //�ڲ��Ĵ�����ַ��
  vRval += B_LUX_V32D_SendByte(REG_data);                    //�ڲ��Ĵ������ݣ�
  IIC_Stop();                                         //����ֹͣ�ź�
	
	return vRval;
}

/*---------------------------------------------------------------------
 ��������: ��MAX44009�ڲ�����
 ����˵��: ��
 ��������: �ɹ�����0  ʧ�ܷ���1
 ---------------------------------------------------------------------*/
u8 B_LUX_V32D_Single_Read(u8 REG_Address, u8 *vp_Dat)
{  
  u8 vRval = 0;
	
  IIC_Start();                                        //��ʼ�ź�
  vRval += B_LUX_V32D_SendByte(B_LUX_V32D_SlaveAddress+0);   //�����豸��ַ+���ź�
  vRval += B_LUX_V32D_SendByte(REG_Address);                 //�ڲ��Ĵ�����ַ��
  IIC_Start();                                        //��ʼ�ź�
  vRval += B_LUX_V32D_SendByte(B_LUX_V32D_SlaveAddress+1);   //�����豸��ַ+���ź�
  *vp_Dat = B_LUX_V32D_RecvByte();                           
  B_LUX_V32D_SendACK(1);                                     //��ӦACK
  IIC_Stop();                                         //ֹͣ�ź�
  
  return vRval;
  
}

/*---------------------------------------------------------------------
 ��������: ��MAX44009�ڲ�����
 ����˵��: ��
 ��������: �ɹ�����0  ʧ�ܷ���1
 ---------------------------------------------------------------------*/
extern u8 B_LUX_V32D_Multi_read(u8 REG_Address1, u8 REG_Address2, u8 *vBuf)
{   	
	u8 vRval = 0;
	
  //�Ĵ���1
  IIC_Start();                                        //��ʼ�ź�
  vRval += B_LUX_V32D_SendByte(B_LUX_V32D_SlaveAddress+0);   //�����豸��ַ+���ź�
  vRval += B_LUX_V32D_SendByte(REG_Address1);                //�ڲ��Ĵ�����ַ��
  IIC_Start();                                        //��ʼ�ź�
  vRval += B_LUX_V32D_SendByte(B_LUX_V32D_SlaveAddress+1);   //�����豸��ַ+���ź�
  vBuf[0] = B_LUX_V32D_RecvByte();                           
  B_LUX_V32D_SendACK(1);

  //�����Ĵ���2
  IIC_Start();                                        //��ʼ�ź�
  vRval += B_LUX_V32D_SendByte(B_LUX_V32D_SlaveAddress+0);   //�����豸��ַ+���ź�
  vRval += B_LUX_V32D_SendByte(REG_Address2);                //�ڲ��Ĵ�����ַ��
  IIC_Start();                                        //��ʼ�ź�
  vRval += B_LUX_V32D_SendByte(B_LUX_V32D_SlaveAddress+1);   //�����豸��ַ+���ź�
  vBuf[1] = B_LUX_V32D_RecvByte();                           
  B_LUX_V32D_SendACK(1);                                     //��ӦACK
  
  IIC_Stop();                                         //ֹͣ�ź�
	
	return vRval;
}

/*---------------------------------------------------------------------
 ��������: �������Ĵ������ú���
 ����˵��: val - ���ò���
 ��������: �ɹ�����0  ʧ�ܷ���1
 ---------------------------------------------------------------------*/
u8 B_LUX_V32D_RegCfg(u8 val)
{
	u8 vRval = 0;
	
  u8 valCfg = 0;
  
  valCfg = (0x00<<7)|(0x00<<6)|(0x00<<3)|(0x00<<0);               //Ĭ��800ms�ɼ�һ��   //����Ϊ�Զ���ģʽ  //���û���ʱ��800ms (���վ���0.045Lux  �����ն� 188006.4Lux)                      
  
  if (val&0x80)                //�ɼ���ʽ
  {
    valCfg |= 0x80;
  }
  if (val&0x40)                //�ɼ�ģʽ 0-�Զ�ģʽ  1-�ֶ�ģʽ
  {
    valCfg |= 0x40;
  }
  if (val&0x08)                //������
  {
    valCfg |= 0x08;
  }
  if (val&0x04)                //����ʱ��
  {
    valCfg |= 0x04;
  }
  if (val&0x02)
  {
    valCfg |= 0x02;
  }
  if (val&0x01)
  {
    valCfg |= 0x01;
  }
  
  vRval += B_LUX_V32D_Single_Write(0x02, valCfg);                               //���ò���
	
	return vRval;
}

/*---------------------------------------------------------------------
 ��������: ��ʼ�����մ�����
 ����˵��: ��
 ��������: ��
 ---------------------------------------------------------------------*/
u8 B_LUX_V32D_Init()
{
	u8 vRval = 0;
	
	RCC->APB2ENR |= (0x01<<3);    //ʹ��PORTB 
	RCC->APB2ENR |= (0x01<<4);    //ʹ��PORTC 
	
	
  B_LUX_V32D_SCL0_O;
  SDA_OUT();//B_LUX_V32D_SDA0_O;
	B_LUX_V32D_ADDR_O;
	B_LUX_V32D_ADDR_L;
  IIC_SCL=1;//B_LUX_V32D_SCL0_H;
  IIC_SDA=1;//B_LUX_V32D_SDA0_H;

  delay_ms(100);                                      //��ʱ100ms
  
  vRval = B_LUX_V32D_RegCfg(0x00);
  
	return vRval;
}
/*---------------------------------------------------------------------
 ��������: ���ն�ȡ����
 ����˵��: *vLux - ���ع���ֵ
 ��������: �ɹ�����0  ʧ�ܷ���1
 ---------------------------------------------------------------------*/
u8 B_LUX_V32D_GetLux(u32 *vLux)
{
	u8 	vRval = 0;
	
  u8   vIndex = 0;                     //ָ��
  u8   vMantissa = 0;                  //β��
  u8   vLuxBuf[3];
  u16  val16 = 0;
  fp32    vflux = 0.0;
  
  vRval += B_LUX_V32D_Multi_read(0x03, 0x04, vLuxBuf);
  
  //ָ��
  vIndex = vLuxBuf[0]&0xF0;
  vIndex >>= 4;
  
  //β��
  vMantissa   = (vLuxBuf[0]&0x0F);
  vMantissa <<= 4;
  vMantissa  |= (vLuxBuf[1]&0x0F);
  
  if (vIndex == 0x01)
  {
    vIndex = 0x00;
  }
  else if (vIndex > 0x0E)
  {
    vIndex = 0x0E;
  }
  
  val16 = (0x01<<vIndex);
  
  vflux = ( (fp32)val16 ) * ( (fp32)vMantissa) * 0.045;
  
  vflux = (vflux*1000.0);                      		 //*1000 С��λ
  (*vLux) = (u32)(vflux*1.4);                   //С��͸����ǽ���
  delay_ms(1);                          //��ʱ1s
  
	return vRval;
} 
