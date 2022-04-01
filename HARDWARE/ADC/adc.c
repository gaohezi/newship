#include "adc.h"
#include "delay.h"
	   
//初始化ADC
//这里我们仅以规则通道为例
//我们默认将开启通道0~3				

//adc1/2的通道9对应PB1，adc123的通道12对应PC2 													   
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_ADC1	, ENABLE );	  //使能ADC1通道时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB 	, ENABLE );	  //使能ADC1通道时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	//PA1 作为模拟通道输入引脚    数采仪A12、A13端,面贴对应A2、A3对应单片机PC2\PC3                     
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值
	
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_RegInjecSimult;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
				
//		ADC_InitStructure.ADC_NbrOfChannel = 5;								//顺序进行规则转换的ADC通道的数目
//		ADC_Init(ADC1, &ADC_InitStructure);									//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器  
//		
//		
		ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_239Cycles5 );		//PB1
//		ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 2, ADC_SampleTime_239Cycles5 );
//		ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 3, ADC_SampleTime_239Cycles5 );
		ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 4, ADC_SampleTime_239Cycles5 );	//PC2
//		ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 5, ADC_SampleTime_239Cycles5 );
	
  
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1

	
	ADC_ResetCalibration(ADC1);	//使能复位校准 
 
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束

	ADC_StartCalibration(ADC1);	 //开启AD校准

	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束		 

 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能

}			

//获得ADC值
//ch:通道值 0~3
u16 Get_Adc(u8 ch)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
		
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果

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
////本程序只供学习使用，未经作者许可，不得用于其它任何用途
////ALIENTEK miniSTM32开发板
////ADC 代码	   
////正点原子@ALIENTEK
////技术论坛:www.openedv.com
////修改日期:2012/9/7
////版本：V1.0
////版权所有，盗版必究。
////Copyright(C) 广州市星翼电子科技有限公司 2009-2019
////All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////// 
//u16 g_adcData[ADC_AVE_NUM][ADC_CH_NUM];//当前值，
//float g_adcProcData=0;		//adc处理后的值	   
////初始化ADC
////这里我们仅以规则通道为例
////我们默认将开启通道0~3																	   
//void  Adc_Init(void)
//{ 	
//	ADC_InitTypeDef ADC_InitStructure; 
//	GPIO_InitTypeDef GPIO_InitStructure;
//    DMA_InitTypeDef DMA_InitStructure;


//		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //使能MDA1时钟
//		/* DMA channel1 configuration */
//		DMA_DeInit(DMA1_Channel1);  //指定DMA通道
//		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;////ADC1地址---代表ADC1保存转换值的寄存器
//		DMA_InitStructure.DMA_MemoryBaseAddr =ADC_MEMORY_ADDR;	//设置DMA内存地址，ADC转换结果直接放入该地址
//		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //外设为设置为数据传输的来源
//		DMA_InitStructure.DMA_BufferSize = 5*10;	//传输总数据
//		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址固定
//		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址自增---总体表示始终从外设ADC1地址处取值---依次保存到连续的两个内存变量中---
//		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //内存传输数据单元---半字16位
//		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
//		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//循环模式---2个数据依次循环接收从外设ADC1传输过来的ADC值---
//		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
//		DMA_Init(DMA1_Channel1, &DMA_InitStructure);

//		DMA_Cmd(DMA1_Channel1, ENABLE);//启动DMA通道一
//		
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_ADC1, ENABLE ); //使能ADC1和GPIOA、GPIOB外设时钟 
//		RCC_ADCCLKConfig(RCC_PCLK2_Div6); //设置ADC时钟分频因子为6(72M/6=12M),ADC最大工作频率为14M

//		/*设置PB9模拟通道输入引脚*/
//		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_9;	//选择要初始化的GPIOB的PA9引脚
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//工作模式为模拟输入
//		GPIO_Init(GPIOB, &GPIO_InitStructure);				//初始化GPIOB中引脚

//		/*设置PC0、PC1、PC2、PC3模拟通道输入引脚*/
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;	//选择要初始化的GPIOC的PC0、PC1、PC2、PC3引脚 GPIO_Pin_3
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//工作模式为模拟输入
//		GPIO_Init(GPIOC, &GPIO_InitStructure);				//初始化GPIOC中引脚   
//		
//		
//		/*初始化配置ADC1*/
//		ADC_DeInit(ADC1); //复位ADC1,即将ADC1的所有寄存器设置为缺省值

//		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;					//ADC1工作在独立模式
//		ADC_InitStructure.ADC_ScanConvMode = ENABLE;						//扫描模式设置，多通道下使用使能，单通道下使用失能
//		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;					//模数转换工作在连续转换模式
//		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//由软件来触发转换启动,也可以设置成外设启动
//		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;				//ADC数据右对齐
//		ADC_InitStructure.ADC_NbrOfChannel = 5;								//顺序进行规则转换的ADC通道的数目
//		ADC_Init(ADC1, &ADC_InitStructure);									//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器  
//		
//		
//		ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_239Cycles5 );
//		ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 2, ADC_SampleTime_239Cycles5 );
//		ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 3, ADC_SampleTime_239Cycles5 );
//		ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 4, ADC_SampleTime_239Cycles5 );
//		ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 5, ADC_SampleTime_239Cycles5 );
//	  
//		ADC_DMACmd(ADC1, ENABLE);                   //开启ADC的DMA支持
//		
//		ADC_Cmd(ADC1, ENABLE);						//使能ADC1，但还没启动ADC1	
//		ADC_ResetCalibration(ADC1);					//使能ADC1复位校准寄存器	 
//		while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位完成	
//		ADC_StartCalibration(ADC1);					//开启AD校准 
//		while(ADC_GetCalibrationStatus(ADC1));		//等待校准完成
//		 
//		ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能

//}				  
////获得ADC值
////ch:通道值 0~3
//u16 Get_Adc(u8 ch)   
//{
//  	//设置指定ADC的规则组通道，一个序列，采样时间
//	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
//  
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
//	 
//	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

//	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
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




































