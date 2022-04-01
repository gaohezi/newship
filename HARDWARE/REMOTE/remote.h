#ifndef __REMOTE_H
#define __REMOTE_H	
#include "sys.h"
#include <stdio.h>

#define flycolor_low   18100	 //��ӯ���ֵ��
#define flycolor_high  18900	 //��ӯ���ֵ��
//#define flycolor_stop    18500	 //��ӯ���ֵ��ֹͣ
#define flycolor_stop	(flycolor_low+flycolor_high)/2
//#define flycolor_ch2_stop	18510
//#define flycolor_ch3_stop	18480
#define flycolor_ch2_stop	18500
#define flycolor_ch3_stop	18500

#define XRotor_low     13000	 //������
#define XRotor_high    14000	 //������

#define TIM3CH3_collect_stop    1500	 //������

#define XRotor_zero    1500	 //�������ֵ

#define CH2_VAL_FLASH_SAVE_ADDR  0X08070000		//����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)
#define CH3_VAL_FLASH_SAVE_ADDR	 0X08070008		//����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)
					
						////ң����ͨ��2					����Сֵ��1000 1002		1457		1912 2000�����ֵ��
						////ң����ͨ��3�����ţ�	����Сֵ��1120 1171		1626  	2080 2120�����ֵ��
//#define Remote_Max_Val 2120  //ң�������ֵ
//#define Remote_Min_Val 1120  //ң������Сֵ
//#define Remote_Med_Val 1579  //ң�������ֵ
#define Remote_CH2_Max_Val 2005  //ң�������ֵ ���ϸ���5
#define Remote_CH2_Min_Val 995	 //ң������Сֵ	���¸���5
#define Remote_CH3_Max_Val 2125  //ң�������ֵ	���ϸ���5
#define Remote_CH3_Min_Val 1115	 //ң������Сֵ	���¸���5
#define Remote_ch2_Med_Val 1457  //ң�����м�ֵ
#define Remote_ch3_Med_Val 1629  //ң�����м�ֵ

#define Remote_CH2_Out_Max_Val 1912  //ң�������ֵ		��455
#define Remote_CH2_Out_Min_Val 1002	 //ң������Сֵ		��455
#define Remote_CH3_Out_Max_Val 2084  //ң�������ֵ		��455
#define Remote_CH3_Out_Min_Val 1174	 //ң������Сֵ		��455


   void Remote_contorl_status(void);	 
	 void IOT_contorl_status   (void);
	 
	 u16 TIM3CH2_collect(void);
	 u16 TIM3CH3_collect(void);
	 
	 u16 Remote_contorl_right(void);
 	 u16 Remote_contorl_left (void);

u16 IOT_contorl_left (u16 con_CH2,	u16 CH2_correct_data);
u16 IOT_contorl_right(u16 con_CH3,	u16 CH3_correct_data);

void ALIyun_contorl_calibration1(void);
void ALIyun_contorl_calibration2(void);

void iot_contorl(void);

void dective_contorl(void );
#endif 