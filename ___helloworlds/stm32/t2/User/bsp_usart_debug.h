#ifndef __BSP_DEBUG_H
#define __BSP_DEBUG_H

#include "stm32f10x.h"

#define DEBUG_TX_GPIO_PORT          GPIOA                           /* GPIO端口 */
#define DEBUG_TX_GPIO_CLK_PORT      RCC_APB2Periph_GPIOA            /* GPIO端口时钟 */
#define DEBUG_TX_GPIO_PIN           GPIO_Pin_9                      /* 对应PIN脚 */

void DEBUG_USART_PinConfig(void);
void DEBUG_USART_ModeConfig(void);
void DEBUG_USART_Init(void);

#endif  /* __BSP_DEBUG_H */
