#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

#include "soc.h"
#include "NeonRTOS.h"
#include "Timer/Timer.h"

#ifdef STM32G0

#define TIMER_MUTEX_ACCESS_TIMEOUT     500

#define TIMER_IRQ_NVIC_PRIORITY        5
#define TIMER_IRQ_NVIC_SUB_PRIORITY    0

static bool Timer_Init_Status[hwTimer_Index_MAX] = {false};
static bool Timer_IsPeriodic[hwTimer_Index_MAX] = {false};
static onTimerEventHandler Timer_Expired_Handler[hwTimer_Index_MAX] = {NULL};

TIM_HandleTypeDef g_timer[hwTimer_Index_MAX];

static hwTimer_Index Timer_IndexFromHandle(TIM_HandleTypeDef *htimer)
{
    for (int i = 0; i < hwTimer_Index_MAX; i++) {
        if (&g_timer[i] == htimer) {
            return (hwTimer_Index)i;
        }
    }
    return hwTimer_Index_MAX;
}

TIM_TypeDef *Timer_Map_Soc_Base(hwTimer_Index index)
{
    switch (index)
    {
#if defined(TIM1_BASE)
        case hwTimer_Index_0: return TIM1;
#endif
#if defined(TIM2_BASE)
        case hwTimer_Index_1: return TIM2;
#endif
#if defined(TIM3_BASE)
        case hwTimer_Index_2: return TIM3;
#endif
#if defined(TIM14_BASE)
        case hwTimer_Index_13: return TIM14;
#endif
#if defined(TIM16_BASE)
        case hwTimer_Index_15: return TIM16;
#endif
#if defined(TIM17_BASE)
        case hwTimer_Index_16: return TIM17;
#endif
        default: break;
    }
    return NULL;
}

static uint32_t Timer_GetInputClock(hwTimer_Index index)
{
#if defined(TIM1_BASE)
    if (index == hwTimer_Index_0)
    {
        return HAL_RCC_GetPCLK2Freq();
    }
#endif

    return HAL_RCC_GetPCLK1Freq();
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    hwTimer_Index idx = Timer_IndexFromHandle(htim);
    if (idx >= hwTimer_Index_MAX) {
        return;
    }

    if (Timer_Expired_Handler[idx] != NULL)
    {
        Timer_Expired_Handler[idx](idx);
    }

    if (!Timer_IsPeriodic[idx])
    {
        HAL_TIM_Base_Stop_IT(htim);
    }
}

static void Timer_HAL_IRQHandler(hwTimer_Index index)
{
    HAL_TIM_IRQHandler(&g_timer[index]);
}

#if defined(TIM1_BASE)
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
    Timer_HAL_IRQHandler(hwTimer_Index_0);
}
#endif

#if defined(TIM2_BASE)
void TIM2_IRQHandler(void)
{
    Timer_HAL_IRQHandler(hwTimer_Index_1);
}
#endif

#if defined (STM32G0B0xx) || defined (STM32G0B1xx) || defined (STM32G0C1xx)
#if defined(TIM3_BASE) || defined(TIM4_BASE)
void TIM3_TIM4_IRQHandler(void)
{
#if defined(TIM3_BASE)
    Timer_HAL_IRQHandler(hwTimer_Index_2);
#endif
#if defined(TIM7_BASE)
    Timer_HAL_IRQHandler(hwTimer_Index_3);
#endif
}
#endif
#else
#if defined(TIM3_BASE)
void TIM3_IRQHandler(void)
{
    Timer_HAL_IRQHandler(hwTimer_Index_2);
}
#endif
#endif

#if defined(TIM14_BASE)
void TIM14_IRQHandler(void)
{
    Timer_HAL_IRQHandler(hwTimer_Index_13);
}
#endif

#if defined (STM32G0B0xx) || defined (STM32G0B1xx) || defined (STM32G0C1xx)
#if defined(TIM16_BASE)
void TIM16_FDCAN_IT0_IRQHandler(void)
{
    Timer_HAL_IRQHandler(hwTimer_Index_15);
}
#endif
#if defined(TIM17_BASE)
void TIM17_FDCAN_IT1_IRQHandler(void)
{
    Timer_HAL_IRQHandler(hwTimer_Index_16);
}
#endif
#else
#if defined(TIM16_BASE)
void TIM16_IRQHandler(void)
{
    Timer_HAL_IRQHandler(hwTimer_Index_15);
}
#endif
#if defined(TIM17_BASE)
void TIM17_IRQHandler(void)
{
    Timer_HAL_IRQHandler(hwTimer_Index_16);
}
#endif
#endif

hwTimer_OpResult Timer_Init(hwTimer_Index index)
{
    if (index >= hwTimer_Index_MAX)
    {
        return hwTimer_InvalidParameter;
    }

    if (Timer_Init_Status[index])
    {
        return hwTimer_OK;
    }

    TIM_TypeDef *timer_soc_base = Timer_Map_Soc_Base(index);
    if (timer_soc_base == NULL)
    {
        return hwTimer_InvalidParameter;
    }

    switch (index)
    {
#if defined(TIM1_BASE)
        case hwTimer_Index_0:
            __HAL_RCC_TIM1_CLK_ENABLE();
            break;
#endif
#if defined(TIM2_BASE)
        case hwTimer_Index_1:
            __HAL_RCC_TIM2_CLK_ENABLE();
            break;
#endif
#if defined(TIM3_BASE)
        case hwTimer_Index_2:
            __HAL_RCC_TIM3_CLK_ENABLE();
            break;
#endif
#if defined(TIM14_BASE)
        case hwTimer_Index_13:
            __HAL_RCC_TIM14_CLK_ENABLE();
            break;
#endif
#if defined(TIM16_BASE)
        case hwTimer_Index_15:
            __HAL_RCC_TIM16_CLK_ENABLE();
            break;
#endif
#if defined(TIM17_BASE)
        case hwTimer_Index_16:
            __HAL_RCC_TIM17_CLK_ENABLE();
            break;
#endif
        default:
            return hwTimer_InvalidParameter;
    }

    switch (index)
    {
#if defined(TIM1_BASE)
        case hwTimer_Index_0:
            HAL_NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, TIMER_IRQ_NVIC_PRIORITY, TIMER_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
            break;
#endif

#if defined(TIM2_BASE)
        case hwTimer_Index_1:
            HAL_NVIC_SetPriority(TIM2_IRQn, TIMER_IRQ_NVIC_PRIORITY, TIMER_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(TIM2_IRQn);
            break;
#endif

#if defined (STM32G0B0xx) || defined (STM32G0B1xx) || defined (STM32G0C1xx)
#if defined(TIM3_BASE) || defined(TIM4_BASE)
#if defined(TIM3_BASE)
        case hwTimer_Index_2:
#endif
#if defined(TIM4_BASE)
        case hwTimer_Index_3:
#endif
            HAL_NVIC_SetPriority(TIM3_TIM4_IRQn, TIMER_IRQ_NVIC_PRIORITY, TIMER_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(TIM3_TIM4_IRQn);
            break;
#endif
#else
#if defined(TIM3_BASE)
        case hwTimer_Index_2:
            HAL_NVIC_SetPriority(TIM3_IRQn, TIMER_IRQ_NVIC_PRIORITY, TIMER_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(TIM3_IRQn);
            break;
#endif
#endif

#if defined(TIM14_BASE)
        case hwTimer_Index_13:
            HAL_NVIC_SetPriority(TIM14_IRQn, TIMER_IRQ_NVIC_PRIORITY, TIMER_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(TIM14_IRQn);
            break;
#endif
#if defined(TIM16_BASE)
        case hwTimer_Index_15:
#if defined (STM32G0B0xx) || defined (STM32G0B1xx) || defined (STM32G0C1xx)
            HAL_NVIC_SetPriority(TIM16_FDCAN_IT0_IRQn, TIMER_IRQ_NVIC_PRIORITY, TIMER_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(TIM16_FDCAN_IT0_IRQn);
#else
            HAL_NVIC_SetPriority(TIM16_IRQn, TIMER_IRQ_NVIC_PRIORITY, TIMER_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(TIM16_IRQn);
#endif
            break;
#endif
#if defined(TIM17_BASE)
        case hwTimer_Index_16:
#if defined (STM32G0B0xx) || defined (STM32G0B1xx) || defined (STM32G0C1xx)
            HAL_NVIC_SetPriority(TIM17_FDCAN_IT1_IRQn, TIMER_IRQ_NVIC_PRIORITY, TIMER_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(TIM17_FDCAN_IT1_IRQn);
#else
            HAL_NVIC_SetPriority(TIM17_IRQn, TIMER_IRQ_NVIC_PRIORITY, TIMER_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(TIM17_IRQn);
#endif
            break;
#endif

        default:
            break;
    }

    g_timer[index].Instance = timer_soc_base;
    g_timer[index].Init.Prescaler         = (Timer_GetInputClock(index) / 1000000U) - 1U; /* 1 MHz = 1 us */
    g_timer[index].Init.CounterMode       = TIM_COUNTERMODE_UP;
    g_timer[index].Init.Period            = 1000U - 1U; /* default 1ms */
    g_timer[index].Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    g_timer[index].Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    if (HAL_TIM_Base_Init(&g_timer[index]) != HAL_OK)
    {
        return hwTimer_HwError;
    }

    Timer_Init_Status[index] = true;

    return hwTimer_OK;
}

hwTimer_OpResult Timer_DeInit(hwTimer_Index index)
{
    if (index >= hwTimer_Index_MAX)
    {
        return hwTimer_InvalidParameter;
    }

    if (Timer_Init_Status[index] == false)
    {
        return hwTimer_OK;
    }

    Timer_Init_Status[index] = false;

    HAL_TIM_Base_Stop_IT(&g_timer[index]);
    HAL_TIM_Base_DeInit(&g_timer[index]);

    switch (index)
    {
#if defined(TIM1_BASE)
        case hwTimer_Index_0:
            HAL_NVIC_DisableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
            break;
#endif

#if defined(TIM2_BASE)
        case hwTimer_Index_1:
            HAL_NVIC_DisableIRQ(TIM2_IRQn);
            break;
#endif

#if defined (STM32G0B0xx) || defined (STM32G0B1xx) || defined (STM32G0C1xx)
#if defined(TIM3_BASE) || defined(TIM4_BASE)
#if defined(TIM3_BASE)
        case hwTimer_Index_2:
#endif
#if defined(TIM4_BASE)
        case hwTimer_Index_3:
#endif
#if defined(TIM3_BASE) && defined(TIM4_BASE)
            if(Timer_Init_Status[hwTimer_Index_2] || Timer_Init_Status[hwTimer_Index_3])
#endif
            {
                HAL_NVIC_DisableIRQ(TIM3_TIM4_IRQn);
            }
            break;
#endif
#else
#if defined(TIM3_BASE)
        case hwTimer_Index_2:
            HAL_NVIC_DisableIRQ(TIM3_IRQn);
            break;
#endif
#endif

#if defined(TIM14_BASE)
        case hwTimer_Index_13:
            HAL_NVIC_DisableIRQ(TIM14_IRQn);
            break;
#endif

#if defined(TIM16_BASE)
        case hwTimer_Index_15:
#if defined (STM32G0B0xx) || defined (STM32G0B1xx) || defined (STM32G0C1xx)
            HAL_NVIC_DisableIRQ(TIM16_FDCAN_IT0_IRQn);
#else
            HAL_NVIC_DisableIRQ(TIM16_IRQn);
#endif
            break;
#endif

#if defined(TIM17_BASE)
        case hwTimer_Index_16:
#if defined (STM32G0B0xx) || defined (STM32G0B1xx) || defined (STM32G0C1xx)
            HAL_NVIC_DisableIRQ(TIM17_FDCAN_IT1_IRQn);
#else
            HAL_NVIC_DisableIRQ(TIM17_IRQn);
#endif
            break;
#endif

        default:
            break;
    }

    switch (index)
    {
#if defined(TIM1_BASE)
        case hwTimer_Index_0:
            __HAL_RCC_TIM1_CLK_DISABLE();
            break;
#endif

#if defined(TIM2_BASE)
        case hwTimer_Index_1:
            __HAL_RCC_TIM2_CLK_DISABLE();
            break;
#endif

#if defined(TIM3_BASE)
        case hwTimer_Index_2:
            __HAL_RCC_TIM3_CLK_DISABLE();
            break;
#endif

#if defined(TIM14_BASE)
        case hwTimer_Index_13:
            __HAL_RCC_TIM14_CLK_DISABLE();
            break;
#endif

#if defined(TIM16_BASE)
        case hwTimer_Index_15:
            __HAL_RCC_TIM16_CLK_DISABLE();
            break;
#endif

#if defined(TIM17_BASE)
        case hwTimer_Index_16:
            __HAL_RCC_TIM17_CLK_DISABLE();
            break;
#endif

        default:
            break;
    }

    return hwTimer_OK;
}

hwTimer_OpResult Timer_Start_OneShout(hwTimer_Index index, uint32_t duration_us, onTimerEventHandler timer_exp_cb)
{
    if (index >= hwTimer_Index_MAX)
    {
        return hwTimer_InvalidParameter;
    }

    if (Timer_Init_Status[index] == false)
    {
        return hwTimer_NotInit;
    }

    if (duration_us == 0)
    {
        return hwTimer_InvalidParameter;
    }

    Timer_Expired_Handler[index] = timer_exp_cb;
    Timer_IsPeriodic[index] = false;

    HAL_TIM_Base_Stop_IT(&g_timer[index]);

    g_timer[index].Init.Period = duration_us - 1U;
    if (HAL_TIM_Base_Init(&g_timer[index]) != HAL_OK)
    {
        return hwTimer_HwError;
    }

    if (HAL_TIM_Base_Start_IT(&g_timer[index]) != HAL_OK)
    {
        return hwTimer_HwError;
    }

    return hwTimer_OK;
}

hwTimer_OpResult Timer_Start_Period(hwTimer_Index index, uint32_t duration_us, onTimerEventHandler timer_exp_cb)
{
    if (index >= hwTimer_Index_MAX)
    {
        return hwTimer_InvalidParameter;
    }

    if (Timer_Init_Status[index] == false)
    {
        return hwTimer_NotInit;
    }

    if (duration_us == 0)
    {
        return hwTimer_InvalidParameter;
    }

    Timer_Expired_Handler[index] = timer_exp_cb;
    Timer_IsPeriodic[index] = true;

    HAL_TIM_Base_Stop_IT(&g_timer[index]);

    g_timer[index].Init.Period = duration_us - 1U;
    if (HAL_TIM_Base_Init(&g_timer[index]) != HAL_OK)
    {
        return hwTimer_HwError;
    }

    if (HAL_TIM_Base_Start_IT(&g_timer[index]) != HAL_OK)
    {
        return hwTimer_HwError;
    }

    return hwTimer_OK;
}

hwTimer_OpResult Timer_Reload(hwTimer_Index index, uint32_t duration_us)
{
    if (index >= hwTimer_Index_MAX)
    {
        return hwTimer_InvalidParameter;
    }

    if (Timer_Init_Status[index] == false)
    {
        return hwTimer_NotInit;
    }

    if (duration_us == 0)
    {
        return hwTimer_InvalidParameter;
    }

    HAL_TIM_Base_Stop_IT(&g_timer[index]);

    g_timer[index].Init.Period = duration_us - 1U;
    if (HAL_TIM_Base_Init(&g_timer[index]) != HAL_OK)
    {
        return hwTimer_HwError;
    }

    if (HAL_TIM_Base_Start_IT(&g_timer[index]) != HAL_OK)
    {
        return hwTimer_HwError;
    }

    return hwTimer_OK;
}

hwTimer_OpResult Timer_Stop(hwTimer_Index index)
{
    if (index >= hwTimer_Index_MAX)
    {
        return hwTimer_InvalidParameter;
    }

    if (Timer_Init_Status[index] == false)
    {
        return hwTimer_NotInit;
    }

    HAL_TIM_Base_Stop_IT(&g_timer[index]);

    return hwTimer_OK;
}

hwTimer_OpResult Timer_Read_Ticks(hwTimer_Index index, uint32_t* ticks)
{
    if (index >= hwTimer_Index_MAX)
    {
        return hwTimer_InvalidParameter;
    }

    if (ticks == NULL)
    {
        return hwTimer_InvalidParameter;
    }

    if (Timer_Init_Status[index] == false)
    {
        return hwTimer_NotInit;
    }

    *ticks = g_timer[index].Instance->CNT;

    return hwTimer_OK;
}

hwTimer_OpResult Timer_Read_uSec(hwTimer_Index index, uint32_t* uSec)
{
    if (index >= hwTimer_Index_MAX)
    {
        return hwTimer_InvalidParameter;
    }

    if (uSec == NULL)
    {
        return hwTimer_InvalidParameter;
    }

    if (Timer_Init_Status[index] == false)
    {
        return hwTimer_NotInit;
    }

    return Timer_Read_Ticks(index, uSec);
}

bool Timer_is_Init(hwTimer_Index hw_index)
{
    if (hw_index >= hwTimer_Index_MAX)
    {
        return false;
    }

    return Timer_Init_Status[hw_index];
}

#endif // STM32F0