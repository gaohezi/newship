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

#define IOT_calibration_VAL  20		//����ƫ����У׼����ֵ��������30%
#define IOT_calibration_data 2  //����ƫ����ÿ��У׼ֵ��ÿ��Ϊ3%
#define IOT_calibration_CH1_VAL 0  //���ù̶�ƫ������ÿ��У׼ֵ���̶�Ϊ5%
#define IOT_calibration_CH2_VAL 0  //���ù̶�ƫ������ÿ��У׼ֵ���̶�Ϊ5%



/*���ļ�������Ϊ������ƫ�������п����벹��*/
//extern bool RemoteFlag ;

extern bool send_data_float;

bool RemoteFlag = false;//ң�������Ʊ�ʶ����Ĭ��Ϊ0
bool MQTTFlag = false;

//Ҫд�뵽STM32 FLASH���ַ�������
extern  u8 TEXT_Buffer[200];

// const u32 TEXT_Buffer[200];

extern u16 len;

extern char *strx,*extstrx;
extern char  RxBuffer[900],RxCounter;

//CH3
extern u8  TIM3CH3_CAPTURE_STA;	//���벶��״̬		    				
extern u16	TIM3CH3_CAPTURE_Date2;	//����2
extern u16 TIM3CH3_CAPTURE_Date1;	//����1
extern u8  TIM5CH1_CAPTURE_STA;	//���벶��״̬		    				
extern u16	TIM5CH1_CAPTURE_Date2;	//����2
extern u16 TIM5CH1_CAPTURE_Date1;	//����1



			u32 temp5_1=0;
			u32 temp3_3=0;		
			u32 temp_CH2;
			u32 temp_CH3;
/*-------------------------------------------------*/
/*��������ң��������ж�                           */
/*��  ������                                       */
/*����ֵ����                                       */
/*�ֵ��Ƕϵ�999 ��������988                     */
/*��ӯ���ֶϵ���900��������1080                      */
/*-------------------------------------------------*/
//��ӯ���ֵ����ƽ���ޣ�   �����ţ�18014/18015   �����ţ�18502��18503   �����ţ�19009-19012   �ϵ�ʱ19001��19002
void Remote_contorl_status(void)
{

		if(TIM5CH1_CAPTURE_STA&0X80)			//�ɹ�������һ�θߵ�ƽ
				{			
					temp3_3=TIM5CH1_CAPTURE_STA&0X3F;
					temp3_3*=65536;						//���ʱ���ܺ�
					temp3_3+=TIM5CH1_CAPTURE_Date2;		
					temp3_3-=TIM5CH1_CAPTURE_Date1;		//�õ��ܵĸߵ�ƽʱ��
					u1_printf("TIM2_CH2_HIGH:%d us\r\n",temp3_3);		//��ӡ�ܵĸߵ�ƽʱ��
					TIM5CH1_CAPTURE_Date1=0;
					TIM5CH1_CAPTURE_STA=0;				//������һ�β���	
					
					if(temp3_3<2120&&temp3_3>1120)		//�жϸߵ�ƽ�Ƿ����1000us��ң�����ϵ�ʱΪ999��������ʱΪ1080		1120-2120
						{
							RemoteFlag=1;		//ң����������ʶ��
							 //send_data_float=0;
						}						
				}
		else
				RemoteFlag=0;

}

/*-------------------------------------------------*/
/*������������������ж�                           */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/

		u8 Cor_time = 0;

void IOT_contorl_status(void)
{
	if(strstr((const char*)RxBuffer,(const char*)"/shadow/get/")&& strstr((const char*)RxBuffer,(const char*)"version"))	//IOT���������·�
			{		
					MQTTFlag=1;				//���������ƿ�����ʶ��
				
					send_data_float=0;	//�����ϴ���ʶ��	

					Cor_time	=	ML302_Time_Message_shadow()-1;				

//							a=0;
//							TIM_Cmd(TIM2,ENABLE);//������ʱ�� ��������ʵ������Ҫ�Ĺ�����	
						
			}
	
				
				
}

/*-------------------------------------------------*/
/*��������TIM3CH2�źŲɼ�                          */
/*��  ������                                       */
/*����ֵ��temp2                                    */
/*ʱ�䣺2021-10-28                                 */
/*-------------------------------------------------*/
	u16 TIM3CH2_collect(void)
	{
					if(TIM3CH3_CAPTURE_STA&0X80)			//�ɹ�������һ�θߵ�ƽ
				{			
					temp3_3=TIM3CH3_CAPTURE_STA&0X3F;
					temp3_3*=65536;						//���ʱ���ܺ�
					temp3_3+=	TIM3CH3_CAPTURE_Date2;		
					temp3_3-= TIM3CH3_CAPTURE_Date1;		//�õ��ܵĸߵ�ƽʱ��
			//		u1_printf("-----TIM2_CH2_HIGH:%d us\r\n",temp3_3);		//��ӡ�ܵĸߵ�ƽʱ��
					TIM3CH3_CAPTURE_Date1	=	0;
					TIM3CH3_CAPTURE_STA		=	0;				//������һ�β���			
				}

		return	temp3_3;
	}


/*-------------------------------------------------*/
/*��������TIM3CH3�źŲɼ�                          */
/*��  ������                                       */
/*����ֵ��temp3                                    */
/*ʱ�䣺2021-10-28                                 */
/*-------------------------------------------------*/
	u16 TIM3CH3_collect(void)
	{
	if(TIM5CH1_CAPTURE_STA&0X80)//�ɹ�������һ��������
		{
			temp5_1=TIM5CH1_CAPTURE_STA&0X3F;
			temp5_1*=65536;//���ʱ���ܺ�
//			temp5_1+=TIM4CH1_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
					temp5_1+=TIM5CH1_CAPTURE_Date2;		
					temp5_1-=TIM5CH1_CAPTURE_Date1;		//�õ��ܵĸߵ�ƽʱ��
		///	u1_printf("temp5_1:%d us\r\n",temp5_1);//��ӡ�ܵĸߵ�ƽʱ��
			TIM5CH1_CAPTURE_STA=0;//������һ�β���
		}
		
					if(temp5_1<Remote_CH3_Max_Val&&temp5_1>Remote_CH3_Min_Val)		//�жϸߵ�ƽ�Ƿ����1000us��ң�����ϵ�ʱΪ999��������ʱΪ1080
						{
							RemoteFlag=1;		//ң����������ʶ��
							 //send_data_float=0;
						}						
						else
							RemoteFlag=0;
		return	temp5_1;
	}

/*-------------------------------------------------*/
/*�����������1�����ź�ֵ���                    */
/*��  ����as ƫ����                                */
/*����ֵ��temp_CH2                                 */
/*ʱ�䣺2021-10-28                                 */
/*-------------------------------------------------*/
	u16  Remote_contorl_left()	//���Ų���ͨ��
	{	
		u16 pencent_1;
		u16 Remote_Collect_Data;	
		Remote_Collect_Data = TIM3CH2_collect();
		
		if(Remote_Collect_Data	<	Remote_CH2_Max_Val	&&	Remote_Collect_Data	>	Remote_CH2_Min_Val)		//�жϸߵ�ƽ�Ƿ����1000us��ң�����ϵ�ʱΪ999��������ʱΪ1080
		{
			RemoteFlag=1;		//ң����������ʶ��
			 //send_data_float=0;
			Remote_Collect_Data = TIM3CH2_collect();
		}						

		else
		RemoteFlag=0;
		pencent_1=(Remote_Collect_Data-1000)*1.0;
	
		//u1_printf("pencent_1��%d\r\n	  ",pencent_1);//1\459		2\508
		if (450<pencent_1 && pencent_1<550  )	//	ң�������������������ź����
	{
		pencent_1 = 500;
	}

		return pencent_1;
	}
/*-------------------------------------------------*/
/*�����������2���ң��ź�ֵ���                    */
/*��  ����as ƫ����                                */
/*����ֵ��temp_CH3                                 */
/*ʱ�䣺2021-10-28                                 */
/*-------------------------------------------------*/
	u16 Remote_contorl_right()
	{
		u16 pencent_2;		
		u16 ad2;
	
		ad2 = TIM3CH3_collect();
		pencent_2	=	ad2-1000;
		//u1_printf("		pencent_2��%d\r\n   ",pencent_2);//1\459		2\508
	if (  450<pencent_2 && pencent_2<550)	//	ң�������������������ź����
	{
		pencent_2 = 500;
	}

		return pencent_2;
			
	}

/*-------------------------------------------------*/
/*��������IOT��������  				                   */
/*��  ������                                       */
/*����ֵ����                                       */
/*ʱ�䣺2021-11-4                                  */
/*-------------------------------------------------*/
u16 IOT_contorl_left(u16 con_CH2,	u16 CH2_correct_data)
{	
	u16 CH2_flash_data=0;
	 u16 CH2_iot_OUT;
		ALIyun_contorl_calibration1();
		CH2_flash_data =	STMFLASH_ReadHalfWord(CH2_VAL_FLASH_SAVE_ADDR); //У׼����
		CH2_correct_data = CH2_flash_data *( flycolor_high - flycolor_low )/100;//У׼ֵ

////		con_CH2 = ML302_Left_Message();//����18000-19000����ģ�͡�
			con_CH2 = ML302_Left_Message_shadow();
		//ch2_data = con_CH2;
		if(	con_CH2 != flycolor_ch2_stop)
	///	CH2_iot_OUT = con_CH2 + CH2_correct_data;
				CH2_iot_OUT = con_CH2 ;
		if(	con_CH2 == flycolor_ch2_stop)
		CH2_iot_OUT = flycolor_ch2_stop;
	//IWDG_Feed();//ι��

	return CH2_iot_OUT;
	
}
/*-------------------------------------------------*/
/*��������IOT�����ҵ��  				                   */
/*��  ������                                       */
/*����ֵ����                                       */
/*ʱ�䣺2021-11-4                                  */
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
	//	IWDG_Feed();//ι��

		return CH3_iot_OUT;
}


/*-------------------------------------------------*/
/*����������������� ����ƫ����У׼                */
/*��  ������                                       */
/*����ֵ����� ƫ����0%-30%                        */
///*-------------------------------------------------*/	
		
void ALIyun_contorl_calibration1(void)
{
			static	u16 calibra_data1=0;//���1ƫ����					
	int j; 
	 u32 databuf[]={0};
	 u8 databuf_len;
	calibra_data1 =	STMFLASH_ReadHalfWord(CH2_VAL_FLASH_SAVE_ADDR);
	strx=strstr((const char*)RxBuffer,(const char*)"Boat_Calibration");//��������ص���Ϣ������+Boat_Calibration: ���������ɹ�	
	if(strx)
	{
//////--------------------�̶�ƫ����------------------------------
			if(strstr((const char*)RxBuffer,(const char*)"\"Boat_Calibration\":0"))			    //�̶�ֵ5%У׼
				{														
						calibra_data1 = IOT_calibration_CH1_VAL;	//�̶�ֵ0%У׼
							databuf_len=sizeof databuf/sizeof (databuf[0]);	//�������ݴ洢���ֵĳ���
											
								for (j=0;j<databuf_len;j++)		//�����ݴ���buf��
								{
										databuf[j]=calibra_data1 ;
								}										
								STMFLASH_Write(CH2_VAL_FLASH_SAVE_ADDR,(u16*)databuf,databuf_len);//CH1дflash
				}
							
//*******************CH1��ƫ����
								else if(strstr((const char*)RxBuffer,(const char*)"\"Boat_Calibration\":1"))		//������У׼
									{		
											calibra_data1 = calibra_data1 + IOT_calibration_data;
										
											if(calibra_data1 >= IOT_calibration_VAL)			//�ж��Ƿ񳬹��趨��ƫ������ֵ
												{
													calibra_data1= IOT_calibration_VAL;		//ƫ������ֵ���Ϊ30%
												}
											else calibra_data1 = calibra_data1;
											
										databuf_len=sizeof databuf/sizeof (databuf[0]);//����databuf����
													
										for (j=0;j<databuf_len;j++)
										{
												databuf[j]=calibra_data1;
										}
												STMFLASH_Write(CH2_VAL_FLASH_SAVE_ADDR,(u16*)databuf,databuf_len);			
									}	
							else if(strstr((const char*)RxBuffer,(const char*)"\"Boat_Calibration\":2"))		//������У׼
								{											
									//	Clear_Buffer();		
										calibra_data1 = calibra_data1 - IOT_calibration_data;
									
										if(calibra_data1 >= IOT_calibration_VAL)			//�ж��Ƿ񳬹��趨��ƫ������ֵ
												{
													calibra_data1= 0;		//ƫ������ֵ���Ϊ0%
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
			static	u16 calibra_data2=0;	//���2ƫ����			
	int j; 
	 u32 databuf[]={0};
	 u8 databuf_len;
	calibra_data2  =	STMFLASH_ReadHalfWord(CH3_VAL_FLASH_SAVE_ADDR);				//ȡ�ڴ��е�calibra_dataֵ
	strx=strstr((const char*)RxBuffer,(const char*)"Boat_Calibration");//��������ص���Ϣ������+Boat_Calibration: ���������ɹ�	
	if(strx)
	{
			if(strstr((const char*)RxBuffer,(const char*)"\"Boat_Calibration\":0"))			    //�̶�ֵ5%У׼
				{													
						calibra_data2 = IOT_calibration_CH2_VAL;	//�̶�ֵ0%У׼
							databuf_len=sizeof databuf/sizeof (databuf[0]);	//�������ݴ洢���ֵĳ���
											
								for (j=0;j<databuf_len;j++)		//�����ݴ���buf��
								{
										databuf[j]=calibra_data2 ;
								}										

								STMFLASH_Write(CH3_VAL_FLASH_SAVE_ADDR,(u16*)databuf,databuf_len);//CH2дflash						
				}
						//////--------------------�ɱ�ƫ����------------------------------
							
//*******************CH2��ƫ����
						else if(strstr((const char*)RxBuffer,(const char*)"\"Boat_Calibration\":3"))		//������У׼
						{

								calibra_data2 = calibra_data2 + IOT_calibration_data;
								
									if(calibra_data2 >= IOT_calibration_VAL)			//�ж��Ƿ񳬹��趨��ƫ������ֵ
								{
									calibra_data2= IOT_calibration_VAL;		//ƫ������ֵ���Ϊ30%
								}
									else calibra_data2 = calibra_data2;
									
								databuf_len=sizeof databuf/sizeof (databuf[0]);
											
								for (j=0;j<databuf_len;j++)
								{
										databuf[j]=calibra_data2 ;
								}
										STMFLASH_Write(CH3_VAL_FLASH_SAVE_ADDR,(u16*)databuf,databuf_len);														
						}		
							else if(strstr((const char*)RxBuffer,(const char*)"\"Boat_Calibration\":4"))		//������У׼
							{
	
										calibra_data2 = calibra_data2 - IOT_calibration_data;
								
									if(calibra_data2 >= IOT_calibration_VAL)			//�ж��Ƿ񳬹��趨��ƫ������ֵ
								{
									calibra_data2= 0;		//ƫ������ֵ���Ϊ0%
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

