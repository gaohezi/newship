#include "remote.h"
#include "delay.h"
#include "time.h"
#include "usart.h"
#include "usart3.h"
#include "ml302.h"
#include "flash.h"
#include "wdg.h"
#include <stdio.h>
#include "stdlib.h"

#define IOT_calibration_VAL  20		//设置偏移量校准极限值，不超过30%
#define IOT_calibration_data 2  //设置偏移量每次校准值，每次为3%
#define IOT_calibration_CH1_VAL 0  //设置固定偏移量，每次校准值，固定为5%
#define IOT_calibration_CH2_VAL 0  //设置固定偏移量，每次校准值，固定为5%



/*此文件夹用来为物联网偏移量进行控制与补偿*/
//extern bool RemoteFlag ;

extern bool send_data_float;

bool RemoteFlag = false;//遥控器控制标识符，默认为0
bool MQTTFlag = false;

//要写入到STM32 FLASH的字符串数组
extern  u8 TEXT_Buffer[200];

// const u32 TEXT_Buffer[200];

extern u16 len;

extern char *strx,*extstrx;
extern char  RxBuffer[900],RxCounter;

//CH3
extern u8  TIM3CH3_CAPTURE_STA;	//输入捕获状态		    				
extern u16	TIM3CH3_CAPTURE_Date2;	//数据2
extern u16 TIM3CH3_CAPTURE_Date1;	//数据1
extern u8  TIM5CH1_CAPTURE_STA;	//输入捕获状态		    				
extern u16	TIM5CH1_CAPTURE_Date2;	//数据2
extern u16 TIM5CH1_CAPTURE_Date1;	//数据1



			u32 temp5_1=0;
			u32 temp3_3=0;		
			u32 temp_CH2;
			u32 temp_CH3;
/*-------------------------------------------------*/
/*函数名：遥控器配对判断                           */
/*参  数：无                                       */
/*返回值：无                                       */
/*乐迪是断电999 ，配对完成988                     */
/*飞盈佳乐断电是900，配对完成1080                      */
/*-------------------------------------------------*/
//飞盈佳乐电调电平极限：   高油门：18014/18015   中油门：18502、18503   低油门：19009-19012   断电时19001、19002
void Remote_contorl_status(void)
{

		if(TIM5CH1_CAPTURE_STA&0X80)			//成功捕获到了一次高电平
				{			
					temp3_3=TIM5CH1_CAPTURE_STA&0X3F;
					temp3_3*=65536;						//溢出时间总和
					temp3_3+=TIM5CH1_CAPTURE_Date2;		
					temp3_3-=TIM5CH1_CAPTURE_Date1;		//得到总的高电平时间
					u1_printf("TIM2_CH2_HIGH:%d us\r\n",temp3_3);		//打印总的高点平时间
					TIM5CH1_CAPTURE_Date1=0;
					TIM5CH1_CAPTURE_STA=0;				//开启下一次捕获	
					
					if(temp3_3<2120&&temp3_3>1120)		//判断高电平是否大于1000us，遥控器断电时为999，配对完成时为1080		1120-2120
						{
							RemoteFlag=1;		//遥控器开启标识符
							 //send_data_float=0;
						}						
				}
		else
				RemoteFlag=0;

}

/*-------------------------------------------------*/
/*函数名：物联网配对判断                           */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/

		u8 Cor_time = 0;

void IOT_contorl_status(void)
{
	if(strstr((const char*)RxBuffer,(const char*)"/shadow/get/")&& strstr((const char*)RxBuffer,(const char*)"version"))	//IOT控制命令下发
			{		
					MQTTFlag=1;				//物联网控制开启标识符
				
					send_data_float=0;	//数据上传标识符	

					Cor_time	=	ML302_Time_Message_shadow()-1;				

//							a=0;
//							TIM_Cmd(TIM2,ENABLE);//开启定时器 这样就能实现你需要的功能了	
						
			}
	
				
				
}

/*-------------------------------------------------*/
/*函数名：TIM3CH2信号采集                          */
/*参  数：无                                       */
/*返回值：temp2                                    */
/*时间：2021-10-28                                 */
/*-------------------------------------------------*/
	u16 TIM3CH2_collect(void)
	{
					if(TIM3CH3_CAPTURE_STA&0X80)			//成功捕获到了一次高电平
				{			
					temp3_3=TIM3CH3_CAPTURE_STA&0X3F;
					temp3_3*=65536;						//溢出时间总和
					temp3_3+=	TIM3CH3_CAPTURE_Date2;		
					temp3_3-= TIM3CH3_CAPTURE_Date1;		//得到总的高电平时间
			//		u1_printf("-----TIM2_CH2_HIGH:%d us\r\n",temp3_3);		//打印总的高点平时间
					TIM3CH3_CAPTURE_Date1	=	0;
					TIM3CH3_CAPTURE_STA		=	0;				//开启下一次捕获			
				}

		return	temp3_3;
	}


/*-------------------------------------------------*/
/*函数名：TIM3CH3信号采集                          */
/*参  数：无                                       */
/*返回值：temp3                                    */
/*时间：2021-10-28                                 */
/*-------------------------------------------------*/
	u16 TIM3CH3_collect(void)
	{
	if(TIM5CH1_CAPTURE_STA&0X80)//成功捕获到了一次上升沿
		{
			temp5_1=TIM5CH1_CAPTURE_STA&0X3F;
			temp5_1*=65536;//溢出时间总和
//			temp5_1+=TIM4CH1_CAPTURE_VAL;//得到总的高电平时间
					temp5_1+=TIM5CH1_CAPTURE_Date2;		
					temp5_1-=TIM5CH1_CAPTURE_Date1;		//得到总的高电平时间
		///	u1_printf("temp5_1:%d us\r\n",temp5_1);//打印总的高点平时间
			TIM5CH1_CAPTURE_STA=0;//开启下一次捕获
		}
		
					if(temp5_1<Remote_CH3_Max_Val&&temp5_1>Remote_CH3_Min_Val)		//判断高电平是否大于1000us，遥控器断电时为999，配对完成时为1080
						{
							RemoteFlag=1;		//遥控器开启标识符
							 //send_data_float=0;
						}						
						else
							RemoteFlag=0;
		return	temp5_1;
	}

/*-------------------------------------------------*/
/*函数名：电机1（左）信号值输出                    */
/*参  数：as 偏移量                                */
/*返回值：temp_CH2                                 */
/*时间：2021-10-28                                 */
/*-------------------------------------------------*/
	u16  Remote_contorl_left()	//油门采样通道
	{	
		u16 pencent_1;
		u16 Remote_Collect_Data;	
		Remote_Collect_Data = TIM3CH2_collect();
		
		if(Remote_Collect_Data	<	Remote_CH2_Max_Val	&&	Remote_Collect_Data	>	Remote_CH2_Min_Val)		//判断高电平是否大于1000us，遥控器断电时为999，配对完成时为1080
		{
			RemoteFlag=1;		//遥控器开启标识符
			 //send_data_float=0;
			Remote_Collect_Data = TIM3CH2_collect();
		}						

		else
		RemoteFlag=0;
		pencent_1=(Remote_Collect_Data-1000)*1.0;
	
		//u1_printf("pencent_1：%d\r\n	  ",pencent_1);//1\459		2\508
		if (450<pencent_1 && pencent_1<550  )	//	遥控器连接正常，但无信号输出
	{
		pencent_1 = 500;
	}

		return pencent_1;
	}
/*-------------------------------------------------*/
/*函数名：电机2（右）信号值输出                    */
/*参  数：as 偏移量                                */
/*返回值：temp_CH3                                 */
/*时间：2021-10-28                                 */
/*-------------------------------------------------*/
	u16 Remote_contorl_right()
	{
		u16 pencent_2;		
		u16 ad2;
	
		ad2 = TIM3CH3_collect();
		pencent_2	=	ad2-1000;
		//u1_printf("		pencent_2：%d\r\n   ",pencent_2);//1\459		2\508
	if (  450<pencent_2 && pencent_2<550)	//	遥控器连接正常，但无信号输出
	{
		pencent_2 = 500;
	}

		return pencent_2;
			
	}

/*-------------------------------------------------*/
/*函数名：IOT控制左电机  				                   */
/*参  数：无                                       */
/*返回值：无                                       */
/*时间：2021-11-4                                  */
/*-------------------------------------------------*/
u16 IOT_contorl_left(u16 con_CH2,	u16 CH2_correct_data)
{	
	u16 CH2_flash_data=0;
	 u16 CH2_iot_OUT;
		ALIyun_contorl_calibration1();
		CH2_flash_data =	STMFLASH_ReadHalfWord(CH2_VAL_FLASH_SAVE_ADDR); //校准比例
		CH2_correct_data = CH2_flash_data *( flycolor_high - flycolor_low )/100;//校准值

////		con_CH2 = ML302_Left_Message();//输入18000-19000【物模型】
			con_CH2 = ML302_Left_Message_shadow();
		//ch2_data = con_CH2;
		if(	con_CH2 != flycolor_ch2_stop)
	///	CH2_iot_OUT = con_CH2 + CH2_correct_data;
				CH2_iot_OUT = con_CH2 ;
		if(	con_CH2 == flycolor_ch2_stop)
		CH2_iot_OUT = flycolor_ch2_stop;
	//IWDG_Feed();//喂狗

	return CH2_iot_OUT;
	
}
/*-------------------------------------------------*/
/*函数名：IOT控制右电机  				                   */
/*参  数：无                                       */
/*返回值：无                                       */
/*时间：2021-11-4                                  */
/*-------------------------------------------------*/
u16 IOT_contorl_right(u16 con_CH3,	u16 CH3_correct_data)
{
	
	u16 CH3_flash_data=0;
	u16 CH3_iot_OUT;
		ALIyun_contorl_calibration2();
		CH3_flash_data =	STMFLASH_ReadHalfWord(CH3_VAL_FLASH_SAVE_ADDR); 
		CH3_correct_data = CH3_flash_data * (flycolor_high - flycolor_low )/100;
		
////		con_CH3 = ML302_Right_Message();
		con_CH3 = ML302_Right_Message_shadow();
		
			if(	con_CH3 != flycolor_ch3_stop)
	///	CH3_iot_OUT = con_CH3 + CH3_correct_data;
					CH3_iot_OUT = con_CH3 ;
		if(	con_CH3 == flycolor_ch3_stop)
		CH3_iot_OUT = flycolor_ch3_stop;
	//	IWDG_Feed();//喂狗

		return CH3_iot_OUT;
}


/*-------------------------------------------------*/
/*函数名：物联网电机 控制偏移量校准                */
/*参  数：无                                       */
/*返回值：电机 偏移量0%-30%                        */
///*-------------------------------------------------*/	
		
void ALIyun_contorl_calibration1(void)
{
			static	u16 calibra_data1=0;//电机1偏移量					
	int j; 
	 u32 databuf[]={0};
	 u8 databuf_len;
	calibra_data1 =	STMFLASH_ReadHalfWord(CH2_VAL_FLASH_SAVE_ADDR);
	strx=strstr((const char*)RxBuffer,(const char*)"Boat_Calibration");//·如果返回的信息里面有+Boat_Calibration: 这表明传输成功	
	if(strx)
	{
//////--------------------固定偏移量------------------------------
			if(strstr((const char*)RxBuffer,(const char*)"\"Boat_Calibration\":0"))			    //固定值5%校准
				{														
						calibra_data1 = IOT_calibration_CH1_VAL;	//固定值0%校准
							databuf_len=sizeof databuf/sizeof (databuf[0]);	//计算数据存储部分的长度
											
								for (j=0;j<databuf_len;j++)		//将数据存入buf中
								{
										databuf[j]=calibra_data1 ;
								}										
								STMFLASH_Write(CH2_VAL_FLASH_SAVE_ADDR,(u16*)databuf,databuf_len);//CH1写flash
				}
							
//*******************CH1的偏移量
								else if(strstr((const char*)RxBuffer,(const char*)"\"Boat_Calibration\":1"))		//左电机正校准
									{		
											calibra_data1 = calibra_data1 + IOT_calibration_data;
										
											if(calibra_data1 >= IOT_calibration_VAL)			//判断是否超过设定的偏移量阈值
												{
													calibra_data1= IOT_calibration_VAL;		//偏移量阈值最高为30%
												}
											else calibra_data1 = calibra_data1;
											
										databuf_len=sizeof databuf/sizeof (databuf[0]);//计算databuf数量
													
										for (j=0;j<databuf_len;j++)
										{
												databuf[j]=calibra_data1;
										}
												STMFLASH_Write(CH2_VAL_FLASH_SAVE_ADDR,(u16*)databuf,databuf_len);			
									}	
							else if(strstr((const char*)RxBuffer,(const char*)"\"Boat_Calibration\":2"))		//左电机负校准
								{											
									//	Clear_Buffer();		
										calibra_data1 = calibra_data1 - IOT_calibration_data;
									
										if(calibra_data1 >= IOT_calibration_VAL)			//判断是否超过设定的偏移量阈值
												{
													calibra_data1= 0;		//偏移量阈值最低为0%
												}
										else calibra_data1 = calibra_data1;											
															
											databuf_len=sizeof databuf/sizeof (databuf[0]);
												for (j=0;j<databuf_len;j++)
												{
														databuf[j]=calibra_data1 ;
												}
											STMFLASH_Write(CH2_VAL_FLASH_SAVE_ADDR,(u16*)databuf,databuf_len);																	
								}												
	}
}


void ALIyun_contorl_calibration2(void)
{	
			static	u16 calibra_data2=0;	//电机2偏移量			
	int j; 
	 u32 databuf[]={0};
	 u8 databuf_len;
	calibra_data2  =	STMFLASH_ReadHalfWord(CH3_VAL_FLASH_SAVE_ADDR);				//取内存中的calibra_data值
	strx=strstr((const char*)RxBuffer,(const char*)"Boat_Calibration");//·如果返回的信息里面有+Boat_Calibration: 这表明传输成功	
	if(strx)
	{
			if(strstr((const char*)RxBuffer,(const char*)"\"Boat_Calibration\":0"))			    //固定值5%校准
				{													
						calibra_data2 = IOT_calibration_CH2_VAL;	//固定值0%校准
							databuf_len=sizeof databuf/sizeof (databuf[0]);	//计算数据存储部分的长度
											
								for (j=0;j<databuf_len;j++)		//将数据存入buf中
								{
										databuf[j]=calibra_data2 ;
								}										

								STMFLASH_Write(CH3_VAL_FLASH_SAVE_ADDR,(u16*)databuf,databuf_len);//CH2写flash						
				}
						//////--------------------可变偏移量------------------------------
							
//*******************CH2的偏移量
						else if(strstr((const char*)RxBuffer,(const char*)"\"Boat_Calibration\":3"))		//左电机正校准
						{

								calibra_data2 = calibra_data2 + IOT_calibration_data;
								
									if(calibra_data2 >= IOT_calibration_VAL)			//判断是否超过设定的偏移量阈值
								{
									calibra_data2= IOT_calibration_VAL;		//偏移量阈值最高为30%
								}
									else calibra_data2 = calibra_data2;
									
								databuf_len=sizeof databuf/sizeof (databuf[0]);
											
								for (j=0;j<databuf_len;j++)
								{
										databuf[j]=calibra_data2 ;
								}
										STMFLASH_Write(CH3_VAL_FLASH_SAVE_ADDR,(u16*)databuf,databuf_len);														
						}		
							else if(strstr((const char*)RxBuffer,(const char*)"\"Boat_Calibration\":4"))		//左电机负校准
							{
	
										calibra_data2 = calibra_data2 - IOT_calibration_data;
								
									if(calibra_data2 >= IOT_calibration_VAL)			//判断是否超过设定的偏移量阈值
								{
									calibra_data2= 0;		//偏移量阈值最低为0%
								}
								else calibra_data2 = calibra_data2;											
														
										databuf_len=sizeof databuf/sizeof (databuf[0]);
											for (j=0;j<databuf_len;j++)
											{
													databuf[j]=calibra_data2 ;
											}
										STMFLASH_Write(CH3_VAL_FLASH_SAVE_ADDR,(u16*)databuf,databuf_len);	
							}
	}
}


