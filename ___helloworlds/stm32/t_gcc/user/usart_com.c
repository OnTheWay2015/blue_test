/**
  ******************************************************************************
  * @file       usart_com.c
  * @author     embedfire
  * @version     V1.0
  * @date        2024
  * @brief      重定向C库printf函数到usart端口
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

#include "usart_com.h"

/**
  * @brief  发送一个字节函数
  * @param  pusartx：USARTx(x=1,2,3)/UARTx(x=4,5) 
  * @param  ch:要发送的数据
  * @note   无
  * @retval 无
  */
void USART_SendByte(USART_TypeDef *pusartx,uint8_t ch)
{
    /* 等待发送完成 */
	  FlagStatus r = USART_GetFlagStatus(pusartx,USART_FLAG_TC);
    while( 1 )
		{
			if (r == RESET)
			{
				break;
			}
			r = USART_GetFlagStatus(pusartx,USART_FLAG_TC);
		}
    
    /* 发送一个字节数据到 pusartx */
    USART_SendData(pusartx,ch);
    
    /* 等待发送数据寄存器为空 */
		r = USART_GetFlagStatus(pusartx,USART_FLAG_TXE);
    while( r != RESET)
		{
			r = USART_GetFlagStatus(pusartx,USART_FLAG_TXE);
		}
}

/**
  * @brief  发送8位的数组函数
  * @param  pusartx：USARTx(x=1,2,3)/UARTx(x=4,5) 
  * @param  array:要发送的数组
  * @param  num:数组大小.
  * @note   无
  * @retval 无
  */
void USART_SendArray(USART_TypeDef *pusartx,uint8_t *array,uint32_t num)
{
    /* 等待发送完成 */
    while(USART_GetFlagStatus(pusartx,USART_FLAG_TC) == RESET);
    
    for(uint32_t i = 0;i<num;i++)
    {
        /* 发送一个字节数据到 pusartx */
        USART_SendData(pusartx,array[i]);
        
        /* 等待发送数据寄存器为空 */
        while(USART_GetFlagStatus(pusartx,USART_FLAG_TXE) == RESET);
    }
}



/*****************************END OF FILE***************************************/
