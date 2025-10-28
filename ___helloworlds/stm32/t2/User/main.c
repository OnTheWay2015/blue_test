#include "stm32f10x.h"                  // Device header
#include "led.h"
void Delay(__IO u32 nCount); 

int main(void)
{
	LED_Init(); //LED初始化

	
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


