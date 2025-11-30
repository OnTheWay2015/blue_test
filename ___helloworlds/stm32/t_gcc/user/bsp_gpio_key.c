/**
  ******************************************************************************
  * @file       bsp_gpio_key.c
  * @author     embedfire
  * @version     V1.0
  * @date        2024
  * @brief      扫描按键函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台  ：野火 STM32F103C8T6-STM32开发板 
  * 论坛      ：http://www.firebbs.cn
  * 官网      ：https://embedfire.com/
  * 淘宝      ：https://yehuosm.tmall.com/
  *
  ******************************************************************************
  */

#include "bsp_gpio_key.h"

/**
  * @brief  初始化控制 KEY 的IO
  * @param  无
  * @retval 无
  */
void KEY_GPIO_Config(void)
{
    /* 定义一个 GPIO 结构体 */
    GPIO_InitTypeDef gpio_initstruct = {0};
    
/**************************** 核心板载按键 *****************************/   
#if 1    
    
    /* 开启 KEY 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK_PORT,ENABLE);
    
    /* IO输出状态初始化控制 */
    GPIO_SetBits(KEY1_GPIO_PORT,KEY1_GPIO_PIN);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为 浮空输入、设置GPIO速率为50MHz*/
    gpio_initstruct.GPIO_Pin    = KEY1_GPIO_PIN;
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_IN_FLOATING;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(KEY1_GPIO_PORT,&gpio_initstruct);
   
#endif 
    
#if 1    
    
    /* 开启 KEY 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(KEY2_GPIO_CLK_PORT,ENABLE);
    
    /* IO输出状态初始化控制 */
    GPIO_SetBits(KEY2_GPIO_PORT,KEY2_GPIO_PIN);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为 浮空输入、设置GPIO速率为50MHz*/
    gpio_initstruct.GPIO_Pin    = KEY2_GPIO_PIN;
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_IN_FLOATING;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(KEY2_GPIO_PORT,&gpio_initstruct);
   
#endif 

/**************************** 用户自定义扩展按键 *****************************/

#if 1    
    
    /* 开启 KEY 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(KEY3_GPIO_CLK_PORT,ENABLE);
    
    /* IO输出状态初始化控制 */
    GPIO_SetBits(KEY3_GPIO_PORT,KEY3_GPIO_PIN);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为 下拉输入、设置GPIO速率为50MHz*/
    gpio_initstruct.GPIO_Pin    = KEY3_GPIO_PIN;
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_IPD;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(KEY3_GPIO_PORT,&gpio_initstruct);
   
#endif 

 
}

/**
  * @brief  粗略阻塞延时基本函数接口
  * @param  ncount：传入计数值
  * @note   软件延时函数，使用不同的系统时钟，延时不一样，还会存在函数调用以及其它计算损耗，只能粗略使用
  * @retval 无
  */
void Rough_Delay(__IO uint32_t ncount)
{
    for(uint32_t i = 0;i<ncount;i++)
    {
        __NOP();
    }
}

void Rough_Delay_Ms(__IO uint32_t time)
{
    Rough_Delay(0x3e8*7*time);
}

/**
  * @brief  基础检测按键
  * @param  GPIOx：x 可以是 A，B，C等
  * @param  GPIO_Pin：待操作的pin脚号
  * @param  key_pressstatus：按键按下时的IO电平状态
  * @retval KEY_UP(没有触发按键)、KEY_DOWN(触发按键)
  */
KEY_Status KEY_Scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, KEY_TriggerLevel key_pressstatus)
{
    if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == key_pressstatus)
    {
        Rough_Delay_Ms(20);
        while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == key_pressstatus);
        Rough_Delay_Ms(20);
        return KEY_DOWN;
    }
    else
    {
        return KEY_UP;
    }
}

/*****************************END OF FILE***************************************/
