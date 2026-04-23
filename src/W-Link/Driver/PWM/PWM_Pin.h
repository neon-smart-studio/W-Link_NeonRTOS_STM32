
#ifndef PWM_PIN_H
#define PWM_PIN_H

#include "GPIO/GPIO.h"

#if defined (STM32F100xB) || defined (STM32F100xE) || defined (STM32F101x6)
#include "Pin/PWM_Pin_STM32F100xx.h"
#endif //STM32F100xx

#if defined (STM32F101xB) || defined (STM32F101xE) || defined (STM32F101xG) || defined (STM32F102x6) || defined (STM32F102xB) || defined (STM32F103x6)
#include "Pin/PWM_Pin_STM32F101xx.h"
#endif //STM32F101xx

#if defined (STM32F103xB) || defined (STM32F103xE) || defined (STM32F103xG) || defined (STM32F105xC) || defined (STM32F107xC)
#include "Pin/PWM_Pin_STM32F103xx.h"
#endif //STM32F103xx

#ifdef STM32F207xx
#include "Pin/PWM_Pin_STM32F207xx.h"
#endif // STM32F207xx

#ifdef STM32F767xx
#include "Pin/PWM_Pin_STM32F767xx.h"
#endif //STM32F767xx

#ifdef STM32H723xx
#include "Pin/PWM_Pin_STM32H723xx.h"
#endif //STM32H723xx

#ifdef DEVICE_TM4C1294
#include "Pin/PWM_Pin_TM4C1294.h"
#endif //DEVICE_TM4C1294

#endif //PWM_PIN_H