#ifndef HX117_H
#define HX117_H

#include "stm32f10x.h"

#define HX117_GPIO		 GPIOB
#define HX117DOUT_PIN	 GPIO_Pin_6
#define HX117CLK_PIN	 GPIO_Pin_7


void Hx117Init(void);
void USART_Configuration(void);
uint32_t  Read_HX711(void);


#endif