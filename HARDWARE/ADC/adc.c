#include "adc.h"
#include "delay.h"
	   
//��ʼ��ADC
//�������ǽ��Թ���ͨ��Ϊ��
//����Ĭ�Ͻ�����ͨ��0~3				

//adc1/2��ͨ��9��ӦPB1��adc123��ͨ��12��ӦPC2 													   
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_ADC1	, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB 	, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	//PA1 ��Ϊģ��ͨ����������    ������A12��A13��,������ӦA2��A3��Ӧ��Ƭ��PC2\PC3                     
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
	
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_RegInjecSimult;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת���������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   
				
//		ADC_InitStructure.ADC_NbrOfChannel = 5;								//˳����й���ת����ADCͨ������Ŀ
//		ADC_Init(ADC1, &ADC_InitStructure);									//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���  
//		
//		
		ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_239Cycles5 );		//PB1
//		ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 2, ADC_SampleTime_239Cycles5 );
//		ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 3, ADC_SampleTime_239Cycles5 );
		ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 4, ADC_SampleTime_239Cycles5 );	//PC2
//		ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 5, ADC_SampleTime_239Cycles5 );
	
  
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1

	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼ 
 
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����

	ADC_StartCalibration(ADC1);	 //����ADУ׼

	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����		 

 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1������ת����������

}			

//���ADCֵ
//ch:ͨ��ֵ 0~3
u16 Get_Adc(u8 ch)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1������ת����������	
	 
		
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����

}


//
u16 Get_Adc_Average(u8 ch,u8 times)
{
	 u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(2);
	}
	return temp_val/times;
} 	 







// #include "adc.h"
// #include "delay.h"
// #include "string.h"
// #include "math.h"
// #include "stdlib.h"
// #include "system_init.h"
// #include "aqi.h"
// #include "sample.h"
// #include "network.h"
// #include "power.h"
////////////////////////////////////////////////////////////////////////////////////	 
////������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
////ALIENTEK miniSTM32������
////ADC ����	   
////����ԭ��@ALIENTEK
////������̳:www.openedv.com
////�޸�����:2012/9/7
////�汾��V1.0
////��Ȩ���У�����ؾ���
////Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
////All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////// 
//u16 g_adcData[ADC_AVE_NUM][ADC_CH_NUM];//��ǰֵ��
//float g_adcProcData=0;		//adc�������ֵ	   
////��ʼ��ADC
////�������ǽ��Թ���ͨ��Ϊ��
////����Ĭ�Ͻ�����ͨ��0~3																	   
//void  Adc_Init(void)
//{ 	
//	ADC_InitTypeDef ADC_InitStructure; 
//	GPIO_InitTypeDef GPIO_InitStructure;
//    DMA_InitTypeDef DMA_InitStructure;


//		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //ʹ��MDA1ʱ��
//		/* DMA channel1 configuration */
//		DMA_DeInit(DMA1_Channel1);  //ָ��DMAͨ��
//		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;////ADC1��ַ---����ADC1����ת��ֵ�ļĴ���
//		DMA_InitStructure.DMA_MemoryBaseAddr =ADC_MEMORY_ADDR;	//����DMA�ڴ��ַ��ADCת�����ֱ�ӷ���õ�ַ
//		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //����Ϊ����Ϊ���ݴ������Դ
//		DMA_InitStructure.DMA_BufferSize = 5*10;	//����������
//		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ�̶�
//		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //�ڴ��ַ����---�����ʾʼ�մ�����ADC1��ַ��ȡֵ---���α��浽�����������ڴ������---
//		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //�ڴ洫�����ݵ�Ԫ---����16λ
//		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
//		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//ѭ��ģʽ---2����������ѭ�����մ�����ADC1���������ADCֵ---
//		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
//		DMA_Init(DMA1_Channel1, &DMA_InitStructure);

//		DMA_Cmd(DMA1_Channel1, ENABLE);//����DMAͨ��һ
//		
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_ADC1, ENABLE ); //ʹ��ADC1��GPIOA��GPIOB����ʱ�� 
//		RCC_ADCCLKConfig(RCC_PCLK2_Div6); //����ADCʱ�ӷ�Ƶ����Ϊ6(72M/6=12M),ADC�����Ƶ��Ϊ14M

//		/*����PB9ģ��ͨ����������*/
//		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_9;	//ѡ��Ҫ��ʼ����GPIOB��PA9����
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//����ģʽΪģ������
//		GPIO_Init(GPIOB, &GPIO_InitStructure);				//��ʼ��GPIOB������

//		/*����PC0��PC1��PC2��PC3ģ��ͨ����������*/
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;	//ѡ��Ҫ��ʼ����GPIOC��PC0��PC1��PC2��PC3���� GPIO_Pin_3
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//����ģʽΪģ������
//		GPIO_Init(GPIOC, &GPIO_InitStructure);				//��ʼ��GPIOC������   
//		
//		
//		/*��ʼ������ADC1*/
//		ADC_DeInit(ADC1); //��λADC1,����ADC1�����мĴ�������Ϊȱʡֵ

//		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;					//ADC1�����ڶ���ģʽ
//		ADC_InitStructure.ADC_ScanConvMode = ENABLE;						//ɨ��ģʽ���ã���ͨ����ʹ��ʹ�ܣ���ͨ����ʹ��ʧ��
//		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;					//ģ��ת������������ת��ģʽ
//		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//������������ת������,Ҳ�������ó���������
//		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;				//ADC�����Ҷ���
//		ADC_InitStructure.ADC_NbrOfChannel = 5;								//˳����й���ת����ADCͨ������Ŀ
//		ADC_Init(ADC1, &ADC_InitStructure);									//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���  
//		
//		
//		ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_239Cycles5 );
//		ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 2, ADC_SampleTime_239Cycles5 );
//		ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 3, ADC_SampleTime_239Cycles5 );
//		ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 4, ADC_SampleTime_239Cycles5 );
//		ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 5, ADC_SampleTime_239Cycles5 );
//	  
//		ADC_DMACmd(ADC1, ENABLE);                   //����ADC��DMA֧��
//		
//		ADC_Cmd(ADC1, ENABLE);						//ʹ��ADC1������û����ADC1	
//		ADC_ResetCalibration(ADC1);					//ʹ��ADC1��λУ׼�Ĵ���	 
//		while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λ���	
//		ADC_StartCalibration(ADC1);					//����ADУ׼ 
//		while(ADC_GetCalibrationStatus(ADC1));		//�ȴ�У׼���
//		 
//		ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1������ת����������

//}				  
////���ADCֵ
////ch:ͨ��ֵ 0~3
//u16 Get_Adc(u8 ch)   
//{
//  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
//	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
//  
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1������ת����������	
//	 
//	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

//	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
//}

//u16 Get_Adc_Average(u8 ch,u8 times)
//{
//	u32 temp_val=0;
//	u8 t;
//	for(t=0;t<times;t++)
//	{
//		temp_val+=Get_Adc(ch);
//		delay_ms(5);
//	}
//	return temp_val/times;
//} 	 



































