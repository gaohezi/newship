#ifndef __REMOTE_H
#define __REMOTE_H	
#include "sys.h"
#include <stdio.h>

#define flycolor_low   18100	 //飞盈佳乐电调
#define flycolor_high  18900	 //飞盈佳乐电调
//#define flycolor_stop    18500	 //飞盈佳乐电调停止
#define flycolor_stop	(flycolor_low+flycolor_high)/2
//#define flycolor_ch2_stop	18510
//#define flycolor_ch3_stop	18480
#define flycolor_ch2_stop	18500
#define flycolor_ch3_stop	18500

#define XRotor_low     13000	 //乐天电调
#define XRotor_high    14000	 //乐天电调

#define TIM3CH3_collect_stop    1500	 //乐天电调

#define XRotor_zero    1500	 //乐天归零值

#define CH2_VAL_FLASH_SAVE_ADDR  0X08070000		//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)
#define CH3_VAL_FLASH_SAVE_ADDR	 0X08070008		//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)
					
						////遥控器通道2					（最小值）1000 1002		1457		1912 2000（最大值）
						////遥控器通道3（油门）	（最小值）1120 1171		1626  	2080 2120（最大值）
//#define Remote_Max_Val 2120  //遥控器最大值
//#define Remote_Min_Val 1120  //遥控器最小值
//#define Remote_Med_Val 1579  //遥控器最大值
#define Remote_CH2_Max_Val 2005  //遥控器最大值 向上浮动5
#define Remote_CH2_Min_Val 995	 //遥控器最小值	向下浮动5
#define Remote_CH3_Max_Val 2125  //遥控器最大值	向上浮动5
#define Remote_CH3_Min_Val 1115	 //遥控器最小值	向下浮动5
#define Remote_ch2_Med_Val 1457  //遥控器中间值
#define Remote_ch3_Med_Val 1629  //遥控器中间值

#define Remote_CH2_Out_Max_Val 1912  //遥控器最大值		差455
#define Remote_CH2_Out_Min_Val 1002	 //遥控器最小值		差455
#define Remote_CH3_Out_Max_Val 2084  //遥控器最大值		差455
#define Remote_CH3_Out_Min_Val 1174	 //遥控器最小值		差455


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
