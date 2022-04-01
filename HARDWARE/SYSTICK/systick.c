#include "systick.h"


#include "misc.h"
#include "usart.h"
#if 0
static __INLINE uint32_t SysTick_Config(uint32_t ticks)
{ 
	// 判断 tick 的值是否大于 2^24，如果大于，则不符合大于
  if (ticks > SysTick_LOAD_RELOAD_Msk)  return (1);            
	
	//初始化reload寄存器的值
                                                               
  SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;      
	//配置中断优先级，配置为15，默认为最低优先级
  NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  
	//初始化 counter的值位0
  SysTick->VAL   = 0;  
	//配置 systick 的时钟为 72M
	//使能中断
	//使能systick
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | 
                   SysTick_CTRL_TICKINT_Msk   | 
                   SysTick_CTRL_ENABLE_Msk;                    
  return (0);                                                  
}
#endif

/*实现微秒级的延时*/

void sysTick_Delay_us(uint32_t us)
{
	uint32_t i;
	SysTick_Config(72);//这是根据系统时钟频率
	for(i = 0; i < us; i++)
	{
		while( !(SysTick->CTRL) & 1<<16);//1和1&还是1，当这个位为1的时候表示计数完毕,跳出循环
	}

}

/*实现毫秒级的延时*/
void sysTick_Delay_ms(uint32_t ms)
{
	uint32_t i;
	SysTick_Config(72000);//72,ms就是要*1000
	for(i = 0; i < ms; i++)
	{
		while( !(SysTick->CTRL) & 1<<16);//1和1&还是1，当这个位为1的时候表示计数完毕,跳出循环
		
	}
}

//void SysTick_Handler(void)
//{
//	u1_printf("遥控器连接。。没有信号输出\r\n");
//}

//static __INLINE uint32_t SysTick_Config(uint32_t ticks)
//{ 
//	// 判断 tick 的值是否大于 2^24，如果大于，则不符合大于
//  if (ticks > SysTick_LOAD_RELOAD_Msk)  return (1);            
//	
//	//初始化reload寄存器的值
//                                                               
//  SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;      
//	//配置中断优先级，配置为15，默认为最低优先级
//  NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  
//	//初始化 counter的值位0
//  SysTick->VAL   = 0;  
//	//配置 systick 的时钟为 72M
//	//使能中断
//	//使能systick
//  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | 
//                   SysTick_CTRL_TICKINT_Msk   | 
//                   SysTick_CTRL_ENABLE_Msk;                    
//  return (0);                                                  
//}

//SysTick_Config(SystemCoreClock/72000);

//void SysTick_Handler(void)
//{

//}

