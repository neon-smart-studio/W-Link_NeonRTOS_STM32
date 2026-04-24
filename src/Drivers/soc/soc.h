
#ifndef SOC_H
#define SOC_H

#ifdef DEVICE_TM4C1294
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_emac.h"
#include "inc/hw_types.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/debug.h"
#include "driverlib/emac.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"
#include "driverlib/i2c.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/flash.h"
#endif //DEVICE_TM4C1294

#if defined (STM32F030x6) || defined (STM32F030x8) ||                           \
    defined (STM32F031x6) || defined (STM32F038xx) ||                           \
    defined (STM32F042x6) || defined (STM32F048xx) || defined (STM32F070x6) || \
    defined (STM32F051x8) || defined (STM32F058xx) ||                           \
    defined (STM32F071xB) || defined (STM32F072xB) || defined (STM32F078xx) || defined (STM32F070xB) || \
    defined (STM32F091xC) || defined (STM32F098xx) || defined (STM32F030xC)
#include "stm32f0xx.h"
#include "stm32f0xx_hal_gpio.h"
#include "stm32f0xx_hal_exti.h"
#include "stm32f0xx_hal_uart.h"
#include "stm32f0xx_hal_spi.h"
#include "stm32f0xx_hal_i2c.h"
#include "stm32f0xx_hal_dma.h"
#include "stm32f0xx_hal_rtc.h"
#include "stm32f0xx_hal_adc.h"
#include "stm32f0xx_hal_can.h"
#include "stm32f0xx_hal_dac.h"
#endif //STM32F0

#if defined (STM32F100xB) || defined (STM32F100xE) || defined (STM32F101x6) || \
    defined (STM32F101xB) || defined (STM32F101xE) || defined (STM32F101xG) || defined (STM32F102x6) || defined (STM32F102xB) || defined (STM32F103x6) || \
    defined (STM32F103xB) || defined (STM32F103xE) || defined (STM32F103xG) || defined (STM32F105xC) || defined (STM32F107xC)
#include "stm32f1xx.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_exti.h"
#include "stm32f1xx_hal_uart.h"
#include "stm32f1xx_hal_spi.h"
#include "stm32f1xx_hal_i2c.h"
#include "stm32f1xx_hal_dma.h"
#include "stm32f1xx_hal_rtc.h"
#include "stm32f1xx_hal_adc.h"
#include "stm32f1xx_hal_can.h"
#include "stm32f1xx_hal_dac.h"
#endif //STM32F1

#if defined (STM32F205xx) || defined (STM32F215xx) || defined (STM32F207xx) || defined (STM32F217xx)
#include "stm32f2xx.h"
#include "stm32f2xx_hal_gpio.h"
#include "stm32f2xx_hal_exti.h"
#include "stm32f2xx_hal_uart.h"
#include "stm32f2xx_hal_spi.h"
#include "stm32f2xx_hal_i2c.h"
#include "stm32f2xx_hal_dma.h"
#include "stm32f2xx_hal_rtc.h"
#include "stm32f2xx_hal_adc.h"
#include "stm32f2xx_hal_can.h"
#include "stm32f2xx_hal_dac.h"
#endif //STM32F2

#if defined (STM32F301x8) || defined (STM32F302x8) || defined (STM32F318xx) || \
    defined (STM32F302xC) || defined (STM32F303xC) || defined (STM32F358xx) || \
    defined (STM32F303x8) || defined (STM32F334x8) || defined (STM32F328xx) || \
    defined (STM32F302xE) || defined (STM32F303xE) || defined (STM32F398xx) || \
    defined (STM32F373xC) || defined (STM32F378xx)
#include "stm32f3xx.h"
#include "stm32f3xx_hal_gpio.h"
#include "stm32f3xx_hal_exti.h"
#include "stm32f3xx_hal_uart.h"
#include "stm32f3xx_hal_spi.h"
#include "stm32f3xx_hal_i2c.h"
#include "stm32f3xx_hal_dma.h"
#include "stm32f3xx_hal_rtc.h"
#include "stm32f3xx_hal_adc.h"
#include "stm32f3xx_hal_can.h"
#include "stm32f3xx_hal_dac.h"
#endif //STM32F3

#if defined (STM32F405xx) || defined (STM32F415xx) || defined (STM32F407xx) || defined (STM32F417xx) || \
    defined (STM32F427xx) || defined (STM32F437xx) || defined (STM32F429xx) || defined (STM32F439xx) || \
    defined (STM32F401xC) || defined (STM32F401xE) || defined (STM32F410Tx) || defined (STM32F410Cx) || \
    defined (STM32F410Rx) || defined (STM32F411xE) || defined (STM32F446xx) || defined (STM32F469xx) || \
    defined (STM32F479xx) || defined (STM32F412Cx) || defined (STM32F412Rx) || defined (STM32F412Vx) || \
    defined (STM32F412Zx) || defined (STM32F413xx) || defined (STM32F423xx)
#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_exti.h"
#include "stm32f4xx_hal_uart.h"
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal_i2c.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_rtc.h"
#include "stm32f4xx_hal_adc.h"
#include "stm32f4xx_hal_can.h"
#include "stm32f4xx_hal_dac.h"
#endif //STM32F4

#if defined (STM32F756xx) || defined (STM32F746xx) || defined (STM32F745xx) || defined (STM32F765xx) || \
    defined (STM32F767xx) || defined (STM32F769xx) || defined (STM32F777xx) || defined (STM32F779xx) || \
    defined (STM32F722xx) || defined (STM32F723xx) || defined (STM32F732xx) || defined (STM32F733xx) || \
    defined (STM32F730xx) || defined (STM32F750xx)
#include "stm32f7xx.h"
#include "stm32f7xx_hal_gpio.h"
#include "stm32f7xx_hal_exti.h"
#include "stm32f7xx_hal_uart.h"
#include "stm32f7xx_hal_spi.h"
#include "stm32f7xx_hal_i2c.h"
#include "stm32f7xx_hal_dma.h"
#include "stm32f7xx_hal_rtc.h"
#include "stm32f7xx_hal_adc.h"
#include "stm32f7xx_hal_can.h"
#include "stm32f7xx_hal_dac.h"
#endif //STM32F7

#if defined (STM32H743xx) || defined (STM32H753xx)  || defined (STM32H750xx) || defined (STM32H742xx)  || \
    defined (STM32H745xx) || defined (STM32H745xG)  || defined (STM32H755xx) || defined (STM32H747xx)  || defined (STM32H747xG) || defined (STM32H757xx) || \
    defined (STM32H7A3xx) || defined (STM32H7A3xxQ) || defined (STM32H7B3xx) || defined (STM32H7B3xxQ) || defined (STM32H7B0xx) || defined (STM32H7B0xxQ) || \
    defined (STM32H735xx) || defined (STM32H733xx)  || defined (STM32H730xx) || defined (STM32H730xxQ) || defined (STM32H725xx) || defined (STM32H723xx)
#include "stm32h7xx.h"
#include "stm32h7xx_hal_gpio.h"
#include "stm32h7xx_hal_exti.h"
#include "stm32h7xx_hal_uart.h"
#include "stm32h7xx_hal_spi.h"
#include "stm32h7xx_hal_i2c.h"
#include "stm32h7xx_hal_dma.h"
#include "stm32h7xx_hal_rtc.h"
#include "stm32h7xx_hal_adc.h"
#include "stm32h7xx_hal_can.h"
#include "stm32h7xx_hal_dac.h"
#endif //STM32H7

#if defined (STM32F0) || defined (STM32F1) || defined (STM32F2) || defined (STM32F3) || defined (STM32F4) || defined (STM32F7) || \
    defined (STM32L0) || defined (STM32L1) || defined (STM32L4) || defined (STM32L5) || \
    defined (STM32C0) || \
    defined (STM32G0) || defined (STM32G4) || \
    defined (STM32H5) || defined (STM32H7) || defined (STM32H7RS) || \
    defined (STM32U0) || defined (STM32U5) || \
    defined (STM32WBA) || defined (STM32WB) || defined (STM32WL)
#define DEVICE_STM32
#endif

#if defined (STM32F0) || defined (STM32F7) || defined (STM32F3) || defined (STM32G0) || \
    defined (STM32G4) || defined (STM32H7) || defined (STM32L0) || defined (STM32L4) || \
    defined (STM32L5) || defined (STM32U5) || defined (STM32WB) || defined (STM32WL)
#define STM32_I2C_NEW_ARCH
#endif
#if defined (STM32F1) || defined (STM32F2) || defined (STM32F4) || defined (STM32L1)
#define STM32_I2C_LEGACY_ARCH
#endif

#if defined (STM32F0) || defined (STM32F3) || defined (STM32G0) || \
    defined (STM32G4) || defined (STM32H7) || defined (STM32L0) || defined (STM32L4) || \
    defined (STM32L5) || defined (STM32U5) || defined (STM32WB) || defined (STM32WL)
#define STM32_DMA_NEW_ARCH
#endif
#if defined (STM32F1) || defined (STM32F2) || defined (STM32F4) || defined (STM32F7) || defined (STM32L1)
#define STM32_DMA_LEGACY_ARCH
#endif

#endif //SOC_H