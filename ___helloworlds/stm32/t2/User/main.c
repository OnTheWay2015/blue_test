#include "stm32f10x.h"                  // Device header
#include "led.h"
void Delay(__IO u32 nCount); 

// 定时器3初始化函数   系统接口 "stm32f10x_tim.h"
void TIM3_Init(uint16_t arr, uint16_t psc) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 使能TIM3时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    // 定时器基础配置
    TIM_TimeBaseInitStructure.TIM_Period = arr;          // 自动重装载值
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;     // 预分频系数
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
    
    // 使能定时器更新中断
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    
    // 配置NVIC中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // 启动定时器
    TIM_Cmd(TIM3, ENABLE);
}

/*
SystemInit() ==> SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; // 中断向量表重定位到内部FLASH

*/
// 定时器3中断服务函数
void TIM3_IRQHandler(void) {
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
				uint8_t res  = 1 - GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_13);
        GPIO_WriteBit(GPIOC, GPIO_Pin_13, 
            (BitAction)(res));
    }
}



int main(void)
{
	//系统时钟初始化
	//SystemInit(); // ==> SetSysClock 设置频率,当前为 SYSCLK_FREQ_72MHz
	
	LED_Init(); //LED初始化
	GPIO_WriteBit(GPIOC,GPIO_Pin_13, Bit_SET);  
	
	// 定时器3初始化
    // 预分频系数7199，自动重装载值4999
    // 定时时间 = (7199+1)*(4999+1)/72000000 = 0.5秒
    TIM3_Init(4999, 7199);
    //TIM3_Init(49990, 7199);
	
	while(1)
	{
	}
	return 0;
	/*
	int x = 1;
	while(1)
	{
		Delay(1000000);//todo 怎样计算时间
		if (x==1){
			x = 0;
			GPIO_WriteBit(GPIOC,GPIO_Pin_13, Bit_RESET); //P13引脚输出低电平

		}else{
			x=1;
			GPIO_WriteBit(GPIOC,GPIO_Pin_13, Bit_SET);   //P13引脚输出高电平

		}



	} 
	*/

}


int main2(void)
{
	//库函数，专门用于控制APB2总线上的外设时钟开启或关闭
	//在STM32中，任何外设（包括GPIO）在使用前都必须先启用其时钟，否则无法正常工作
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOC, GPIO_Pin_13);
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	while (1)
	{
		
	}
}

void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}


