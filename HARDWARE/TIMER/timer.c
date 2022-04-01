#include "timer.h"
#include "led.h"
#include "usart.h"
#include "remote.h"
#include "ml302.h"
#include <stdbool.h>

//������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������//
/*��ʱ��4PWM�������ʱ��4\3�ɼ�PWM*/
//������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������// 
//
/*
PB7��PB8��Ϊ������ƣ���TIM4_CH2  TIM4_CH3
PB6��PB0��Ϊң������������TIM4_CH1  TIM3_CH3
*/
//������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������//
/*��ʱ��4PWM���*/
//������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������// 
//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!

void TIM4_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,���������ж�

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx����
							 
}
//��ʱ��2�жϷ������
void TIM4_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
			TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
			LED1=!LED1;
		}
}

//TIM4 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
//PB5------TIM_SetCompare2------��ߵ��
//PB0------TIM_SetCompare3------�ұߵ��
//PB1------TIM_SetCompare4------������ˮ���

//////PB6------TIM_SetCompare1------��ߵ��
//////PB8------TIM_SetCompare3------�ұߵ��
void TIM4_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  , ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3������ӳ��  TIM3_CH2->PB5     
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOB.5		//PB7 TIM4_CH2,��iic�г�ͻ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_8; //TIM4_CH2|TIM4_CH3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
	
	//GPIO_ResetBits(GPIOB,GPIO_Pin_1); 	//PB1Ҫ���ͣ���ΪоƬĬ�ϸߵ�ƽ����L298Nһ���Žӵأ����γɻ�·�������Ĭ�������˵�
	
   //��ʼ��TIM4
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
		 	//��ʼ��TIM4 ͨ��1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM������ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4 OC1
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR1�ϵ�Ԥװ�ؼĴ���
	
 	//��ʼ��TIM4 ͨ��2 PWMģʽ	 
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM������ȵ���ģʽ2
// 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
//	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
//	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
	
//	 	//��ʼ��TIM4 ͨ��3 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM������ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC3
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR3�ϵ�Ԥװ�ؼĴ���

//	 	//��ʼ��TIM4 ͨ��4 PWMģʽ	 
////	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM������ȵ���ģʽ2
//// 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
////	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
////	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC4
////	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR4�ϵ�Ԥװ�ؼĴ���

	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM4
	
}
  	 
//������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������//
/*��ʱ��3��4�ɼ�PWM    PB0 PB6
	��TIM4_CH1  TIM3_CH3
*/   
//������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������// 

//��ʱ��3_CH3�ɼ�PWM
TIM_ICInitTypeDef  TIM3_ICInitStructure;

void TIM3_Cap_Init(u16 arr,u16 psc)
{	 

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ��TIM3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIOAʱ��
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3������ӳ�� TIM3_CH2->PC7

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;  //PA0 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 ����  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);		

	
	//��ʼ����ʱ��3 TIM3	 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  

	//��ʼ��TIM3���벶�����
		TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  	TIM3_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
//	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC2|TIM_IT_CC3,ENABLE);//���������ж� ,����CC1IE�����ж�	
		TIM_ITConfig(TIM3,TIM_IT_CC3,ENABLE);//���������ж� ,����CC1IE�����ж�
   	
		TIM_Cmd(TIM3,ENABLE ); 	//ʹ�ܶ�ʱ��5
		

}
//u8  TIM3CH3_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM3CH3_CAPTURE_VAL;	//���벶��ֵ
//CH3
u8  TIM3CH3_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM3CH3_CAPTURE_Date2;	//����2
u16 TIM3CH3_CAPTURE_Date1;	//����1
void TIM3_IRQHandler(void)
{  	
u16 tsr;
	tsr=TIM3->SR;

	//CH3�жϴ���		
	if((TIM3CH3_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
		{		
		//			if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)			 
		if(tsr&0X01)//���
			{	    
				if(TIM3CH3_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
				{
					if((TIM3CH3_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
					{
						TIM3CH3_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
						TIM3CH3_CAPTURE_Date2=0XFFFF;
					}else TIM3CH3_CAPTURE_STA++;
				}	 
			}
		//			if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)//����1���������¼�
			if(tsr&0x08)//����1���������¼�
			{	
				if(TIM3CH3_CAPTURE_STA&0X40)		//����һ���½��� 		
				{	  			
					TIM3CH3_CAPTURE_STA|=0X80;		//��ǳɹ�����һ��������
					TIM3CH3_CAPTURE_Date2=TIM_GetCapture3(TIM3);
		//					TIM3CH3_CAPTURE_Date2=TIM3->CCR3;
					TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
		//					TIM3->CCER&=~(1<<9);
				}else  								//��δ��ʼ,��һ�β���������
				{
					TIM3CH3_CAPTURE_Date2=0;			//���
		//					TIM3CH3_CAPTURE_STA=0;
		//					TIM_SetCounter(TIM3,0);
					TIM3CH3_CAPTURE_STA=0X40;		//��ǲ�����������
		//					TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
				TIM3CH3_CAPTURE_Date1=TIM3->CCR3;
				TIM3->CCER|=1<<9; 				//CC1P=1 ����Ϊ�½��ز��� 
				}		    
			}		     	    					   
		} 
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC3|TIM_IT_Update); //����жϱ�־λ
 
}
//��ʱ��4_CH1�ɼ�PWM
TIM_ICInitTypeDef  TIM5_ICInitStructure;
void TIM5_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   	NVIC_InitTypeDef NVIC_InitStructure;
		
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//ʹ��TIM3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIOAʱ��
	
//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3������ӳ�� TIM3_CH2->PC7
//	GPIO_PinRemapConfig(GPIO_Remap_TIM4,ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;  //PA0 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 ����  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);		
	
	//TIM4_CH1
//��ʼ����ʱ��4 TIM4	 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	//��ʼ��TIM3���벶�����
		TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
  	TIM_ICInit(TIM5, &TIM5_ICInitStructure);	

	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
//	TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC3,ENABLE);//���������ж� ,����CC1IE�����ж�	
	TIM_ITConfig(TIM5,TIM_IT_CC1,ENABLE);//���������ж� ,����CC1IE�����ж�
   	TIM_Cmd(TIM5,ENABLE ); 	//ʹ�ܶ�ʱ��5	
		
}

u8  TIM5CH1_CAPTURE_STA=0;	//���벶��״̬		    				
//u16	TIM4CH1_CAPTURE_VAL;	//���벶��ֵ
	    				
u16	TIM5CH1_CAPTURE_Date2;	//����2
u16 TIM5CH1_CAPTURE_Date1;	//����1

void TIM5_IRQHandler(void)
{ 

	u16 tsr;
	tsr=TIM5->SR;
		//CH1�жϴ���
 	if((TIM5CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{	 
		if(tsr&0X01)//���
		//			if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
			{	
					if(TIM5CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
					{
						if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
						{
							TIM5CH1_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
							TIM5CH1_CAPTURE_Date2=0XFFFF;
						}else TIM5CH1_CAPTURE_STA++;
					}	 
			}
		if(tsr&0x02)//����1���������¼�
		//		if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)//����1���������¼�
			{	
				if(TIM5CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
				{	  			
					TIM5CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ��������
					TIM5CH1_CAPTURE_Date2=TIM_GetCapture1(TIM5);
						TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
				}else  								//��δ��ʼ,��һ�β���������
				{
		//					TIM3CH2_CAPTURE_Date2=0;			//���
					TIM5CH1_CAPTURE_Date2=0;
		//					TIM_SetCounter(TIM3,0);
					TIM5CH1_CAPTURE_STA=0X40;		//��ǲ�����������
					TIM5CH1_CAPTURE_Date1=TIM5->CCR1;
					TIM5->CCER|=1<<1; 				//CC1P=1 ����Ϊ�½��ز��� 
		//					TIM_OC2PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
				}					
			}								
 	}
  TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ  
 
}


// Tout= ((arr+1)*(psc+1))/Tclk��
// arr��������װֵ��psc��Ƶ����Tclkϵͳʱ��Ƶ�ʣ�Toutһ�����ڵ�ʱ�䡣
	
void TIM2_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM4��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM2�ж�,���������ж�
//	TIM_ITConfig(TIM2,TIM_IT_Trigger,ENABLE ); //ʹ��ָ����TIM4�ж�,���������ж�
	
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	
	//TIM_Cmd(TIM2,ENABLE);
						 
}
//��ʱ��2�жϷ������

void TIM2_IRQHandler(void)   //TIM3�ж�
{

		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{	
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //����жϱ�־λ	
			TIM2_IRQHandler_void();
		} 

}
extern u8 Cor_time;
u8 tim2_time=0;
void TIM2_IRQHandler_void (void)   //TIM3�ж�
{
			tim2_time++;	//��ʱ����
	
			if (tim2_time<Cor_time)	//����ʱ�䷶Χ��
			{
				NEW_IOT_Contorl_TIM2();	//ִ������������
			}
			
			if (tim2_time > Cor_time)	//����ʱ�䷶Χ��
			{
				tim2_time=0;
				TIM_Cmd(TIM2,DISABLE);
				stop();						//����ֹͣ
			}
			u1_printf("tim2_time�������� %d\r\n",tim2_time);

}


//���ݷ��͵���ʱTIM6
void TIM6_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM4��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM4�ж�,���������ж�
//	TIM_ITConfig(TIM4,TIM_IT_Trigger,ENABLE ); //ʹ��ָ����TIM4�ж�,���������ж�
	
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

		TIM_Cmd(TIM6, ENABLE);				 
}
//��ʱ��6�жϷ������
u8 data_Send_Time=0,GPS_Data_Send_Time=0;
void TIM6_IRQHandler(void)   //TIM3�ж�
{
 u8 t=0;
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
			t++;
			data_Send_Time++;
			GPS_Data_Send_Time++;
//			TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
			if(GPS_Data_Send_Time==5)
			{
				GPS_Data_Send_Time=0;
	///			ML302_GPS_MQTTPUBMessage(2);
			}
			if(data_Send_Time==10)
			{
				data_Send_Time=0;
	///			ML302_MQTTPUBMessage(2);
			}
				
		}
			 TIM_ClearITPendingBit(TIM6, TIM_IT_Update); //����жϱ�־λ
}

