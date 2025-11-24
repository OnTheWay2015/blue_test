#include "stm32f10x.h"                  // Device header
#include "led.h"
#include "bsp_usart_debug.h"
#include "bsp_gpio_key.h"
#include "usart_com.h"
 

void test_usart_send()
{
//测试串口助手以HEX查看
		uint8_t array[] = {0x31,0x32,0x8F,0x66,0x99};
		USART_SendByte(USART1,0x1);
		//USART_SendByte(USART1,0x10);
		//USART_SendArray(USART1,array,sizeof(array));
}

// 定时器3初始化函数   系统接口 "stm32f10x_tim.h"
/*
计数、自动重装,预分频寄存器 是16位 

计数器 就是用来执行计数定时的一个寄存器，每来一个时钟，计数器加1, 累计变化的
预分频器 可以对计数器的时钟进行分频，让这个计数更加灵活, 设置固定值
自动重装寄存器 就是计数的目标值，就是想要计多少个时钟申请中断, 设置固定值

*/
//@arr 自动重装载值
//@psc 预分频系数
void TIM3_Init(uint16_t arr, uint16_t psc) {
    // 定义定时器时间基准初始化结构体
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    // 定义NVIC中断控制器初始化结构体
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 使能TIM3时钟 - 开启定时器3的时钟源
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    // 定时器基础配置
    TIM_TimeBaseInitStructure.TIM_Period = arr;          // 自动重装载值 - 决定定时周期
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;       // 预分频系数 - 对时钟进行分频
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // 时钟分频 - 不分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  // 计数模式 - 向上计数
    // 初始化TIM3定时器的时间基准参数
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
    
    // 使能定时器更新中断 - 允许定时器溢出时产生中断
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    
    // 配置NVIC中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           // 中断通道 - TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 抢占优先级 - 0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;       // 子优先级 - 3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          // 使能中断通道
    // 初始化NVIC中断控制器
    NVIC_Init(&NVIC_InitStructure);
    
    // 启动定时器 - 开始计数
    TIM_Cmd(TIM3, ENABLE);
}



/*
SystemInit() ==> SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; // 中断向量表重定位到内部FLASH

*/
// 定时器3中断服务函数
//在 Start\startup_stm32f10x_md.s 声明,及放入中断向量表
void TIM3_IRQHandler(void) {
    // 检查TIM3的更新中断标志位是否被置位
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
        // 清除TIM3的更新中断标志位，防止重复进入中断
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        
        // 读取GPIOC Pin13的当前输出状态并取反
        // 1 - 当前状态：如果当前为1则得到0，如果当前为0则得到1
        uint8_t res = 1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13);
        
        // 将取反后的值写入GPIOC Pin13，实现LED状态翻转
        GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)(res));
			
				
    }
}

void test_key()
{
	
			if(KEY_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN,KEY_HIGH_TRIGGER) == KEY_DOWN)
			{
					LED_TOGGLE(R_LED_GPIO_PORT,R_LED_GPIO_PIN);
					test_usart_send();
			}
			if(KEY_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN,KEY_HIGH_TRIGGER) == KEY_DOWN)
			{
					LED_TOGGLE(G_LED_GPIO_PORT,G_LED_GPIO_PIN);
			}
			if(KEY_Scan(KEY3_GPIO_PORT,KEY3_GPIO_PIN,KEY_HIGH_TRIGGER) == KEY_DOWN)
			{
					LED_TOGGLE(B_LED_GPIO_PORT,B_LED_GPIO_PIN);
			}
	
	
}

int main(void)
{
	//系统时钟初始化
	//SystemInit(); // ==> SetSysClock 设置频率,当前为 SYSCLK_FREQ_72MHz
	
	LED_GPIO_Config(); //LED初始化
	GPIO_WriteBit(GPIOC,GPIO_Pin_13, Bit_SET);  
	
	
    
	/*
定时时间 = (ARR + 1) × (PSC + 1) / 系统时钟频率
	
预分频器的值和实际的分频系数相差了 1

 
系统时钟 = 72MHz = 72,000,000 Hz
PSC = 7199 -> PSC + 1 = 7200
ARR = 4999 -> ARR + 1 = 5000
	
定时时间 = 5000 × 7200 / 72,000,000 = 36,000,000 / 72,000,000 = 0.5秒

为什么是4999而不是5000?
STM32定时器从0开始计数，当计数器达到自动重装载值时触发中断

设置ARR=4999意味着计数器从0数到4999（共5000个计数值），然后触发更新事件


参数含义
	预分频7200：将72MHz分频为10kHz（0.1ms周期）
	自动重装载值5000：计数5000次达到0.5秒
		
	
	*/
		// 预分频系数 7199，自动重装载值 4999
		// 定时器3初始化
    //TIM3_Init(4999, 7199); //0.5s
    //TIM3_Init(10000-1, 7199); //1s
	
		//DEBUG_USART_Init();
		
		KEY_GPIO_Config();
		
//保持程序不结束,程序结束后,中断收不到	
	while(1)
	{
		test_key();
	}
	return 0;
	 

}

 
 

