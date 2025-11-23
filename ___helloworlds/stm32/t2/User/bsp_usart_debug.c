/**
  ******************************************************************************
  * @file       bsp_debug.c
  * @author     embedfire
  * @version     V1.0
  * @date        2024
  * @brief      调试串口函数接口
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

#include "bsp_usart_debug.h"

/**
  * @brief  初始化控制 DEBUG 串口 的IO
  * @param  无
  * @retval 无
  */
void DEBUG_USART_PinConfig(void)
{
    /* 定义一个 GPIO 结构体 */
    GPIO_InitTypeDef gpio_initstruct = {0};
   
    /* 开启 DEBUG 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(DEBUG_TX_GPIO_CLK_PORT,ENABLE);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为 推挽复用、设置GPIO速率为50MHz*/
    gpio_initstruct.GPIO_Pin    = DEBUG_TX_GPIO_PIN;
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_AF_PP;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(DEBUG_TX_GPIO_PORT,&gpio_initstruct);
   
}

/**
 * @brief  配置 DEBUG 串口模式
 * @param  无
 * @retval 无
 */
void DEBUG_USART_ModeConfig(void)
{
    /* 定义一个 USART 结构体 */
    USART_InitTypeDef usart_initstruct = {0};
   
    /* 开启 DEBUG 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    
    /* 配置串口的工作参数 */
    usart_initstruct.USART_BaudRate                 = 115200;
    usart_initstruct.USART_HardwareFlowControl      = USART_HardwareFlowControl_None;
    usart_initstruct.USART_Mode                     = USART_Mode_Tx;
    usart_initstruct.USART_Parity                   = USART_Parity_No;
    usart_initstruct.USART_StopBits                 = USART_StopBits_1;
    usart_initstruct.USART_WordLength               = USART_WordLength_8b;
    
    USART_Init(USART1,&usart_initstruct);
   
}

/**
  * @brief  DEBUG 串口初始化
  * @param  无
  * @retval 无
  */
void DEBUG_USART_Init(void)
{

    /* 配置 DEBUG 串口模式 */
    DEBUG_USART_ModeConfig();
    
    /* 对应的 GPIO 的配置 */
    DEBUG_USART_PinConfig();
    
    /* 使能串口 */
    USART_Cmd(USART1,ENABLE);

}


/*****************************END OF FILE***************************************/
