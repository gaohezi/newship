#include "systick.h"


#include "misc.h"
#include "usart.h"
#if 0
static __INLINE uint32_t SysTick_Config(uint32_t ticks)
{ 
	// �ж� tick ��ֵ�Ƿ���� 2^24��������ڣ��򲻷��ϴ���
  if (ticks > SysTick_LOAD_RELOAD_Msk)  return (1);            
	
	//��ʼ��reload�Ĵ�����ֵ
                                                               
  SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;      
	//�����ж����ȼ�������Ϊ15��Ĭ��Ϊ������ȼ�
  NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  
	//��ʼ�� counter��ֵλ0
  SysTick->VAL   = 0;  
	//���� systick ��ʱ��Ϊ 72M
	//ʹ���ж�
	//ʹ��systick
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | 
                   SysTick_CTRL_TICKINT_Msk   | 
                   SysTick_CTRL_ENABLE_Msk;                    
  return (0);                                                  
}
#endif

/*ʵ��΢�뼶����ʱ*/

void sysTick_Delay_us(uint32_t us)
{
	uint32_t i;
	SysTick_Config(72);//���Ǹ���ϵͳʱ��Ƶ��
	for(i = 0; i < us; i++)
	{
		while( !(SysTick->CTRL) & 1<<16);//1��1&����1�������λΪ1��ʱ���ʾ�������,����ѭ��
	}

}

/*ʵ�ֺ��뼶����ʱ*/
void sysTick_Delay_ms(uint32_t ms)
{
	uint32_t i;
	SysTick_Config(72000);//72,ms����Ҫ*1000
	for(i = 0; i < ms; i++)
	{
		while( !(SysTick->CTRL) & 1<<16);//1��1&����1�������λΪ1��ʱ���ʾ�������,����ѭ��
		
	}
}

//void SysTick_Handler(void)
//{
//	u1_printf("ң�������ӡ���û���ź����\r\n");
//}

//static __INLINE uint32_t SysTick_Config(uint32_t ticks)
//{ 
//	// �ж� tick ��ֵ�Ƿ���� 2^24��������ڣ��򲻷��ϴ���
//  if (ticks > SysTick_LOAD_RELOAD_Msk)  return (1);            
//	
//	//��ʼ��reload�Ĵ�����ֵ
//                                                               
//  SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;      
//	//�����ж����ȼ�������Ϊ15��Ĭ��Ϊ������ȼ�
//  NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  
//	//��ʼ�� counter��ֵλ0
//  SysTick->VAL   = 0;  
//	//���� systick ��ʱ��Ϊ 72M
//	//ʹ���ж�
//	//ʹ��systick
//  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | 
//                   SysTick_CTRL_TICKINT_Msk   | 
//                   SysTick_CTRL_ENABLE_Msk;                    
//  return (0);                                                  
//}

//SysTick_Config(SystemCoreClock/72000);

//void SysTick_Handler(void)
//{

//}
