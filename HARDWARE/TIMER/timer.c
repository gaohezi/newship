#include "timer.h"
#include "led.h"
#include "usart.h"
#include "remote.h"
#include "ml302.h"
#include <stdbool.h>

//————————————————————————————————————————————————————————————————————————————————————————————————————————————//
/*定时器4PWM输出，定时器4\3采集PWM*/
//————————————————————————————————————————————————————————————————————————————————————————————————————————————// 
//
/*
PB7与PB8作为输出控制，即TIM4_CH2  TIM4_CH3
PB6与PB0作为遥控器采样，即TIM4_CH1  TIM3_CH3
*/
//————————————————————————————————————————————————————————————————————————————————————————————————————————————//
/*定时器4PWM输出*/
//————————————————————————————————————————————————————————————————————————————————————————————————————————————// 
//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!

void TIM4_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM4, ENABLE);  //使能TIMx外设
							 
}
//定时器2中断服务程序
void TIM4_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
			TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
			LED1=!LED1;
		}
}

//TIM4 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
//PB5------TIM_SetCompare2------左边电机
//PB0------TIM_SetCompare3------右边电机
//PB1------TIM_SetCompare4------加速喷水电机

//////PB6------TIM_SetCompare1------左边电机
//////PB8------TIM_SetCompare3------右边电机
void TIM4_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  , ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5     
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOB.5		//PB7 TIM4_CH2,与iic有冲突
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_8; //TIM4_CH2|TIM4_CH3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
	
	//GPIO_ResetBits(GPIOB,GPIO_Pin_1); 	//PB1要拉低，因为芯片默认高电平，而L298N一个脚接地，会形成回路，电机就默认驱动了的
	
   //初始化TIM4
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
		 	//初始化TIM4 通道1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC1
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR1上的预装载寄存器
	
 	//初始化TIM4 通道2 PWM模式	 
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
// 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
//	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
//	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
	
//	 	//初始化TIM4 通道3 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC3
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR3上的预装载寄存器

//	 	//初始化TIM4 通道4 PWM模式	 
////	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
//// 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
////	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
////	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC4
////	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR4上的预装载寄存器

	TIM_Cmd(TIM4, ENABLE);  //使能TIM4
	
}
  	 
//————————————————————————————————————————————————————————————————————————————————————————————————————————————//
/*定时器3和4采集PWM    PB0 PB6
	即TIM4_CH1  TIM3_CH3
*/   
//————————————————————————————————————————————————————————————————————————————————————————————————————————————// 

//定时器3_CH3采集PWM
TIM_ICInitTypeDef  TIM3_ICInitStructure;

void TIM3_Cap_Init(u16 arr,u16 psc)
{	 

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能TIM3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);  //使能GPIOA时钟
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射 TIM3_CH2->PC7

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;  //PA0 清除之前设置  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 输入  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);		

	
	//初始化定时器3 TIM3	 
	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  

	//初始化TIM3输入捕获参数
		TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3; //CC1S=01 	选择输入端 IC1映射到TI1上
  	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  	TIM3_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
//	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC2|TIM_IT_CC3,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
		TIM_ITConfig(TIM3,TIM_IT_CC3,ENABLE);//允许更新中断 ,允许CC1IE捕获中断
   	
		TIM_Cmd(TIM3,ENABLE ); 	//使能定时器5
		

}
//u8  TIM3CH3_CAPTURE_STA=0;	//输入捕获状态		    				
u16	TIM3CH3_CAPTURE_VAL;	//输入捕获值
//CH3
u8  TIM3CH3_CAPTURE_STA=0;	//输入捕获状态		    				
u16	TIM3CH3_CAPTURE_Date2;	//数据2
u16 TIM3CH3_CAPTURE_Date1;	//数据1
void TIM3_IRQHandler(void)
{  	
u16 tsr;
	tsr=TIM3->SR;

	//CH3中断处理		
	if((TIM3CH3_CAPTURE_STA&0X80)==0)//还未成功捕获	
		{		
		//			if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)			 
		if(tsr&0X01)//溢出
			{	    
				if(TIM3CH3_CAPTURE_STA&0X40)//已经捕获到高电平了
				{
					if((TIM3CH3_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
					{
						TIM3CH3_CAPTURE_STA|=0X80;//标记成功捕获了一次
						TIM3CH3_CAPTURE_Date2=0XFFFF;
					}else TIM3CH3_CAPTURE_STA++;
				}	 
			}
		//			if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)//捕获1发生捕获事件
			if(tsr&0x08)//捕获1发生捕获事件
			{	
				if(TIM3CH3_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
				{	  			
					TIM3CH3_CAPTURE_STA|=0X80;		//标记成功捕获到一次上升沿
					TIM3CH3_CAPTURE_Date2=TIM_GetCapture3(TIM3);
		//					TIM3CH3_CAPTURE_Date2=TIM3->CCR3;
					TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
		//					TIM3->CCER&=~(1<<9);
				}else  								//还未开始,第一次捕获上升沿
				{
					TIM3CH3_CAPTURE_Date2=0;			//清空
		//					TIM3CH3_CAPTURE_STA=0;
		//					TIM_SetCounter(TIM3,0);
					TIM3CH3_CAPTURE_STA=0X40;		//标记捕获到了上升沿
		//					TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
				TIM3CH3_CAPTURE_Date1=TIM3->CCR3;
				TIM3->CCER|=1<<9; 				//CC1P=1 设置为下降沿捕获 
				}		    
			}		     	    					   
		} 
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC3|TIM_IT_Update); //清除中断标志位
 
}
//定时器4_CH1采集PWM
TIM_ICInitTypeDef  TIM5_ICInitStructure;
void TIM5_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   	NVIC_InitTypeDef NVIC_InitStructure;
		
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//使能TIM3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);  //使能GPIOA时钟
	
//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射 TIM3_CH2->PC7
//	GPIO_PinRemapConfig(GPIO_Remap_TIM4,ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;  //PA0 清除之前设置  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 输入  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);		
	
	//TIM4_CH1
//初始化定时器4 TIM4	 
	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  
	//初始化TIM3输入捕获参数
		TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
  	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
  	TIM_ICInit(TIM5, &TIM5_ICInitStructure);	

	
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
//	TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC3,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	TIM_ITConfig(TIM5,TIM_IT_CC1,ENABLE);//允许更新中断 ,允许CC1IE捕获中断
   	TIM_Cmd(TIM5,ENABLE ); 	//使能定时器5	
		
}

u8  TIM5CH1_CAPTURE_STA=0;	//输入捕获状态		    				
//u16	TIM4CH1_CAPTURE_VAL;	//输入捕获值
	    				
u16	TIM5CH1_CAPTURE_Date2;	//数据2
u16 TIM5CH1_CAPTURE_Date1;	//数据1

void TIM5_IRQHandler(void)
{ 

	u16 tsr;
	tsr=TIM5->SR;
		//CH1中断处理
 	if((TIM5CH1_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{	 
		if(tsr&0X01)//溢出
		//			if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
			{	
					if(TIM5CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
					{
						if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
						{
							TIM5CH1_CAPTURE_STA|=0X80;//标记成功捕获了一次
							TIM5CH1_CAPTURE_Date2=0XFFFF;
						}else TIM5CH1_CAPTURE_STA++;
					}	 
			}
		if(tsr&0x02)//捕获1发生捕获事件
		//		if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)//捕获1发生捕获事件
			{	
				if(TIM5CH1_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
				{	  			
					TIM5CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次上升沿
					TIM5CH1_CAPTURE_Date2=TIM_GetCapture1(TIM5);
						TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
				}else  								//还未开始,第一次捕获上升沿
				{
		//					TIM3CH2_CAPTURE_Date2=0;			//清空
					TIM5CH1_CAPTURE_Date2=0;
		//					TIM_SetCounter(TIM3,0);
					TIM5CH1_CAPTURE_STA=0X40;		//标记捕获到了上升沿
					TIM5CH1_CAPTURE_Date1=TIM5->CCR1;
					TIM5->CCER|=1<<1; 				//CC1P=1 设置为下降沿捕获 
		//					TIM_OC2PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
				}					
			}								
 	}
  TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位  
 
}


// Tout= ((arr+1)*(psc+1))/Tclk；
// arr：计数重装值，psc分频数，Tclk系统时钟频率，Tout一个周期的时间。
	
void TIM2_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	//定时器TIM4初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM2中断,允许更新中断
//	TIM_ITConfig(TIM2,TIM_IT_Trigger,ENABLE ); //使能指定的TIM4中断,允许更新中断
	
	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	
	//TIM_Cmd(TIM2,ENABLE);
						 
}
//定时器2中断服务程序

void TIM2_IRQHandler(void)   //TIM3中断
{

		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{	
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //清除中断标志位	
			TIM2_IRQHandler_void();
		} 

}
extern u8 Cor_time;
u8 tim2_time=0;
void TIM2_IRQHandler_void (void)   //TIM3中断
{
			tim2_time++;	//定时参数
	
			if (tim2_time<Cor_time)	//合理时间范围内
			{
				NEW_IOT_Contorl_TIM2();	//执行物联网控制
			}
			
			if (tim2_time > Cor_time)	//合理时间范围外
			{
				tim2_time=0;
				TIM_Cmd(TIM2,DISABLE);
				stop();						//控制停止
			}
			u1_printf("tim2_time———— %d\r\n",tim2_time);

}


//数据发送倒计时TIM6
void TIM6_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //时钟使能
	
	//定时器TIM4初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //使能指定的TIM4中断,允许更新中断
//	TIM_ITConfig(TIM4,TIM_IT_Trigger,ENABLE ); //使能指定的TIM4中断,允许更新中断
	
	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

		TIM_Cmd(TIM6, ENABLE);				 
}
//定时器6中断服务程序
u8 data_Send_Time=0,GPS_Data_Send_Time=0;
void TIM6_IRQHandler(void)   //TIM3中断
{
 u8 t=0;
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{
			t++;
			data_Send_Time++;
			GPS_Data_Send_Time++;
//			TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );  //清除TIMx更新中断标志 
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
			 TIM_ClearITPendingBit(TIM6, TIM_IT_Update); //清除中断标志位
}


