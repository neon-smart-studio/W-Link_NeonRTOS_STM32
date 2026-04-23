
#ifndef GPIO_STM32_H
#define GPIO_STM32_H

#include "soc.h"

#include "GPIO/Pin/GPIO_Pin.h"

#include "GPIO/GPIO_Def.h"

//internal use
extern bool gpio_pin_init_status[hwGPIO_Pin_MAX];
void GPIO_Enable_RCC_Clock(GPIO_TypeDef * base);
void GPIO_Disable_RCC_Clock(GPIO_TypeDef * base);

#endif //GPIO_STM32_H