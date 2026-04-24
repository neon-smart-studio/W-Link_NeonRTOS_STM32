
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

#include "soc.h"

#include "NeonRTOS.h"

#include "UART/UART.h"

#ifdef STM32F3

#include "UART/UART_Pin.h"

#include "GPIO/Device/GPIO_STM32.h"

#define UART_IRQ_NVIC_PRIORITY 5
#define UART_IRQ_NVIC_SUB_PRIORITY 0

static bool UART_Init_Status[hwUART_Index_MAX] = {false};
static int UART_BaudRate[hwUART_Index_MAX] = {0};
static bool UART_FlowControl[hwUART_Index_MAX] = {false};
static NeonRTOS_SyncObj_t UART_Send_SyncHandle[hwUART_Index_MAX];
static NeonRTOS_SyncObj_t UART_Recv_SyncHandle[hwUART_Index_MAX];

UART_HandleTypeDef g_uart[hwUART_Index_MAX];

uint32_t STM32_UART_GetAF(hwUART_Index uart, hwGPIO_Pin pin)
{
    for (size_t i = 0; i < sizeof(UART_Pin_AF_Map)/sizeof(UART_Pin_AF_Map[0]); i++) {
        if (UART_Pin_AF_Map[i].uart == uart &&
            UART_Pin_AF_Map[i].pin  == pin)
            return UART_Pin_AF_Map[i].af;
    }
    return 0;
}

static hwUART_Index UART_IndexFromHandle(UART_HandleTypeDef *huart)
{
    for(int i=0;i<hwUART_Index_MAX;i++){
        if(&g_uart[i] == huart) return (hwUART_Index)i;
    }
    return hwUART_Index_MAX;
}

USART_TypeDef * UART_Map_Soc_Base(hwUART_Index index)
{
    switch(index)
    {
#if defined(UART1)
        case hwUART_Index_0: return UART1;
#endif
#if defined(USART1)
        case hwUART_Index_0: return USART1;
#endif
#if defined(UART2)
        case hwUART_Index_1: return UART2;
#endif
#if defined(USART2)
        case hwUART_Index_1: return USART2;
#endif
#if defined(UART3)
        case hwUART_Index_2: return UART3;
#endif
#if defined(USART3)
        case hwUART_Index_2: return USART3;
#endif
#if defined(UART4)
        case hwUART_Index_3: return UART4;
#endif
#if defined(USART4)
        case hwUART_Index_3: return USART4;
#endif
#if defined(UART5)
        case hwUART_Index_4: return UART5;
#endif
#if defined(USART5)
        case hwUART_Index_4: return USART5;
#endif
#if defined(UART6)
        case hwUART_Index_5: return UART6;
#endif
#if defined(USART6)
        case hwUART_Index_5: return USART6;
#endif
        default: break;
    }

    return NULL;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    hwUART_Index idx = UART_IndexFromHandle(huart);
    if(idx >= hwUART_Index_MAX) return;

#if defined(STM32H7)
    __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_RXFNE);
#else
    __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_RXNE);
#endif
    huart->RxState = HAL_UART_STATE_READY;

    NeonRTOS_SyncObjSignalFromISR(&UART_Recv_SyncHandle[idx]);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    hwUART_Index idx = UART_IndexFromHandle(huart);
    if(idx >= hwUART_Index_MAX) return;

    NeonRTOS_SyncObjSignalFromISR(&UART_Send_SyncHandle[idx]);
}

// UART IRQ 統一入口
static void UART_HAL_IRQHandler(hwUART_Index index)
{
    HAL_UART_IRQHandler(&g_uart[index]);
}

// ====== 你的 IRQ handler（請確保對應的向量真的存在）======
#if defined(UART1)
void UART1_IRQHandler(void){ UART_HAL_IRQHandler(hwUART_Index_0); }
#endif
#if defined(USART1)
void USART1_IRQHandler(void){ UART_HAL_IRQHandler(hwUART_Index_0); }
#endif

#if defined(UART2)
void UART2_IRQHandler(void){ UART_HAL_IRQHandler(hwUART_Index_1); }
#endif
#if defined(USART2)
void USART2_IRQHandler(void){ UART_HAL_IRQHandler(hwUART_Index_1); }
#endif

#if defined(UART3)
void UART3_IRQHandler(void){ UART_HAL_IRQHandler(hwUART_Index_2); }
#endif
#if defined(USART3)
void USART3_IRQHandler(void){ UART_HAL_IRQHandler(hwUART_Index_2); }
#endif

#if defined(UART4)
void UART4_IRQHandler(void){ UART_HAL_IRQHandler(hwUART_Index_3); }
#endif
#if defined(USART4)
void USART4_IRQHandler(void){ UART_HAL_IRQHandler(hwUART_Index_3); }
#endif

#if defined(UART5)
void UART5_IRQHandler(void){ UART_HAL_IRQHandler(hwUART_Index_4); }
#endif
#if defined(USART5)
void USART5_IRQHandler(void){ UART_HAL_IRQHandler(hwUART_Index_4); }
#endif

#if defined(UART6)
void UART6_IRQHandler(void){ UART_HAL_IRQHandler(hwUART_Index_5); }
#endif
#if defined(USART6)
void USART6_IRQHandler(void){ UART_HAL_IRQHandler(hwUART_Index_5); }
#endif

hwUART_OpResult UART_Open(hwUART_Index index, uint32_t baudrate, bool rts_cts)
{
    if(index>=hwUART_Index_MAX)
    {
        return hwUART_InvalidParameter;
    }
    
    if(rts_cts)
    {
        if(UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].rts_pin==hwGPIO_Pin_NC || UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].cts_pin==hwGPIO_Pin_NC)
        {
            return hwUART_Unsupport;
        }
    }

    return UART_Open_Specific_Format(index, baudrate, rts_cts, 8, UART_Parity_None, 1);
}

hwUART_OpResult UART_Open_Specific_Format(hwUART_Index index, uint32_t baudrate, bool rts_cts, uint8_t data_bits, UART_Parity parity, uint8_t stop_bits)
{
    if(index>=hwUART_Index_MAX)
    {
        return hwUART_InvalidParameter;
    }
  
    if(parity>=UART_Parity_MAX)
    {
        return hwUART_InvalidParameter;
    }
    
    if(stop_bits!=1 && stop_bits!=2)
    {
        return hwUART_InvalidParameter;
    }
    
#ifdef UART_WORDLENGTH_7B
    if(data_bits!=7 && data_bits!=8 && data_bits!=9)
    {
        return hwUART_InvalidParameter;
    }
#else
    if(data_bits!=8 && data_bits!=9)
    {
        return hwUART_InvalidParameter;
    }
#endif
    
    if(UART_Init_Status[index]==true)
    {
        return hwUART_OK;
    }

    if(rts_cts)
    {
        if(UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].rts_pin==hwGPIO_Pin_NC || UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].cts_pin==hwGPIO_Pin_NC)
        {
            return hwUART_Unsupport;
        }
    }

    if(NeonRTOS_SyncObjCreate(&UART_Send_SyncHandle[index])!=NeonRTOS_OK)
    {
        return hwUART_MemoryError;
    }
    
    if(NeonRTOS_SyncObjCreate(&UART_Recv_SyncHandle[index])!=NeonRTOS_OK)
    {
        NeonRTOS_SyncObjDelete(&UART_Send_SyncHandle[index]);
        return hwUART_MemoryError;
    }

    UART_BaudRate[index] = baudrate;
    
    GPIO_TypeDef * tx_soc_base = GPIO_Map_Soc_Base(UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].tx_pin);
    GPIO_TypeDef * rx_soc_base = GPIO_Map_Soc_Base(UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].rx_pin);
    uint16_t tx_soc_pin = GPIO_Map_Soc_Pin(UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].tx_pin);
    uint16_t rx_soc_pin = GPIO_Map_Soc_Pin(UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].rx_pin);

    GPIO_TypeDef * rts_soc_base = NULL;
    GPIO_TypeDef * cts_soc_base = NULL;
    uint16_t rts_soc_pin = 0;
    uint16_t cts_soc_pin = 0;

    if(tx_soc_pin==0 || tx_soc_base==NULL || rx_soc_pin==0 || rx_soc_base==NULL)
    {
        return hwGPIO_InvalidParameter;
    }

    if(rts_cts)
    {
        rts_soc_base = GPIO_Map_Soc_Base(UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].rts_pin);
        cts_soc_base = GPIO_Map_Soc_Base(UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].cts_pin);
        rts_soc_pin = GPIO_Map_Soc_Pin(UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].rts_pin);
        cts_soc_pin = GPIO_Map_Soc_Pin(UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].cts_pin);
        
        if(rts_soc_pin==0 || rts_soc_base==NULL || cts_soc_pin==0 || cts_soc_base==NULL)
        {
            return hwGPIO_InvalidParameter;
        }
    }

    uint32_t tx_af = STM32_UART_GetAF(index, UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].tx_pin);
    uint32_t rx_af = STM32_UART_GetAF(index, UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].rx_pin);
    uint32_t rts_af = 0;
    uint32_t cts_af = 0;

    if(tx_af==0 || rx_af==0)
    {
        return hwGPIO_InvalidParameter;
    }

    if(rts_cts)
    {
        rts_af = STM32_UART_GetAF(index, UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].rts_pin);
        cts_af = STM32_UART_GetAF(index, UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].cts_pin);
        
        if(rts_af==0 || cts_af==0)
        {
            return hwGPIO_InvalidParameter;
        }
    }

    USART_TypeDef * uart_soc_base = UART_Map_Soc_Base(index);
    if(uart_soc_base==NULL)
    {
        return hwGPIO_InvalidParameter;
    }

    GPIO_Enable_RCC_Clock(tx_soc_base);
    GPIO_Enable_RCC_Clock(rx_soc_base);
    
    if(rts_cts)
    {
        GPIO_Enable_RCC_Clock(rts_soc_base);
        GPIO_Enable_RCC_Clock(cts_soc_base);
    }

    // UART peripheral clock
    switch(index)
    {
#if defined(UART1)
        case hwUART_Index_0: __HAL_RCC_UART1_CLK_ENABLE(); break;
#endif
#if defined(USART1)
        case hwUART_Index_0: __HAL_RCC_USART1_CLK_ENABLE(); break;
#endif
#if defined(UART2)
        case hwUART_Index_1: __HAL_RCC_UART2_CLK_ENABLE(); break;
#endif
#if defined(USART2)
        case hwUART_Index_1: __HAL_RCC_USART2_CLK_ENABLE(); break;
#endif
#if defined(UART3)
        case hwUART_Index_2: __HAL_RCC_UART3_CLK_ENABLE(); break;
#endif
#if defined(USART3)
        case hwUART_Index_2: __HAL_RCC_USART3_CLK_ENABLE(); break;
#endif
#if defined(UART4)
        case hwUART_Index_3: __HAL_RCC_UART4_CLK_ENABLE(); break;
#endif
#if defined(USART4)
        case hwUART_Index_3: __HAL_RCC_USART4_CLK_ENABLE(); break;
#endif
#if defined(UART5)
        case hwUART_Index_4: __HAL_RCC_UART5_CLK_ENABLE(); break;
#endif
#if defined(USART5)
        case hwUART_Index_4: __HAL_RCC_USART5_CLK_ENABLE(); break;
#endif
#if defined(UART6)
        case hwUART_Index_5: __HAL_RCC_UART6_CLK_ENABLE(); break;
#endif
#if defined(USART6)
        case hwUART_Index_5: __HAL_RCC_USART6_CLK_ENABLE(); break;
#endif
    }

    GPIO_InitTypeDef g_uart_tx = {0};
    g_uart_tx.Pin       = tx_soc_pin;
    g_uart_tx.Mode      = GPIO_MODE_AF_PP;
    g_uart_tx.Pull      = GPIO_NOPULL;
    g_uart_tx.Speed     = GPIO_SPEED_FREQ_HIGH;
    g_uart_tx.Alternate = tx_af;
    HAL_GPIO_Init(tx_soc_base, &g_uart_tx);

    GPIO_InitTypeDef g_uart_rx = {0};
    g_uart_rx.Pin       = rx_soc_pin;
    g_uart_rx.Mode      = GPIO_MODE_AF_PP;
    g_uart_rx.Pull      = GPIO_NOPULL;
    g_uart_rx.Speed     = GPIO_SPEED_FREQ_HIGH;
    g_uart_rx.Alternate = rx_af;
    HAL_GPIO_Init(rx_soc_base, &g_uart_rx);

    if(rts_cts)
    {
        GPIO_InitTypeDef g_uart_rts = {0};
        g_uart_rts.Pin       = rts_soc_pin;
        g_uart_rts.Mode      = GPIO_MODE_AF_PP;
        g_uart_rts.Pull      = GPIO_NOPULL;
        g_uart_rts.Speed     = GPIO_SPEED_FREQ_HIGH;
        g_uart_rts.Alternate = rts_af;
        HAL_GPIO_Init(rts_soc_base, &g_uart_rts);

        GPIO_InitTypeDef g_uart_cts = {0};
        g_uart_cts.Pin       = cts_soc_pin;
        g_uart_cts.Mode      = GPIO_MODE_AF_PP;
        g_uart_cts.Pull      = GPIO_NOPULL;
        g_uart_cts.Speed     = GPIO_SPEED_FREQ_HIGH;
        g_uart_cts.Alternate = cts_af;
        HAL_GPIO_Init(cts_soc_base, &g_uart_cts);
    }

    UART_HandleTypeDef* huart = &g_uart[index];
    memset(huart, 0, sizeof(*huart));

    huart->Instance = uart_soc_base;
    huart->Init.BaudRate= baudrate * 3;
    switch(data_bits)
    {
#ifdef UART_WORDLENGTH_7B
        case 7:
            huart->Init.WordLength = UART_WORDLENGTH_7B;
            break;
#endif
        case 8:
            huart->Init.WordLength = UART_WORDLENGTH_8B;
            break;
        case 9:
            huart->Init.WordLength = UART_WORDLENGTH_9B;
            break;
    }
    switch(stop_bits)
    {
        case 1:
            huart->Init.StopBits = UART_STOPBITS_1;
            break;
        case 2:
            huart->Init.StopBits = UART_STOPBITS_2;
            break;
    }
    huart->Init.Mode       = UART_MODE_TX_RX;
    huart->Init.HwFlowCtl  = rts_cts ? UART_HWCONTROL_RTS_CTS : UART_HWCONTROL_NONE;
    huart->Init.OverSampling = UART_OVERSAMPLING_16;

    if(HAL_UART_Init(huart) != HAL_OK)
    {
        return hwUART_HwError;
    }

    switch(index)
    {
#if defined(UART1)
        case hwUART_Index_0:
            HAL_NVIC_SetPriority(UART1_IRQn, UART_IRQ_NVIC_PRIORITY, UART_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(UART1_IRQn);
            break;
#endif
#if defined(USART1)
        case hwUART_Index_0:
            HAL_NVIC_SetPriority(USART1_IRQn, UART_IRQ_NVIC_PRIORITY, UART_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(USART1_IRQn);
            break;
#endif
#if defined(UART2)
        case hwUART_Index_1:
            HAL_NVIC_SetPriority(UART2_IRQn, UART_IRQ_NVIC_PRIORITY, UART_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(UART2_IRQn);
            break;
#endif
#if defined(USART2)
        case hwUART_Index_1:
            HAL_NVIC_SetPriority(USART2_IRQn, UART_IRQ_NVIC_PRIORITY, UART_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(USART2_IRQn);
            break;

#endif
#if defined(UART3)
        case hwUART_Index_2:
            HAL_NVIC_SetPriority(UART3_IRQn, UART_IRQ_NVIC_PRIORITY, UART_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(UART3_IRQn);
            break;
#endif
#if defined(USART3)
        case hwUART_Index_2:
            HAL_NVIC_SetPriority(USART3_IRQn, UART_IRQ_NVIC_PRIORITY, UART_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(USART3_IRQn);
            break;
#endif
#if defined(UART4)
        case hwUART_Index_3:
            HAL_NVIC_SetPriority(UART4_IRQn, UART_IRQ_NVIC_PRIORITY, UART_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(UART4_IRQn);
            break;
#endif
#if defined(USART4)
        case hwUART_Index_3:
            HAL_NVIC_SetPriority(USART4_IRQn, UART_IRQ_NVIC_PRIORITY, UART_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(USART4_IRQn);
            break;
#endif
#if defined(UART5)
        case hwUART_Index_4:
            HAL_NVIC_SetPriority(UART5_IRQn, UART_IRQ_NVIC_PRIORITY, UART_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(UART5_IRQn);
            break;
#endif
#if defined(USART5)
        case hwUART_Index_4:
            HAL_NVIC_SetPriority(USART5_IRQn, UART_IRQ_NVIC_PRIORITY, UART_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(USART5_IRQn);
            break;
#endif
#if defined(UART6)
        case hwUART_Index_5:
            HAL_NVIC_SetPriority(UART6_IRQn, UART_IRQ_NVIC_PRIORITY, UART_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(UART6_IRQn);
            break;
#endif
#if defined(USART6)
        case hwUART_Index_5:
            HAL_NVIC_SetPriority(USART6_IRQn, UART_IRQ_NVIC_PRIORITY, UART_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(USART6_IRQn);
            break;
#endif
    }

    gpio_pin_init_status[UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].tx_pin] = true;
    gpio_pin_init_status[UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].rx_pin] = true;
    if(rts_cts)
    {
        gpio_pin_init_status[UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].rts_pin] = true;
        gpio_pin_init_status[UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].cts_pin] = true;

        UART_FlowControl[index] = true;
    }

    UART_Init_Status[index] = true;

    return hwUART_OK;
}

hwUART_OpResult UART_Close(hwUART_Index index)
{
    if(UART_Init_Status[index]==false)
    {
        return hwUART_OK;
    }

    GPIO_TypeDef * tx_soc_base = GPIO_Map_Soc_Base(UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].tx_pin);
    GPIO_TypeDef * rx_soc_base = GPIO_Map_Soc_Base(UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].rx_pin);
    uint16_t tx_soc_pin = GPIO_Map_Soc_Pin(UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].tx_pin);
    uint16_t rx_soc_pin = GPIO_Map_Soc_Pin(UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].rx_pin);

    GPIO_TypeDef * rts_soc_base = NULL;
    GPIO_TypeDef * cts_soc_base = NULL;
    uint16_t rts_soc_pin = 0;
    uint16_t cts_soc_pin = 0;

    if(tx_soc_pin==0 || tx_soc_base==NULL || rx_soc_pin==0 || rx_soc_base==NULL)
    {
        return hwGPIO_InvalidParameter;
    }

    if(UART_FlowControl[index])
    {
        rts_soc_base = GPIO_Map_Soc_Base(UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].rts_pin);
        cts_soc_base = GPIO_Map_Soc_Base(UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].cts_pin);
        rts_soc_pin = GPIO_Map_Soc_Pin(UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].rts_pin);
        cts_soc_pin = GPIO_Map_Soc_Pin(UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].cts_pin);
        
        if(rts_soc_pin==0 || rts_soc_base==NULL || cts_soc_pin==0 || cts_soc_base==NULL)
        {
            return hwGPIO_InvalidParameter;
        }
    }

    UART_HandleTypeDef* huart = &g_uart[index];

    switch(index)
    {
#if defined(UART1)
        case hwUART_Index_0:
            HAL_NVIC_DisableIRQ(UART1_IRQn);
            break;
#endif
#if defined(USART1)
        case hwUART_Index_0:
            HAL_NVIC_DisableIRQ(USART1_IRQn);
            break;
#endif
#if defined(UART2)
        case hwUART_Index_1:
            HAL_NVIC_DisableIRQ(UART2_IRQn);
            break;
#endif
#if defined(USART2)
        case hwUART_Index_1:
            HAL_NVIC_DisableIRQ(USART2_IRQn);
            break;

#endif
#if defined(UART3)
        case hwUART_Index_2:
            HAL_NVIC_DisableIRQ(UART3_IRQn);
            break;
#endif
#if defined(USART3)
        case hwUART_Index_2:
            HAL_NVIC_DisableIRQ(USART3_IRQn);
            break;
#endif
#if defined(UART4)
        case hwUART_Index_3:
            HAL_NVIC_DisableIRQ(UART4_IRQn);
            break;
#endif
#if defined(USART4)
        case hwUART_Index_3:
            HAL_NVIC_DisableIRQ(USART4_IRQn);
            break;
#endif
#if defined(UART5)
        case hwUART_Index_4:
            HAL_NVIC_DisableIRQ(UART5_IRQn);
            break;
#endif
#if defined(USART5)
        case hwUART_Index_4:
            HAL_NVIC_DisableIRQ(USART5_IRQn);
            break;
#endif
#if defined(UART6)
        case hwUART_Index_5:
            HAL_NVIC_DisableIRQ(UART6_IRQn);
            break;
#endif
#if defined(USART6)
        case hwUART_Index_5:
            HAL_NVIC_DisableIRQ(USART6_IRQn);
            break;
#endif
    }

    if(HAL_UART_DeInit(huart) != HAL_OK)
    {
        return hwUART_HwError;
    }
    
    // disable peripheral clock
    switch(index)
    {
#if defined(UART1)
        case hwUART_Index_0: __HAL_RCC_UART1_CLK_DISABLE(); break;
#endif
#if defined(USART1)
        case hwUART_Index_0: __HAL_RCC_USART1_CLK_DISABLE(); break;
#endif
#if defined(UART2)
        case hwUART_Index_1: __HAL_RCC_UART2_CLK_DISABLE(); break;
#endif
#if defined(USART2)
        case hwUART_Index_1: __HAL_RCC_USART2_CLK_DISABLE(); break;
#endif
#if defined(UART3)
        case hwUART_Index_2: __HAL_RCC_UART3_CLK_DISABLE(); break;
#endif
#if defined(USART3)
        case hwUART_Index_2: __HAL_RCC_USART3_CLK_DISABLE(); break;
#endif
#if defined(UART4)
        case hwUART_Index_3: __HAL_RCC_UART4_CLK_DISABLE(); break;
#endif
#if defined(USART4)
        case hwUART_Index_3: __HAL_RCC_USART4_CLK_DISABLE(); break;
#endif
#if defined(UART5)
        case hwUART_Index_4: __HAL_RCC_UART5_CLK_DISABLE(); break;
#endif
#if defined(USART5)
        case hwUART_Index_4: __HAL_RCC_USART5_CLK_DISABLE(); break;
#endif
#if defined(UART6)
        case hwUART_Index_5: __HAL_RCC_UART6_CLK_DISABLE(); break;
#endif
#if defined(USART6)
        case hwUART_Index_5: __HAL_RCC_USART6_CLK_DISABLE(); break;
#endif
    }

    HAL_GPIO_DeInit(tx_soc_base, tx_soc_pin);
    HAL_GPIO_DeInit(rx_soc_base, rx_soc_pin);

    if(UART_FlowControl[index])
    {
        HAL_GPIO_DeInit(rts_soc_base, rts_soc_pin);
        HAL_GPIO_DeInit(cts_soc_base, cts_soc_pin);
    }

    NeonRTOS_SyncObjDelete(&UART_Send_SyncHandle[index]);
    NeonRTOS_SyncObjDelete(&UART_Recv_SyncHandle[index]);

    gpio_pin_init_status[UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].tx_pin] = false;
    gpio_pin_init_status[UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].rx_pin] = false;
    if(UART_FlowControl[index])
    {
        gpio_pin_init_status[UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].rts_pin] = false;
        gpio_pin_init_status[UART_Pin_Def_Table[index][UART_Index_Map_Alt[index]].cts_pin] = false;

        UART_FlowControl[index] = false;
    }

    UART_Init_Status[index] = false;

    return hwUART_OK;
}

hwUART_OpResult UART_Read(hwUART_Index index, uint8_t* data_rd, size_t size, uint32_t timeoutMs)
{
    if(index>=hwUART_Index_MAX)
    {
        return hwUART_InvalidParameter;
    }
    
    if(!data_rd)
    {
        return hwUART_InvalidParameter;
    }

    if(UART_Init_Status[index]==false)
    {
        return hwUART_NotInit;
    }

    float wait_ms_f = ((float)size)/((float)UART_BaudRate[index]/8/1000);
    NeonRTOS_Time_t wait_ms;

    if(timeoutMs==NEONRT_WAIT_FOREVER) wait_ms = NEONRT_WAIT_FOREVER;
    else if(timeoutMs==NEONRT_NO_WAIT) wait_ms = NEONRT_NO_WAIT;
    else wait_ms = ((int)wait_ms_f) + timeoutMs + 1;

    UART_HandleTypeDef *huart = &g_uart[index];

    uint16_t recv_bytes = 0;

    for(recv_bytes = 0; recv_bytes < size; recv_bytes++)
    {
        NeonRTOS_SyncObjClear(&UART_Recv_SyncHandle[index]);
        
        huart->RxState = HAL_UART_STATE_READY;
        HAL_UART_Receive_IT(huart, &data_rd[recv_bytes], 1);

        if(NeonRTOS_SyncObjWait(&UART_Recv_SyncHandle[index], wait_ms)!=NeonRTOS_OK)
        {
            if(recv_bytes>0)
            {
                return (hwUART_OpResult)recv_bytes;
            }
            else
            {
                return hwUART_Busy;
            }
        }
    }

    return (hwUART_OpResult)recv_bytes;
}

hwUART_OpResult UART_GetChar(hwUART_Index index, uint8_t* char_rd, uint32_t timeoutMs)
{
    if(index>=hwUART_Index_MAX)
    {
        return hwUART_InvalidParameter;
    }
    
    if(!char_rd)
    {
        return hwUART_InvalidParameter;
    }

    if(UART_Init_Status[index]==false)
    {
        return hwUART_NotInit;
    }

    return UART_Read(index, char_rd, 1, timeoutMs);
}

hwUART_OpResult UART_Write(hwUART_Index index, uint8_t* data_wr, size_t size, uint32_t timeoutMs)
{
    if(index>=hwUART_Index_MAX)
    {
        return hwUART_InvalidParameter;
    }

    if(!data_wr || size==0)
    {
        return hwUART_InvalidParameter;
    }

    if(UART_Init_Status[index]==false)
    {
        return hwUART_NotInit;
    }

    float wait_ms_f = ((float)size)/((float)UART_BaudRate[index]/8/1000);
    NeonRTOS_Time_t wait_ms;

    if(timeoutMs==NEONRT_WAIT_FOREVER) wait_ms = NEONRT_WAIT_FOREVER;
    else if(timeoutMs==NEONRT_NO_WAIT) wait_ms = NEONRT_NO_WAIT;
    else wait_ms = ((int)wait_ms_f) + timeoutMs + 1;

    UART_HandleTypeDef *huart = &g_uart[index];

    uint16_t send_bytes = 0;
    
    for(send_bytes = 0; send_bytes<size; send_bytes++)
    {
        NeonRTOS_SyncObjClear(&UART_Send_SyncHandle[index]);
        
        HAL_UART_Transmit_IT(huart, &data_wr[send_bytes], 1);

        if(NeonRTOS_SyncObjWait(&UART_Send_SyncHandle[index], wait_ms)!=NeonRTOS_OK)
        {
            if(send_bytes>0)
            {
                return (hwUART_OpResult)send_bytes;
            }
            else
            {
                return hwUART_Busy;
            }
        }
    }
    
    return (hwUART_OpResult)send_bytes;
}

hwUART_OpResult UART_PutChar(hwUART_Index index, uint8_t char_wr, uint32_t timeoutMs)
{
    if(index>=hwUART_Index_MAX)
    {
        return hwUART_InvalidParameter;
    }
    
    if(UART_Init_Status[index]==false)
    {
        return hwUART_NotInit;
    }

    return UART_Write(index, &char_wr, 1, timeoutMs);
}

void UART_Printf(const char *format, ...)
{
    if(UART_Init_Status[LOG_UART_INDEX]==false)
    {
        return;
    }

    char buffer[256];
    va_list args;

    va_start(args, format);
    int len = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    UART_HandleTypeDef *huart = &g_uart[LOG_UART_INDEX];

    NeonRTOS_EnterCritical();
    
    for(int i=0;i<len;i++){
        while(HAL_UART_Transmit(huart, (uint8_t*)&buffer[i], 1, 10) == HAL_BUSY) {}
    }

    NeonRTOS_ExitCritical(0);
}

#endif //STM32F3