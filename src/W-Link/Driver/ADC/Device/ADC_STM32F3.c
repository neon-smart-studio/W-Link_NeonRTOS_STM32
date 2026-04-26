#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "soc.h"

#include "NeonRTOS.h"

#include "GPIO/GPIO.h"

#include "ADC/ADC.h"

#ifdef STM32F3

#include "ADC/ADC_Pin.h"

#include "GPIO/Device/GPIO_STM32.h"

#define ADC_IRQ_NVIC_PRIORITY 5
#define ADC_IRQ_NVIC_SUB_PRIORITY 0

#define ADC_VREF_MV     3300.0f
#define ADC_MAX_COUNT   4095.0f

#define ADC_CONV_TIMEOUT_MS 100

typedef struct {
    uint32_t raw;
} ADC_QueueItem;

static bool ADC_NVIC_Init_Status = false;
static bool ADC_Instance_Init_Status[hwADC_Instance_MAX] = {false};
static bool ADC_Channel_Init_Status[hwADC_Channel_Index_MAX] = {false};
static NeonRTOS_MsgQ_t ADC_Channel_SyncQueue[hwADC_Instance_MAX] = {NULL};

static ADC_HandleTypeDef g_adc[hwADC_Instance_MAX];

static uint32_t ADC_Channel_To_HAL(hwADC_Channel_Index ch)
{
    switch (ch) {
        case hwADC_Channel_Index_0:  return ADC_CHANNEL_1;
        case hwADC_Channel_Index_1:  return ADC_CHANNEL_2;
        case hwADC_Channel_Index_2:  return ADC_CHANNEL_3;
        case hwADC_Channel_Index_3:  return ADC_CHANNEL_4;
        case hwADC_Channel_Index_4:  return ADC_CHANNEL_5;
        case hwADC_Channel_Index_5:  return ADC_CHANNEL_6;
        case hwADC_Channel_Index_6:  return ADC_CHANNEL_7;
        case hwADC_Channel_Index_7:  return ADC_CHANNEL_8;
        case hwADC_Channel_Index_8:  return ADC_CHANNEL_9;
        case hwADC_Channel_Index_9:  return ADC_CHANNEL_10;
        case hwADC_Channel_Index_10: return ADC_CHANNEL_11;
        case hwADC_Channel_Index_11: return ADC_CHANNEL_12;
        case hwADC_Channel_Index_12: return ADC_CHANNEL_13;
        case hwADC_Channel_Index_13: return ADC_CHANNEL_14;
        case hwADC_Channel_Index_14: return ADC_CHANNEL_15;
        case hwADC_Channel_Index_15: return ADC_CHANNEL_16;
        case hwADC_Channel_Index_16: return ADC_CHANNEL_17;
        case hwADC_Channel_Index_17: return ADC_CHANNEL_18;
        default: return 0;
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    ADC_QueueItem item;

    for (hwADC_Instance inst = 0; inst < hwADC_Instance_MAX; inst++)
    {
        if (hadc == &g_adc[inst])
        {
            item.raw = HAL_ADC_GetValue(hadc);
            NeonRTOS_MsgQWrite(&ADC_Channel_SyncQueue[inst], &item, NEONRT_NO_WAIT);
            break;
        }
    }
}

#if defined(ADC1_BASE) || defined(ADC2_BASE)
void ADC1_2_IRQHandler(void)
{
#if defined(ADC1_BASE)
    HAL_ADC_IRQHandler(&g_adc[hwADC_Instance_1]);
#endif

#if defined(ADC2_BASE)
    HAL_ADC_IRQHandler(&g_adc[hwADC_Instance_2]);
#endif
}
#endif

#if defined(ADC3_BASE)
void ADC3_IRQHandler(void)
{
    HAL_ADC_IRQHandler(&g_adc[hwADC_Instance_3]);
}
#endif

hwADC_OpStatus hwADC_Channel_Init(hwADC_Channel_Index ch)
{
    if (ch >= hwADC_Channel_Index_MAX)
    {
        return hwADC_InvalidParameter;
    }

    hwADC_Instance inst = ADC_Channel_Def_Table[ch].inst;

    if (inst >= hwADC_Instance_MAX)
    {
        return hwADC_InvalidParameter;
    }

    GPIO_TypeDef *adc_pin_soc_base = GPIO_Map_Soc_Base(ADC_Channel_Def_Table[ch].adc_pin);
    uint16_t adc_soc_pin = GPIO_Map_Soc_Pin(ADC_Channel_Def_Table[ch].adc_pin);

    if (adc_soc_pin == 0 || adc_pin_soc_base == NULL)
    {
        return hwADC_InvalidParameter;
    }

    if (!ADC_Instance_Init_Status[inst])
    {
        if (NeonRTOS_MsgQCreate(&ADC_Channel_SyncQueue[inst], "adc", sizeof(ADC_QueueItem), 1) != NeonRTOS_OK)
        {
            return hwADC_MemoryError;
        }
    }

    GPIO_Enable_RCC_Clock(adc_pin_soc_base);

    GPIO_InitTypeDef g_adc_pin = {0};
    g_adc_pin.Pin  = adc_soc_pin;
    g_adc_pin.Mode = GPIO_MODE_ANALOG;
    g_adc_pin.Pull = GPIO_NOPULL;

    HAL_GPIO_Init(adc_pin_soc_base, &g_adc_pin);

    if (!ADC_Instance_Init_Status[inst])
    {
        switch (inst)
        {
#if defined(ADC1_BASE)
            case hwADC_Instance_1:
                __HAL_RCC_ADC12_CLK_ENABLE();
                g_adc[inst].Instance = ADC1;
                break;
#endif

#if defined(ADC2_BASE)
            case hwADC_Instance_2:
                __HAL_RCC_ADC12_CLK_ENABLE();
                g_adc[inst].Instance = ADC2;
                break;
#endif

#if defined(ADC3_BASE)
            case hwADC_Instance_3:
                __HAL_RCC_ADC34_CLK_ENABLE();
                g_adc[inst].Instance = ADC3;
                break;
#endif

            default:
                return hwADC_InvalidParameter;
        }

        g_adc[inst].Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;
        g_adc[inst].Init.Resolution            = ADC_RESOLUTION_12B;
        g_adc[inst].Init.DataAlign             = ADC_DATAALIGN_RIGHT;
        g_adc[inst].Init.ScanConvMode          = ADC_SCAN_DISABLE;
        g_adc[inst].Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
        g_adc[inst].Init.LowPowerAutoWait      = DISABLE;
        g_adc[inst].Init.ContinuousConvMode    = DISABLE;
        g_adc[inst].Init.NbrOfConversion       = 1;
        g_adc[inst].Init.DiscontinuousConvMode = DISABLE;
        g_adc[inst].Init.ExternalTrigConv      = ADC_SOFTWARE_START;
        g_adc[inst].Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
        g_adc[inst].Init.DMAContinuousRequests = DISABLE;
        g_adc[inst].Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;

        if (HAL_ADC_Init(&g_adc[inst]) != HAL_OK)
        {
            ADC_Instance_Disable(inst);
            NeonRTOS_MsgQDelete(&ADC_Channel_SyncQueue[inst]);
            return hwADC_HwError;
        }

#if defined(ADC_SINGLE_ENDED)
        if (HAL_ADCEx_Calibration_Start(&g_adc[inst], ADC_SINGLE_ENDED) != HAL_OK)
#else
        if (HAL_ADCEx_Calibration_Start(&g_adc[inst]) != HAL_OK)
#endif
        {
            HAL_ADC_DeInit(&g_adc[inst]);
            ADC_Instance_Disable(inst);
            NeonRTOS_MsgQDelete(&ADC_Channel_SyncQueue[inst]);
            return hwADC_HwError;
        }

        if (!ADC_NVIC_Init_Status)
        {
#if defined(ADC1_BASE) || defined(ADC2_BASE)
                HAL_NVIC_SetPriority(ADC1_2_IRQn, ADC_IRQ_NVIC_PRIORITY, ADC_IRQ_NVIC_SUB_PRIORITY);
                HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
#endif

#if defined(ADC3_BASE)
                HAL_NVIC_SetPriority(ADC3_IRQn, ADC_IRQ_NVIC_PRIORITY, ADC_IRQ_NVIC_SUB_PRIORITY);
                HAL_NVIC_EnableIRQ(ADC3_IRQn);
#endif

                ADC_NVIC_Init_Status = true;
        }

        ADC_Instance_Init_Status[inst] = true;
    }

    gpio_pin_init_status[ADC_Channel_Def_Table[ch].adc_pin] = true;
    ADC_Channel_Init_Status[ch] = true;

    return hwADC_OK;
}

hwADC_OpStatus hwADC_Channel_DeInit(hwADC_Channel_Index ch)
{
    if (ch >= hwADC_Channel_Index_MAX)
    {
        return hwADC_InvalidParameter;
    }

    hwADC_Instance inst = ADC_Channel_Def_Table[ch].inst;

    if (inst >= hwADC_Instance_MAX)
    {
        return hwADC_InvalidParameter;
    }

    if (!ADC_Channel_Init_Status[ch])
    {
        return hwADC_OK;
    }

    GPIO_TypeDef *adc_pin_soc_base = GPIO_Map_Soc_Base(ADC_Channel_Def_Table[ch].adc_pin);
    uint16_t adc_soc_pin = GPIO_Map_Soc_Pin(ADC_Channel_Def_Table[ch].adc_pin);

    if (adc_soc_pin == 0 || adc_pin_soc_base == NULL)
    {
        return hwADC_InvalidParameter;
    }

    ADC_Channel_Init_Status[ch] = false;

    bool inst_used = false;

    for (hwADC_Instance i_inst = 0; i_inst < hwADC_Instance_MAX; i_inst++)
    {
        bool ch_used = false;

        for (size_t i = 0; i < hwADC_Channel_Index_MAX; i++)
        {
            if (ADC_Channel_Init_Status[i] &&
                ADC_Channel_Def_Table[i].inst == i_inst)
            {
                ch_used = true;
                break;
            }
        }

        if (!ch_used && ADC_Instance_Init_Status[i_inst])
        {
            HAL_ADC_DeInit(&g_adc[i_inst]);

            ADC_Instance_Init_Status[i_inst] = false;
            
            switch (inst)
            {
#if defined(ADC1_BASE)
                case hwADC_Instance_1:
                    if (!ADC_Instance_Init_Status[hwADC_Instance_1])
                    {
                        __HAL_RCC_ADC12_CLK_DISABLE();
                    }
                    break;
#endif

#if defined(ADC2_BASE)
                case hwADC_Instance_2:
                    if (!ADC_Instance_Init_Status[hwADC_Instance_2])
                    {
                        __HAL_RCC_ADC12_CLK_DISABLE();
                    }
                    break;
#endif

#if defined(ADC3_BASE)
                case hwADC_Instance_3:
                    __HAL_RCC_ADC34_CLK_DISABLE();
                    break;
#endif

                default:
                    break;
            }

            NeonRTOS_MsgQDelete(&ADC_Channel_SyncQueue[i_inst]);
            ADC_Channel_SyncQueue[i_inst] = NULL;
        }

        if (ADC_Instance_Init_Status[i_inst])
        {
            inst_used = true;
        }
    }

    if (!inst_used && ADC_NVIC_Init_Status)
    {
#if defined(ADC1_BASE) || defined(ADC2_BASE)
        HAL_NVIC_DisableIRQ(ADC1_2_IRQn);
 #endif

#if defined(ADC3_BASE)
        HAL_NVIC_DisableIRQ(ADC3_IRQn);
#endif

        ADC_NVIC_Init_Status = false;
    }

    HAL_GPIO_DeInit(adc_pin_soc_base, adc_soc_pin);

    gpio_pin_init_status[ADC_Channel_Def_Table[ch].adc_pin] = false;

    return hwADC_OK;
}

hwADC_OpStatus hwADC_Read_MiniVolt(hwADC_Channel_Index ch, float *readMv)
{
    if (ch >= hwADC_Channel_Index_MAX)
    {
        return hwADC_InvalidParameter;
    }

    if (!readMv)
    {
        return hwADC_InvalidParameter;
    }

    hwADC_Instance inst = ADC_Channel_Def_Table[ch].inst;

    if (inst >= hwADC_Instance_MAX)
    {
        return hwADC_InvalidParameter;
    }

    if (!ADC_Channel_Init_Status[ch])
    {
        return hwADC_NotInit;
    }

    ADC_HandleTypeDef *hadc = &g_adc[inst];

    ADC_ChannelConfTypeDef cfg = {0};
    cfg.Channel = ADC_Channel_To_HAL(ch);

#if defined(ADC_REGULAR_RANK_1)
    cfg.Rank = ADC_REGULAR_RANK_1;
#else
    cfg.Rank = 1;
#endif

    cfg.SamplingTime = ADC_SAMPLETIME_601CYCLES_5;

#if defined(ADC_SINGLE_ENDED)
    cfg.SingleDiff = ADC_SINGLE_ENDED;
#endif

#if defined(ADC_OFFSET_NONE)
    cfg.OffsetNumber = ADC_OFFSET_NONE;
#endif

    cfg.Offset = 0;

    if (HAL_ADC_ConfigChannel(hadc, &cfg) != HAL_OK)
    {
        return hwADC_HwError;
    }

    if (HAL_ADC_Start_IT(hadc) != HAL_OK)
    {
        return hwADC_HwError;
    }

    ADC_QueueItem item;

    if (NeonRTOS_MsgQRead(&ADC_Channel_SyncQueue[inst], &item, ADC_CONV_TIMEOUT_MS) != NeonRTOS_OK)
    {
        HAL_ADC_Stop_IT(hadc);
        return hwADC_HW_Error;
    }

    HAL_ADC_Stop_IT(hadc);

    *readMv = (item.raw * ADC_VREF_MV) / ADC_MAX_COUNT;

    return hwADC_OK;
}

#endif // STM32F3