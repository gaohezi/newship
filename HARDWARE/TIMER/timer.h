#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
//pwm���
//void TIM2_Int_Init(u16 arr,u16 psc);
//void TIM2_PWM_Init(u16 arr,u16 psc);

//pwm�ɼ�
void TIM3_Cap_Init(u16 arr,u16 psc);
void TIM5_Cap_Init(u16 arr,u16 psc);

//pwm���
void TIM4_Int_Init(u16 arr,u16 psc);
void TIM4_PWM_Init(u16 arr,u16 psc);

//����������
void TIM2_Int_Init(u16 arr,u16 psc);
void stop (void);
//��������
void TIM6_Int_Init(u16 arr,u16 psc);


void NEW_IOT_Contorl_TIM2(void);

void TIM2_IRQHandler_void(void);
#endif