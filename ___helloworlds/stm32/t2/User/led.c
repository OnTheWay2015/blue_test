#include "stm32f10x.h"

#include "led.h"



void LED_Init(void)

{	


	/*
	
	怎样查看 RCC_APB2Periph_GPIOC 对应的具体引脚? 主要通过查阅?STM32参考手册 和 芯片数据手册 来实现。
 
主要查看途径
1. 查阅STM32参考手册?

	在手册的"存储器与总线架构"或"RCC寄存器"章节找到 APB2ENR 寄存器
	该寄存器的位4对应 IOPCEN（GPIOC时钟使能位）
	使能GPIOC时钟后，该端口的所有引脚（PC0-PC15）都可使用
2. 查看芯片数据手册?

	获取具体型号的数据手册（如STM32F103C8T6）
	查看"引脚定义"章节，找到标记为 PC13、PC14、PC15 等引脚
3. 通过库函数定义确认?
	在 stm32f10x_rcc.h 头文件中，RCC_APB2Periph_GPIOC 对应数值 0x00000010，这个数值在 APB2ENR 寄存器中对应位。

引脚对应关系说明
	RCC_APB2Periph_GPIOC  使能的是整个GPIOC端口 ，具体包括：

		PC0 到 PC15（16个引脚）
		具体哪些引脚可用取决于芯片封装
		在你的代码中使用了 GPIO_Pin_13，即对应 GPIOC13 引脚
实用建议
	快速定位方法 :直接搜索"STM32Fxxx引脚定义图"找到对应型号的引脚分布图，图中标有PCx的引脚就是GPIOC端口对应的具体引脚
	
	*/
	
	/*
		为什么要先打开时钟?
			任何一个外设使用前，必须首先使能其相应的时钟.
			STM32 的整个芯片系统是按照模块划分的，
			每个模块（GPIO、ADC、DAC、USART、TIM等）都挂载在不同的总线上（如 APB1、APB2、AHB 等）。
			为了节能和提升效率，STM32 默认关闭大多数外设的时钟
			
			外设模块的寄存器（如 GPIOx->CRL、ADC1->SR、DAC->CR 等）属于模块内部电路的一部分，如果该模块没有时钟供电：
				寄存器不可访问或访问无效
				配置内容无法写入或无响应
				写入后不生效
				有可能引发不可预测行为（如锁死）
	*/
/*
时钟的核心作用：

寄存器操作使能：没有时钟信号，对GPIOC寄存器的读写操作完全无效

功能同步基础：时钟为GPIO的输入检测、输出控制、模式切换提供同步时序

低功耗管理：STM32默认关闭所有外设时钟，只有使用时才单独开启以节省功耗

硬件工作流程：
	时钟源选择->HSE/HSI作为原始时钟输入
	时钟树传递->HSE/HSI → PLL → SYSCLK → AHB → APB2总线
	时钟门控使能->设置RCC_APB2ENR寄存器的bit4(IOPCEN位)为 1
	时钟信号分发->时钟信号通过APB2总线送达GPIOC端口的所有寄存器
	寄存器可操作->此时才能正常配置GPIOC的引脚模式、读写数据寄存器

硬件层面的具体变化：
	GPIOC端口的配置寄存器(GPIOx_CRL/CRH)可正常写入
	输出数据寄存器(GPIOx_ODR)能够控制引脚电平
	输入数据寄存器(GPIOx_IDR)能够读取引脚状态
	复用功能寄存器(GPIOx_AFRL/AFRH)可配置引脚复用功能
建议：在调试STM32外设时，如果发现配置无效，首先检查对应的时钟是否已正确使能，这是排查硬件问题的重要步骤。
*/
	
	//1.打开控制GPIOC的时钟(APB2) 这个时钟对应的是所有 GPIOC 引脚, 系统接口 "stm32f10x_rcc.h"
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	

	//2.配置结构体，向对应的外设模块寄存器写入数据

	GPIO_InitTypeDef led_init;

	led_init.GPIO_Pin   = GPIO_Pin_13;      //GPIOC13引脚

	led_init.GPIO_Mode  = GPIO_Mode_Out_PP; //推挽输出	

	led_init.GPIO_Speed = GPIO_Speed_10MHz; //10MHz

	

	//3.对成员进行初始化  系统接口 "stm32f10x_gpio.h"
	GPIO_Init(GPIOC, &led_init);

}

