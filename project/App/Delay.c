#include "Delay.h"

/**
  * @name  		Delay_us
  * @describe	延时微秒
  * @param  	u32 us
  * @return 	void
  * @author 	liangziyue
  */
void Delay_us(u32 us) {
    u32 i;
    
    /* SYSTICK configuration */
    SYSTICK_ClockSourceConfig(SYSTICK_SRC_STCLK);       // 即默认选择了外部参考时钟
    SYSTICK_SetReloadValue(SystemCoreClock / 8 / 1000000); // (CK_SYS/8/1000000) = 1us 
    SYSTICK_IntConfig(DISABLE);                         // 不开启中断
 
    /* 打开SysTick计数器 */
    SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);
    SYSTICK_CounterCmd(SYSTICK_COUNTER_ENABLE);
 
    for( i = 0;i < us;i++ )
    {
        while( !( (SysTick->CTRL) & (1<<16) ) ); 
    }
 
    /* 关闭SysTick计数器 */
    SYSTICK_CounterCmd(SYSTICK_COUNTER_DISABLE);
    /* 复位SysTick计数器 */
    SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);
}

/**
  * @name  		Delay_ms	
  * @describe	延时毫秒
  * @param  	u32 ms
  * @return 	void
  * @author 	liangziyue
  */
void Delay_ms(u32 ms) {
    u32 i;
    
    /* SYSTICK configuration */
    SYSTICK_ClockSourceConfig(SYSTICK_SRC_STCLK);       // 即默认选择了外部参考时钟
    SYSTICK_SetReloadValue(SystemCoreClock / 8 / 1000); // (CK_SYS/8/1000) = 1ms 
    SYSTICK_IntConfig(DISABLE);                         // 不开启中断
 
    /* 打开SysTick计数器 */
    SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);
    SYSTICK_CounterCmd(SYSTICK_COUNTER_ENABLE);
    
    for( i = 0;i < ms;i++ )
    {
        while( !( (SysTick->CTRL) & (1<<16) ) ); 
    }
 
    /* 关闭SysTick计数器 */
    SYSTICK_CounterCmd(SYSTICK_COUNTER_DISABLE);
    /* 复位SysTick计数器 */
    SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);
}

/**
  * @name  		Delay_s	
  * @describe	延时秒
  * @param  	u32 s
  * @return 	void
  * @author 	liangziyue
  */
void Delay_s(u32 s) {
	while (s--)
	{
		Delay_ms(1000);
	}
}

