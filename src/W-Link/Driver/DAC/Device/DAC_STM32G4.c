#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "soc.h"
#include "NeonRTOS.h"
#include "GPIO/GPIO.h"
#include "DAC/DAC.h"

#ifdef STM32G4

#if defined(DAC1_BASE) || defined(DAC2_BASE) || defined(DAC3_BASE) || defined(DAC4_BASE)

#include "DAC/DAC_Pin.h"
#include "GPIO/Device/GPIO_STM32.h"

#define DAC_VREF_MV   3300.0f
#define DAC_MAX_COUNT 4095.0f

static bool DAC_Instance_Init_Status[hwDAC_Instance_MAX];
static bool DAC_Channel_Init_Status[hwDAC_Channel_Index_MAX];

static DAC_HandleTypeDef g_dac[hwDAC_Instance_MAX];

static uint32_t DAC_Channel_To_HAL(hwDAC_Channel_Index ch)
{
    hwDAC_Instance inst = DAC_Channel_Def_Table[ch].inst;

    switch (inst)
    {
#if defined(DAC1_BASE)
        case hwDAC_Instance_1:
            if (ch == hwDAC_Channel_Index_0) return DAC_CHANNEL_1;
            if (ch == hwDAC_Channel_Index_1) return DAC_CHANNEL_2;
            break;
#endif

#if defined(DAC2_BASE)
        case hwDAC_Instance_2:
            return DAC_CHANNEL_1;
#endif

#if defined(DAC3_BASE)
        case hwDAC_Instance_3:
            return DAC_CHANNEL_1;
#endif

#if defined(DAC4_BASE)
        case hwDAC_Instance_4:
            return DAC_CHANNEL_1;
#endif

        default:
            break;
    }

    return 0;
}

hwDAC_OpStatus hwDAC_Channel_Init(hwDAC_Channel_Index ch)
{
    if (ch >= hwDAC_Channel_Index_MAX)
    {
        return hwDAC_InvalidParameter;
    }

    hwDAC_Instance inst = DAC_Channel_Def_Table[ch].inst;
    if (inst >= hwDAC_Instance_MAX)
    {
        return hwDAC_InvalidParameter;
    }

    if (DAC_Channel_Init_Status[ch])
    {
        return hwDAC_OK;
    }

    GPIO_TypeDef *dac_pin_soc_base =
        GPIO_Map_Soc_Base(DAC_Channel_Def_Table[ch].dac_pin);

    uint16_t dac_soc_pin =
        GPIO_Map_Soc_Pin(DAC_Channel_Def_Table[ch].dac_pin);

    if (dac_soc_pin == 0 || dac_pin_soc_base == NULL)
    {
        return hwDAC_InvalidParameter;
    }

    GPIO_Enable_RCC_Clock(dac_pin_soc_base);

    GPIO_InitTypeDef g_dac_pin = {0};
    g_dac_pin.Pin  = dac_soc_pin;
    g_dac_pin.Mode = GPIO_MODE_ANALOG;
    g_dac_pin.Pull = GPIO_NOPULL;

    HAL_GPIO_Init(dac_pin_soc_base, &g_dac_pin);

    if (!DAC_Instance_Init_Status[inst])
    {
        switch (inst)
        {
#if defined(DAC1_BASE)
            case hwDAC_Instance_1:
                __HAL_RCC_DAC1_CLK_ENABLE();
                g_dac[inst].Instance = DAC1;
                break;
#endif

#if defined(DAC2_BASE)
            case hwDAC_Instance_2:
                __HAL_RCC_DAC2_CLK_ENABLE();
                g_dac[inst].Instance = DAC2;
                break;
#endif

#if defined(DAC3_BASE)
            case hwDAC_Instance_3:
                __HAL_RCC_DAC3_CLK_ENABLE();
                g_dac[inst].Instance = DAC3;
                break;
#endif

#if defined(DAC4_BASE)
            case hwDAC_Instance_4:
                __HAL_RCC_DAC4_CLK_ENABLE();
                g_dac[inst].Instance = DAC4;
                break;
#endif

            default:
                return hwDAC_InvalidParameter;
        }

        if (HAL_DAC_Init(&g_dac[inst]) != HAL_OK)
        {
            return hwDAC_HwError;
        }

        DAC_Instance_Init_Status[inst] = true;
    }

    uint32_t hal_ch = DAC_Channel_To_HAL(ch);
    if (hal_ch == 0)
    {
        return hwDAC_InvalidParameter;
    }

    DAC_ChannelConfTypeDef cfg = {0};
    cfg.DAC_Trigger      = DAC_TRIGGER_NONE;
    cfg.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

#if defined(DAC_CHIPCONNECT_DISABLE)
    cfg.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
#endif

#if defined(DAC_SAMPLEANDHOLD_DISABLE)
    cfg.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
#endif

#if defined(DAC_OUTPUTBUFFER_ENABLE)
    cfg.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
#endif

    if (HAL_DAC_ConfigChannel(&g_dac[inst], &cfg, hal_ch) != HAL_OK)
    {
        return hwDAC_HwError;
    }

    if (HAL_DAC_Start(&g_dac[inst], hal_ch) != HAL_OK)
    {
        return hwDAC_HwError;
    }

    gpio_pin_init_status[DAC_Channel_Def_Table[ch].dac_pin] = true;
    DAC_Channel_Init_Status[ch] = true;

    return hwDAC_OK;
}

hwDAC_OpStatus hwDAC_Channel_DeInit(hwDAC_Channel_Index ch)
{
    if (ch >= hwDAC_Channel_Index_MAX)
    {
        return hwDAC_InvalidParameter;
    }

    hwDAC_Instance inst = DAC_Channel_Def_Table[ch].inst;
    if (inst >= hwDAC_Instance_MAX)
    {
        return hwDAC_InvalidParameter;
    }

    if (!DAC_Channel_Init_Status[ch])
    {
        return hwDAC_OK;
    }

    GPIO_TypeDef *dac_pin_soc_base =
        GPIO_Map_Soc_Base(DAC_Channel_Def_Table[ch].dac_pin);

    uint16_t dac_soc_pin =
        GPIO_Map_Soc_Pin(DAC_Channel_Def_Table[ch].dac_pin);

    if (dac_soc_pin == 0 || dac_pin_soc_base == NULL)
    {
        return hwDAC_InvalidParameter;
    }

    uint32_t hal_ch = DAC_Channel_To_HAL(ch);
    if (hal_ch == 0)
    {
        return hwDAC_InvalidParameter;
    }

    HAL_DAC_Stop(&g_dac[inst], hal_ch);
    DAC_Channel_Init_Status[ch] = false;

    bool used = false;
    for (size_t i = 0; i < hwDAC_Channel_Index_MAX; i++)
    {
        if (DAC_Channel_Init_Status[i] &&
            DAC_Channel_Def_Table[i].inst == inst)
        {
            used = true;
            break;
        }
    }

    if (!used && DAC_Instance_Init_Status[inst])
    {
        HAL_DAC_DeInit(&g_dac[inst]);

        switch (inst)
        {
#if defined(DAC1_BASE)
            case hwDAC_Instance_1:
                __HAL_RCC_DAC1_CLK_DISABLE();
                break;
#endif

#if defined(DAC2_BASE)
            case hwDAC_Instance_2:
                __HAL_RCC_DAC2_CLK_DISABLE();
                break;
#endif

#if defined(DAC3_BASE)
            case hwDAC_Instance_3:
                __HAL_RCC_DAC3_CLK_DISABLE();
                break;
#endif

#if defined(DAC4_BASE)
            case hwDAC_Instance_4:
                __HAL_RCC_DAC4_CLK_DISABLE();
                break;
#endif

            default:
                break;
        }

        DAC_Instance_Init_Status[inst] = false;
    }

    HAL_GPIO_DeInit(dac_pin_soc_base, dac_soc_pin);
    gpio_pin_init_status[DAC_Channel_Def_Table[ch].dac_pin] = false;

    return hwDAC_OK;
}

hwDAC_OpStatus hwDAC_Write_MiniVolt(hwDAC_Channel_Index ch, float mv)
{
    if (ch >= hwDAC_Channel_Index_MAX)
    {
        return hwDAC_InvalidParameter;
    }

    if (!DAC_Channel_Init_Status[ch])
    {
        return hwDAC_NotInit;
    }

    if (mv < 0.0f)
    {
        mv = 0.0f;
    }

    if (mv > DAC_VREF_MV)
    {
        mv = DAC_VREF_MV;
    }

    uint32_t raw = (uint32_t)((mv / DAC_VREF_MV) * DAC_MAX_COUNT);
    hwDAC_Instance inst = DAC_Channel_Def_Table[ch].inst;

    uint32_t hal_ch = DAC_Channel_To_HAL(ch);
    if (hal_ch == 0)
    {
        return hwDAC_InvalidParameter;
    }

    if (HAL_DAC_SetValue(&g_dac[inst], hal_ch, DAC_ALIGN_12B_R, raw) != HAL_OK)
    {
        return hwDAC_HwError;
    }

    return hwDAC_OK;
}

#endif // defined(DAC1_BASE) || defined(DAC2_BASE) || defined(DAC3_BASE) || defined(DAC4_BASE)

#endif // STM32G4