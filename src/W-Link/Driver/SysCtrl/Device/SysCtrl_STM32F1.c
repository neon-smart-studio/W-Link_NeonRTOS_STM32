
#include <stdint.h>
#include <stdbool.h>

#include "soc.h"

#ifdef STM32F1

void SysCtrl_Init()
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

#if defined(STM32F100xB) || defined(STM32F100xE) || defined(STM32F100x4) || defined(STM32F100x6)

    /* STM32F100: max 24 MHz */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
#if defined(RCC_HSE_PREDIV_DIV1)
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
#endif
    RCC_OscInitStruct.HSIState       = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL     = RCC_PLL_MUL3;   /* 8MHz x 3 = 24MHz */

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        while(1); // error
    }

    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK  |
                                       RCC_CLOCKTYPE_SYSCLK|
                                       RCC_CLOCKTYPE_PCLK1 |
                                       RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;   /* 24MHz */
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;     /* 24MHz */
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;     /* 24MHz */

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        while(1); // error
    }

#else

    /* F101/F103 常見 72MHz */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
#if defined(RCC_HSE_PREDIV_DIV1)
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
#endif
    RCC_OscInitStruct.HSIState       = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL     = RCC_PLL_MUL9;   /* 8MHz x 9 = 72MHz */

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        while(1); // error
    }

    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK  |
                                       RCC_CLOCKTYPE_SYSCLK|
                                       RCC_CLOCKTYPE_PCLK1 |
                                       RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;   /* 72MHz */
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;     /* 36MHz */
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;     /* 72MHz */

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        while(1); // error
    }

#endif
}

#endif //STM32F1
