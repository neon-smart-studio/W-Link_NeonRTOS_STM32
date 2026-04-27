#ifndef DAC_PIN_STM32Lx_H
#define DAC_PIN_STM32Lx_H

#include "DAC_Pin_STM32.h"

const DAC_Channel_Def DAC_Channel_Def_Table[hwDAC_Channel_Index_MAX] = {
#if defined(DAC_BASE) || defined(DAC1_BASE)
    /* ===== DAC / DAC1 ===== */
    { hwGPIO_Pin_A4, hwDAC_Instance_1 },   // DAC_OUT1 / DAC1_OUT1

#if defined(DAC_CHANNEL_2)
    { hwGPIO_Pin_A5, hwDAC_Instance_1 },   // DAC_OUT2 / DAC1_OUT2
#endif
#endif
};

#endif // DAC_PIN_STM32Lx_H