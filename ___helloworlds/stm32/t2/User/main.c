#include "stm32f10x.h"                  // Device header
#include "led.h"
void Delay(__IO u32 nCount); 

int main(void)
{
	LED_Init(); //LED��ʼ��

	
	int x = 1;
	while(1)
	{
		Delay(1000000);//todo ��������ʱ��
		if (x==1){
			x = 0;
			GPIO_WriteBit(GPIOC,GPIO_Pin_13, Bit_RESET); //P13��������͵�ƽ

		}else{
			x=1;
			GPIO_WriteBit(GPIOC,GPIO_Pin_13, Bit_SET);   //P13��������ߵ�ƽ

		}



	}   
}
int main2(void)
{
	//�⺯����ר�����ڿ���APB2�����ϵ�����ʱ�ӿ�����ر�
	//��STM32�У��κ����裨����GPIO����ʹ��ǰ��������������ʱ�ӣ������޷���������
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

void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}


