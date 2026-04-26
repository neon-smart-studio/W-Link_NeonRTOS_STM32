
#ifndef UART_PIN_H
#define UART_PIN_H

#ifdef STM32C0
#include "Pin/UART_Pin_STM32C0.h"
#endif //STM32C0

#ifdef STM32F0
#include "Pin/UART_Pin_STM32F0.h"
#endif //STM32F0

#ifdef STM32F1
#include "Pin/UART_Pin_STM32F1.h"
#endif //STM32F1

#ifdef STM32F2
#include "Pin/UART_Pin_STM32F2.h"
#endif //STM32F2

#ifdef STM32F3
#include "Pin/UART_Pin_STM32F3.h"
#endif //STM32F3

#ifdef STM32F4
#include "Pin/UART_Pin_STM32F4.h"
#endif //STM32F4

#ifdef STM32F7
#include "Pin/UART_Pin_STM32F7.h"
#endif //STM32F7

#ifdef STM32G0
#include "Pin/UART_Pin_STM32G0.h"
#endif //STM32G0

#ifdef STM32H7
#include "Pin/UART_Pin_STM32H7.h"
#endif //STM32H7

#endif //UART_PIN_H