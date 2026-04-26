#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

#include "soc.h"
#include "NeonRTOS.h"
#include "Timer/Timer.h"

#ifdef STM32G4

#define TIMER_IRQ_NVIC_PRIORITY        5
#define TIMER_IRQ_NVIC_SUB_PRIORITY    0

static bool Timer_Init_Status[hwTimer_Index_MAX] = {false};
static bool Timer_IsPeriodic[hwTimer_Index_MAX] = {false};
static onTimerEventHandler Timer_Expired_Handler[hwTimer_Index_MAX] = {NULL};

TIM_HandleTypeDef g_timer[hwTimer_Index_MAX];

#if defined(HRTIM1_BASE)
static HRTIM_HandleTypeDef g_hrtim1;
static bool HRTIM1_Init_Status = false;
#endif

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
#if defined(TIM4_BASE)
        case hwTimer_Index_3: return TIM4;
#endif
#if defined(TIM6_BASE)
        case hwTimer_Index_5: return TIM6;
#endif
#if defined(TIM7_BASE)
        case hwTimer_Index_6: return TIM7;
#endif
#if defined(TIM8_BASE)
        case hwTimer_Index_7: return TIM8;
#endif
#if defined(TIM15_BASE)
        case hwTimer_Index_14: return TIM15;
#endif
#if defined(TIM16_BASE)
        case hwTimer_Index_15: return TIM16;
#endif
#if defined(TIM17_BASE)
        case hwTimer_Index_16: return TIM17;
#endif
#if defined(TIM20_BASE)
        case hwTimer_Index_19: return TIM20;
#endif
        default: break;
    }
    return NULL;
}

static uint32_t Timer_GetInputClock(hwTimer_Index index)
{
    switch (index)
    {
#if defined(TIM1_BASE)
        case hwTimer_Index_0:
#endif
#if defined(TIM8_BASE)
        case hwTimer_Index_7:
#endif
#if defined(TIM15_BASE)
        case hwTimer_Index_14:
#endif
#if defined(TIM16_BASE)
        case hwTimer_Index_15:
#endif
#if defined(TIM17_BASE)
        case hwTimer_Index_16:
#endif
#if defined(TIM20_BASE)
        case hwTimer_Index_19:
#endif
            return HAL_RCC_GetPCLK2Freq();

        default:
            return HAL_RCC_GetPCLK1Freq();
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    hwTimer_Index idx = Timer_IndexFromHandle(htim);

    if (idx >= hwTimer_Index_MAX) {
        return;
    }

    if (Timer_Expired_Handler[idx] != NULL) {
        Timer_Expired_Handler[idx](idx);
    }

    if (!Timer_IsPeriodic[idx]) {
        HAL_TIM_Base_Stop_IT(htim);
    }
}

static void Timer_HAL_IRQHandler(hwTimer_Index index)
{
    HAL_TIM_IRQHandler(&g_timer[index]);
}

/* ================= IRQ Handlers STM32G4 ================= */

#if defined(TIM1_UP_TIM16_IRQn)
void TIM1_UP_TIM16_IRQHandler(void)
{
#if defined(TIM1_BASE)
    Timer_HAL_IRQHandler(hwTimer_Index_0);
#endif
#if defined(TIM16_BASE)
    Timer_HAL_IRQHandler(hwTimer_Index_15);
#endif
}
#elif defined(TIM1_UP_IRQn)
void TIM1_UP_IRQHandler(void)
{
    Timer_HAL_IRQHandler(hwTimer_Index_0);
}
#endif

#if defined(TIM1_TRG_COM_TIM17_IRQn)
void TIM1_TRG_COM_TIM17_IRQHandler(void)
{
#if defined(TIM17_BASE)
    Timer_HAL_IRQHandler(hwTimer_Index_16);
#endif
}
#elif defined(TIM17_IRQn)
void TIM17_IRQHandler(void)
{
    Timer_HAL_IRQHandler(hwTimer_Index_16);
}
#endif

#if defined(TIM2_IRQn)
void TIM2_IRQHandler(void) { Timer_HAL_IRQHandler(hwTimer_Index_1); }
#endif

#if defined(TIM3_IRQn)
void TIM3_IRQHandler(void) { Timer_HAL_IRQHandler(hwTimer_Index_2); }
#endif

#if defined(TIM4_IRQn)
void TIM4_IRQHandler(void) { Timer_HAL_IRQHandler(hwTimer_Index_3); }
#endif

#if defined(TIM6_DAC_IRQn)
void TIM6_DAC_IRQHandler(void) { Timer_HAL_IRQHandler(hwTimer_Index_5); }
#elif defined(TIM6_IRQn)
void TIM6_IRQHandler(void) { Timer_HAL_IRQHandler(hwTimer_Index_5); }
#endif

#if defined(TIM7_DAC_IRQn)
void TIM7_DAC_IRQHandler(void) { Timer_HAL_IRQHandler(hwTimer_Index_6); }
#elif defined(TIM7_IRQn)
void TIM7_IRQHandler(void) { Timer_HAL_IRQHandler(hwTimer_Index_6); }
#endif

#if defined(TIM8_UP_IRQn)
void TIM8_UP_IRQHandler(void) { Timer_HAL_IRQHandler(hwTimer_Index_7); }
#endif

#if defined(TIM1_BRK_TIM15_IRQn)
void TIM1_BRK_TIM15_IRQHandler(void)
{
#if defined(TIM15_BASE)
    Timer_HAL_IRQHandler(hwTimer_Index_14);
#endif
}
#elif defined(TIM15_IRQn)
void TIM15_IRQHandler(void)
{
    Timer_HAL_IRQHandler(hwTimer_Index_14);
}
#endif

#if defined(TIM20_UP_IRQn)
void TIM20_UP_IRQHandler(void) { Timer_HAL_IRQHandler(hwTimer_Index_19); }
#endif

hwTimer_OpResult Timer_Init(hwTimer_Index index)
{
    if (index >= hwTimer_Index_MAX) {
        return hwTimer_InvalidParameter;
    }

    if (Timer_Init_Status[index]) {
        return hwTimer_OK;
    }

    TIM_TypeDef *timer_soc_base = Timer_Map_Soc_Base(index);
    if (timer_soc_base == NULL) {
        return hwTimer_InvalidParameter;
    }

    switch (index)
    {
#if defined(TIM1_BASE)
        case hwTimer_Index_0:  __HAL_RCC_TIM1_CLK_ENABLE(); break;
#endif
#if defined(TIM2_BASE)
        case hwTimer_Index_1:  __HAL_RCC_TIM2_CLK_ENABLE(); break;
#endif
#if defined(TIM3_BASE)
        case hwTimer_Index_2:  __HAL_RCC_TIM3_CLK_ENABLE(); break;
#endif
#if defined(TIM4_BASE)
        case hwTimer_Index_3:  __HAL_RCC_TIM4_CLK_ENABLE(); break;
#endif
#if defined(TIM6_BASE)
        case hwTimer_Index_5:  __HAL_RCC_TIM6_CLK_ENABLE(); break;
#endif
#if defined(TIM7_BASE)
        case hwTimer_Index_6:  __HAL_RCC_TIM7_CLK_ENABLE(); break;
#endif
#if defined(TIM8_BASE)
        case hwTimer_Index_7:  __HAL_RCC_TIM8_CLK_ENABLE(); break;
#endif
#if defined(TIM15_BASE)
        case hwTimer_Index_14: __HAL_RCC_TIM15_CLK_ENABLE(); break;
#endif
#if defined(TIM16_BASE)
        case hwTimer_Index_15: __HAL_RCC_TIM16_CLK_ENABLE(); break;
#endif
#if defined(TIM17_BASE)
        case hwTimer_Index_16: __HAL_RCC_TIM17_CLK_ENABLE(); break;
#endif
#if defined(TIM20_BASE)
        case hwTimer_Index_19: __HAL_RCC_TIM20_CLK_ENABLE(); break;
#endif
        default:
            return hwTimer_InvalidParameter;
    }

    switch (index)
    {
#if defined(TIM1_UP_TIM16_IRQn)
        case hwTimer_Index_0:
            HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, TIMER_IRQ_NVIC_PRIORITY, TIMER_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
            break;
#elif defined(TIM1_UP_IRQn)
        case hwTimer_Index_0:
            HAL_NVIC_SetPriority(TIM1_UP_IRQn, TIMER_IRQ_NVIC_PRIORITY, TIMER_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
            break;
#endif

#if defined(TIM2_IRQn)
        case hwTimer_Index_1:
            HAL_NVIC_SetPriority(TIM2_IRQn, TIMER_IRQ_NVIC_PRIORITY, TIMER_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(TIM2_IRQn);
            break;
#endif

#if defined(TIM3_IRQn)
        case hwTimer_Index_2:
            HAL_NVIC_SetPriority(TIM3_IRQn, TIMER_IRQ_NVIC_PRIORITY, TIMER_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(TIM3_IRQn);
            break;
#endif

#if defined(TIM4_IRQn)
        case hwTimer_Index_3:
            HAL_NVIC_SetPriority(TIM4_IRQn, TIMER_IRQ_NVIC_PRIORITY, TIMER_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(TIM4_IRQn);
            break;
#endif

#if defined(TIM6_DAC_IRQn)
        case hwTimer_Index_5:
            HAL_NVIC_SetPriority(TIM6_DAC_IRQn, TIMER_IRQ_NVIC_PRIORITY, TIMER_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
            break;
#elif defined(TIM6_IRQn)
        case hwTimer_Index_5:
            HAL_NVIC_SetPriority(TIM6_IRQn, TIMER_IRQ_NVIC_PRIORITY, TIMER_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(TIM6_IRQn);
            break;
#endif

#if defined(TIM7_DAC_IRQn)
        case hwTimer_Index_6:
            HAL_NVIC_SetPriority(TIM7_DAC_IRQn, TIMER_IRQ_NVIC_PRIORITY, TIMER_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(TIM7_DAC_IRQn);
            break;
#elif defined(TIM7_IRQn)
        case hwTimer_Index_6:
            HAL_NVIC_SetPriority(TIM7_IRQn, TIMER_IRQ_NVIC_PRIORITY, TIMER_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(TIM7_IRQn);
            break;
#endif

#if defined(TIM8_UP_IRQn)
        case hwTimer_Index_7:
            HAL_NVIC_SetPriority(TIM8_UP_IRQn, TIMER_IRQ_NVIC_PRIORITY, TIMER_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(TIM8_UP_IRQn);
            break;
#endif

#if defined(TIM1_BRK_TIM15_IRQn)
        case hwTimer_Index_14:
            HAL_NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, TIMER_IRQ_NVIC_PRIORITY, TIMER_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn);
            break;
#elif defined(TIM15_IRQn)
        case hwTimer_Index_14:
            HAL_NVIC_SetPriority(TIM15_IRQn, TIMER_IRQ_NVIC_PRIORITY, TIMER_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(TIM15_IRQn);
            break;
#endif

#if defined(TIM1_UP_TIM16_IRQn)
        case hwTimer_Index_15:
            HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, TIMER_IRQ_NVIC_PRIORITY, TIMER_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
            break;
#elif defined(TIM16_IRQn)
        case hwTimer_Index_15:
            HAL_NVIC_SetPriority(TIM16_IRQn, TIMER_IRQ_NVIC_PRIORITY, TIMER_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(TIM16_IRQn);
            break;
#endif

#if defined(TIM1_TRG_COM_TIM17_IRQn)
        case hwTimer_Index_16:
            HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM17_IRQn, TIMER_IRQ_NVIC_PRIORITY, TIMER_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM17_IRQn);
            break;
#elif defined(TIM17_IRQn)
        case hwTimer_Index_16:
            HAL_NVIC_SetPriority(TIM17_IRQn, TIMER_IRQ_NVIC_PRIORITY, TIMER_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(TIM17_IRQn);
            break;
#endif

#if defined(TIM20_UP_IRQn)
        case hwTimer_Index_19:
            HAL_NVIC_SetPriority(TIM20_UP_IRQn, TIMER_IRQ_NVIC_PRIORITY, TIMER_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(TIM20_UP_IRQn);
            break;
#endif

        default:
            break;
    }

    g_timer[index].Instance = timer_soc_base;
    g_timer[index].Init.Prescaler         = (Timer_GetInputClock(index) / 1000000U) - 1U;
    g_timer[index].Init.CounterMode       = TIM_COUNTERMODE_UP;
    g_timer[index].Init.Period            = 1000U - 1U;
    g_timer[index].Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    g_timer[index].Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    if (HAL_TIM_Base_Init(&g_timer[index]) != HAL_OK) {
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
#if defined(TIM1_UP_TIM16_IRQn)
        case hwTimer_Index_0:
            if (!Timer_Init_Status[hwTimer_Index_15])
            {
                HAL_NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn);
            }
            break;
#elif defined(TIM1_UP_IRQn)
        case hwTimer_Index_0:
            HAL_NVIC_DisableIRQ(TIM1_UP_IRQn);
            break;
#endif

#if defined(TIM2_IRQn)
        case hwTimer_Index_1:
            HAL_NVIC_DisableIRQ(TIM2_IRQn);
            break;
#endif

#if defined(TIM3_IRQn)
        case hwTimer_Index_2:
            HAL_NVIC_DisableIRQ(TIM3_IRQn);
            break;
#endif

#if defined(TIM4_IRQn)
        case hwTimer_Index_3:
            HAL_NVIC_DisableIRQ(TIM4_IRQn);
            break;
#endif

#if defined(TIM6_DAC_IRQn)
        case hwTimer_Index_5:
            HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
            break;
#elif defined(TIM6_IRQn)
        case hwTimer_Index_5:
            HAL_NVIC_DisableIRQ(TIM6_IRQn);
            break;
#endif

#if defined(TIM7_DAC_IRQn)
        case hwTimer_Index_6:
            HAL_NVIC_DisableIRQ(TIM7_DAC_IRQn);
            break;
#elif defined(TIM7_IRQn)
        case hwTimer_Index_6:
            HAL_NVIC_DisableIRQ(TIM7_IRQn);
            break;
#endif

#if defined(TIM8_UP_IRQn)
        case hwTimer_Index_7:
            HAL_NVIC_DisableIRQ(TIM8_UP_IRQn);
            break;
#endif

#if defined(TIM1_BRK_TIM15_IRQn)
        case hwTimer_Index_14:
            HAL_NVIC_DisableIRQ(TIM1_BRK_TIM15_IRQn);
            break;
#elif defined(TIM15_IRQn)
        case hwTimer_Index_14:
            HAL_NVIC_DisableIRQ(TIM15_IRQn);
            break;
#endif

#if defined(TIM1_UP_TIM16_IRQn)
        case hwTimer_Index_15:
            if (!Timer_Init_Status[hwTimer_Index_0])
            {
                HAL_NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn);
            }
            break;
#elif defined(TIM16_IRQn)
        case hwTimer_Index_15:
            HAL_NVIC_DisableIRQ(TIM16_IRQn);
            break;
#endif

#if defined(TIM1_TRG_COM_TIM17_IRQn)
        case hwTimer_Index_16:
            HAL_NVIC_DisableIRQ(TIM1_TRG_COM_TIM17_IRQn);
            break;
#elif defined(TIM17_IRQn)
        case hwTimer_Index_16:
            HAL_NVIC_DisableIRQ(TIM17_IRQn);
            break;
#endif

#if defined(TIM20_UP_IRQn)
        case hwTimer_Index_19:
            HAL_NVIC_DisableIRQ(TIM20_UP_IRQn);
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

#if defined(TIM4_BASE)
        case hwTimer_Index_3:
            __HAL_RCC_TIM4_CLK_DISABLE();
            break;
#endif

#if defined(TIM6_BASE)
        case hwTimer_Index_5:
            __HAL_RCC_TIM6_CLK_DISABLE();
            break;
#endif

#if defined(TIM7_BASE)
        case hwTimer_Index_6:
            __HAL_RCC_TIM7_CLK_DISABLE();
            break;
#endif

#if defined(TIM8_BASE)
        case hwTimer_Index_7:
            __HAL_RCC_TIM8_CLK_DISABLE();
            break;
#endif

#if defined(TIM15_BASE)
        case hwTimer_Index_14:
            __HAL_RCC_TIM15_CLK_DISABLE();
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

#if defined(TIM20_BASE)
        case hwTimer_Index_19:
            __HAL_RCC_TIM20_CLK_DISABLE();
            break;
#endif

        default:
            break;
    }

    Timer_Expired_Handler[index] = NULL;
    Timer_IsPeriodic[index] = false;

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

#endif // STM32G4