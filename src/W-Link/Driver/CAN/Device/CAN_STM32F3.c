#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "soc.h"

#include "NeonRTOS.h"

#include "CAN/CAN.h"

#ifdef STM32F3

#if defined(CAN1_BASE)

#include "CAN/CAN_Pin.h"
#include "GPIO/Device/GPIO_STM32.h"

#define CAN_IRQ_NVIC_PRIORITY     5
#define CAN_IRQ_NVIC_SUB_PRIORITY 0

static bool CAN_Init_Status[hwCAN_Index_MAX] = {false};
static NeonRTOS_SyncObj_t CAN_TxDone_Sync[hwCAN_Index_MAX];
static NeonRTOS_MsgQ_t    CAN_RxQueue[hwCAN_Index_MAX];

static CAN_HandleTypeDef g_can[hwCAN_Index_MAX];

uint32_t STM32_CAN_GetAF(hwCAN_Index CAN, hwGPIO_Pin pin)
{
    for (size_t i = 0; i < sizeof(CAN_Pin_AF_Map) / sizeof(CAN_Pin_AF_Map[0]); i++) {
        if (CAN_Pin_AF_Map[i].can == CAN &&
            CAN_Pin_AF_Map[i].pin == pin) {
            return CAN_Pin_AF_Map[i].af;
        }
    }

    return 0;
}

static hwCAN_Index CAN_IndexFromHandle(CAN_HandleTypeDef *hcan)
{
    for (int i = 0; i < hwCAN_Index_MAX; i++) {
        if (&g_can[i] == hcan) {
            return (hwCAN_Index)i;
        }
    }

    return hwCAN_Index_MAX;
}

CAN_TypeDef *CAN_Map_Soc_Base(hwCAN_Index index)
{
    switch (index)
    {
        case hwCAN_Index_0:
            return CAN1;

        default:
            break;
    }

    return NULL;
}

/* =========================
 * STM32F3 CAN IRQ wrappers
 * ========================= */

#if defined(CAN_TX_IRQn)
    #define STM32F3_CAN_TX_IRQn CAN_TX_IRQn
#elif defined(USB_HP_CAN_TX_IRQn)
    #define STM32F3_CAN_TX_IRQn USB_HP_CAN_TX_IRQn
#elif defined(USB_HP_CAN1_TX_IRQn)
    #define STM32F3_CAN_TX_IRQn USB_HP_CAN1_TX_IRQn
#endif

#if defined(CAN_RX0_IRQn)
    #define STM32F3_CAN_RX0_IRQn CAN_RX0_IRQn
#elif defined(USB_LP_CAN_RX0_IRQn)
    #define STM32F3_CAN_RX0_IRQn USB_LP_CAN_RX0_IRQn
#elif defined(USB_LP_CAN1_RX0_IRQn)
    #define STM32F3_CAN_RX0_IRQn USB_LP_CAN1_RX0_IRQn
#endif

#if defined(CAN_RX1_IRQn)
    #define STM32F3_CAN_RX1_IRQn CAN_RX1_IRQn
#elif defined(CAN1_RX1_IRQn)
    #define STM32F3_CAN_RX1_IRQn CAN1_RX1_IRQn
#endif

#if defined(CAN_SCE_IRQn)
    #define STM32F3_CAN_SCE_IRQn CAN_SCE_IRQn
#elif defined(CAN1_SCE_IRQn)
    #define STM32F3_CAN_SCE_IRQn CAN1_SCE_IRQn
#endif

/* =========================
 * HAL callbacks
 * ========================= */

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
    hwCAN_Index index = CAN_IndexFromHandle(hcan);

    if (index < hwCAN_Index_MAX) {
        NeonRTOS_SyncObjSignalFromISR(&CAN_TxDone_Sync[index]);
    }
}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
    hwCAN_Index index = CAN_IndexFromHandle(hcan);

    if (index < hwCAN_Index_MAX) {
        NeonRTOS_SyncObjSignalFromISR(&CAN_TxDone_Sync[index]);
    }
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
    hwCAN_Index index = CAN_IndexFromHandle(hcan);

    if (index < hwCAN_Index_MAX) {
        NeonRTOS_SyncObjSignalFromISR(&CAN_TxDone_Sync[index]);
    }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef hdr;
    uint8_t data[8];

    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &hdr, data) != HAL_OK) {
        return;
    }

    hwCAN_Index index = CAN_IndexFromHandle(hcan);

    if (index < hwCAN_Index_MAX) {
        NeonRTOS_MsgQWrite(&CAN_RxQueue[index], data, NEONRT_NO_WAIT);
    }
}

/* =========================
 * IRQ handlers
 * ========================= */

#if defined(CAN_TX_IRQn)
void CAN_TX_IRQHandler(void)
{
    HAL_CAN_IRQHandler(&g_can[hwCAN_Index_0]);
}
#elif defined(USB_HP_CAN_TX_IRQn)
void USB_HP_CAN_TX_IRQHandler(void)
{
    HAL_CAN_IRQHandler(&g_can[hwCAN_Index_0]);
}
#elif defined(USB_HP_CAN1_TX_IRQn)
void USB_HP_CAN1_TX_IRQHandler(void)
{
    HAL_CAN_IRQHandler(&g_can[hwCAN_Index_0]);
}
#endif

#if defined(CAN_RX0_IRQn)
void CAN_RX0_IRQHandler(void)
{
    HAL_CAN_IRQHandler(&g_can[hwCAN_Index_0]);
}
#elif defined(USB_LP_CAN_RX0_IRQn)
void USB_LP_CAN_RX0_IRQHandler(void)
{
    HAL_CAN_IRQHandler(&g_can[hwCAN_Index_0]);
}
#elif defined(USB_LP_CAN1_RX0_IRQn)
void USB_LP_CAN1_RX0_IRQHandler(void)
{
    HAL_CAN_IRQHandler(&g_can[hwCAN_Index_0]);
}
#endif

#if defined(CAN_RX1_IRQn)
void CAN_RX1_IRQHandler(void)
{
    HAL_CAN_IRQHandler(&g_can[hwCAN_Index_0]);
}
#elif defined(CAN1_RX1_IRQn)
void CAN1_RX1_IRQHandler(void)
{
    HAL_CAN_IRQHandler(&g_can[hwCAN_Index_0]);
}
#endif

#if defined(CAN_SCE_IRQn)
void CAN_SCE_IRQHandler(void)
{
    HAL_CAN_IRQHandler(&g_can[hwCAN_Index_0]);
}
#elif defined(CAN1_SCE_IRQn)
void CAN1_SCE_IRQHandler(void)
{
    HAL_CAN_IRQHandler(&g_can[hwCAN_Index_0]);
}
#endif

/* =========================
 * Init
 * ========================= */

hwCAN_OpResult CAN_Init(hwCAN_Index index)
{
    if (index != hwCAN_Index_0) {
        return hwCAN_InvalidParameter;
    }

    if (CAN_Init_Status[index]) {
        return hwCAN_OK;
    }

    hwGPIO_Pin tx_hw_pin = CAN_Pin_Def_Table[index][CAN_Index_Map_Alt[index]].tx_pin;
    hwGPIO_Pin rx_hw_pin = CAN_Pin_Def_Table[index][CAN_Index_Map_Alt[index]].rx_pin;

    GPIO_TypeDef *tx_soc_base = GPIO_Map_Soc_Base(tx_hw_pin);
    GPIO_TypeDef *rx_soc_base = GPIO_Map_Soc_Base(rx_hw_pin);

    uint16_t tx_soc_pin = GPIO_Map_Soc_Pin(tx_hw_pin);
    uint16_t rx_soc_pin = GPIO_Map_Soc_Pin(rx_hw_pin);

    if (tx_soc_base == NULL || rx_soc_base == NULL ||
        tx_soc_pin == 0 || rx_soc_pin == 0) {
        return hwCAN_InvalidParameter;
    }

    uint32_t tx_af = STM32_CAN_GetAF(index, tx_hw_pin);
    uint32_t rx_af = STM32_CAN_GetAF(index, rx_hw_pin);

    if (tx_af == 0 || rx_af == 0) {
        return hwCAN_InvalidParameter;
    }

    CAN_TypeDef *can_soc_base = CAN_Map_Soc_Base(index);

    if (can_soc_base == NULL) {
        return hwCAN_InvalidParameter;
    }

    if (NeonRTOS_SyncObjCreate(&CAN_TxDone_Sync[index]) != NeonRTOS_OK) {
        return hwCAN_MemoryError;
    }

    if (NeonRTOS_MsgQCreate(&CAN_RxQueue[index], "can_rx", 8, 8) != NeonRTOS_OK) {
        NeonRTOS_SyncObjDelete(&CAN_TxDone_Sync[index]);
        return hwCAN_MemoryError;
    }

    GPIO_Enable_RCC_Clock(tx_soc_base);
    GPIO_Enable_RCC_Clock(rx_soc_base);

    GPIO_InitTypeDef gpio = {0};
    gpio.Mode      = GPIO_MODE_AF_PP;
    gpio.Pull      = GPIO_NOPULL;
    gpio.Speed     = GPIO_SPEED_FREQ_HIGH;

    gpio.Pin       = tx_soc_pin;
    gpio.Alternate = tx_af;
    HAL_GPIO_Init(tx_soc_base, &gpio);

    gpio.Pin       = rx_soc_pin;
    gpio.Alternate = rx_af;
    HAL_GPIO_Init(rx_soc_base, &gpio);

    __HAL_RCC_CAN1_CLK_ENABLE();

    g_can[index].Instance = can_soc_base;

    /*
     * Example timing:
     * APB1 = 36MHz
     * Prescaler = 4
     * TQ = 9MHz
     * 1 + BS1 13 + BS2 4 = 18TQ
     * CAN bitrate = 9MHz / 18 = 500kbps
     */
    g_can[index].Init.Prescaler = 4;
    g_can[index].Init.Mode = CAN_MODE_NORMAL;
    g_can[index].Init.SyncJumpWidth = CAN_SJW_1TQ;
    g_can[index].Init.TimeSeg1 = CAN_BS1_13TQ;
    g_can[index].Init.TimeSeg2 = CAN_BS2_4TQ;
    g_can[index].Init.TimeTriggeredMode = DISABLE;
    g_can[index].Init.AutoBusOff = ENABLE;
    g_can[index].Init.AutoWakeUp = DISABLE;
    g_can[index].Init.AutoRetransmission = ENABLE;
    g_can[index].Init.ReceiveFifoLocked = DISABLE;
    g_can[index].Init.TransmitFifoPriority = DISABLE;

    if (HAL_CAN_Init(&g_can[index]) != HAL_OK) {
        NeonRTOS_SyncObjDelete(&CAN_TxDone_Sync[index]);
        NeonRTOS_MsgQDelete(&CAN_RxQueue[index]);
        return hwCAN_HwError;
    }

    CAN_FilterTypeDef filter = {0};

    filter.FilterBank = 0;
    filter.FilterMode = CAN_FILTERMODE_IDMASK;
    filter.FilterScale = CAN_FILTERSCALE_32BIT;
    filter.FilterIdHigh = 0x0000;
    filter.FilterIdLow = 0x0000;
    filter.FilterMaskIdHigh = 0x0000;
    filter.FilterMaskIdLow = 0x0000;
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filter.FilterActivation = ENABLE;

#if defined(CAN_FILTERBANK0)
    filter.SlaveStartFilterBank = 14;
#endif

    if (HAL_CAN_ConfigFilter(&g_can[index], &filter) != HAL_OK) {
        HAL_CAN_DeInit(&g_can[index]);
        NeonRTOS_SyncObjDelete(&CAN_TxDone_Sync[index]);
        NeonRTOS_MsgQDelete(&CAN_RxQueue[index]);
        return hwCAN_HwError;
    }

    if (HAL_CAN_Start(&g_can[index]) != HAL_OK) {
        HAL_CAN_DeInit(&g_can[index]);
        NeonRTOS_SyncObjDelete(&CAN_TxDone_Sync[index]);
        NeonRTOS_MsgQDelete(&CAN_RxQueue[index]);
        return hwCAN_HwError;
    }

    if (HAL_CAN_ActivateNotification(&g_can[index],
        CAN_IT_TX_MAILBOX_EMPTY |
        CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
        HAL_CAN_Stop(&g_can[index]);
        HAL_CAN_DeInit(&g_can[index]);
        NeonRTOS_SyncObjDelete(&CAN_TxDone_Sync[index]);
        NeonRTOS_MsgQDelete(&CAN_RxQueue[index]);
        return hwCAN_HwError;
    }

#if defined(STM32F3_CAN_TX_IRQn)
    HAL_NVIC_SetPriority(STM32F3_CAN_TX_IRQn, CAN_IRQ_NVIC_PRIORITY, CAN_IRQ_NVIC_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(STM32F3_CAN_TX_IRQn);
#endif

#if defined(STM32F3_CAN_RX0_IRQn)
    HAL_NVIC_SetPriority(STM32F3_CAN_RX0_IRQn, CAN_IRQ_NVIC_PRIORITY, CAN_IRQ_NVIC_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(STM32F3_CAN_RX0_IRQn);
#endif

#if defined(STM32F3_CAN_RX1_IRQn)
    HAL_NVIC_SetPriority(STM32F3_CAN_RX1_IRQn, CAN_IRQ_NVIC_PRIORITY, CAN_IRQ_NVIC_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(STM32F3_CAN_RX1_IRQn);
#endif

#if defined(STM32F3_CAN_SCE_IRQn)
    HAL_NVIC_SetPriority(STM32F3_CAN_SCE_IRQn, CAN_IRQ_NVIC_PRIORITY, CAN_IRQ_NVIC_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(STM32F3_CAN_SCE_IRQn);
#endif

    gpio_pin_init_status[tx_hw_pin] = true;
    gpio_pin_init_status[rx_hw_pin] = true;

    CAN_Init_Status[index] = true;

    return hwCAN_OK;
}

/* =========================
 * DeInit
 * ========================= */

hwCAN_OpResult CAN_DeInit(hwCAN_Index index)
{
    if (index != hwCAN_Index_0) {
        return hwCAN_InvalidParameter;
    }

    if (!CAN_Init_Status[index]) {
        return hwCAN_OK;
    }

    hwGPIO_Pin tx_hw_pin = CAN_Pin_Def_Table[index][CAN_Index_Map_Alt[index]].tx_pin;
    hwGPIO_Pin rx_hw_pin = CAN_Pin_Def_Table[index][CAN_Index_Map_Alt[index]].rx_pin;

    GPIO_TypeDef *tx_soc_base = GPIO_Map_Soc_Base(tx_hw_pin);
    GPIO_TypeDef *rx_soc_base = GPIO_Map_Soc_Base(rx_hw_pin);

    uint16_t tx_soc_pin = GPIO_Map_Soc_Pin(tx_hw_pin);
    uint16_t rx_soc_pin = GPIO_Map_Soc_Pin(rx_hw_pin);

#if defined(STM32F3_CAN_TX_IRQn)
    HAL_NVIC_DisableIRQ(STM32F3_CAN_TX_IRQn);
#endif

#if defined(STM32F3_CAN_RX0_IRQn)
    HAL_NVIC_DisableIRQ(STM32F3_CAN_RX0_IRQn);
#endif

#if defined(STM32F3_CAN_RX1_IRQn)
    HAL_NVIC_DisableIRQ(STM32F3_CAN_RX1_IRQn);
#endif

#if defined(STM32F3_CAN_SCE_IRQn)
    HAL_NVIC_DisableIRQ(STM32F3_CAN_SCE_IRQn);
#endif

    HAL_CAN_Stop(&g_can[index]);
    HAL_CAN_DeInit(&g_can[index]);

    __HAL_RCC_CAN1_CLK_DISABLE();

    if (tx_soc_base != NULL && tx_soc_pin != 0) {
        HAL_GPIO_DeInit(tx_soc_base, tx_soc_pin);
    }

    if (rx_soc_base != NULL && rx_soc_pin != 0) {
        HAL_GPIO_DeInit(rx_soc_base, rx_soc_pin);
    }

    NeonRTOS_SyncObjDelete(&CAN_TxDone_Sync[index]);
    NeonRTOS_MsgQDelete(&CAN_RxQueue[index]);

    gpio_pin_init_status[tx_hw_pin] = false;
    gpio_pin_init_status[rx_hw_pin] = false;

    CAN_Init_Status[index] = false;

    return hwCAN_OK;
}

/* =========================
 * Read / Write
 * ========================= */

hwCAN_OpResult CAN_Read(hwCAN_Index index, uint8_t *buf, uint32_t timeout)
{
    if (index != hwCAN_Index_0 || buf == NULL) {
        return hwCAN_InvalidParameter;
    }

    if (!CAN_Init_Status[index]) {
        return hwCAN_NotInit;
    }

    if (NeonRTOS_QueueReceive(&CAN_RxQueue[index], buf, timeout) != NeonRTOS_OK) {
        return hwCAN_Timeout;
    }

    return hwCAN_OK;
}

hwCAN_OpResult CAN_Write(hwCAN_Index index, uint32_t id, uint8_t *data, uint8_t len, uint32_t timeout)
{
    if (index != hwCAN_Index_0 || data == NULL || len > 8) {
        return hwCAN_InvalidParameter;
    }

    if (!CAN_Init_Status[index]) {
        return hwCAN_NotInit;
    }

    CAN_TxHeaderTypeDef hdr = {
        .StdId = id,
        .IDE = CAN_ID_STD,
        .RTR = CAN_RTR_DATA,
        .DLC = len,
        .TransmitGlobalTime = DISABLE
    };

    uint32_t mailbox;

    if (HAL_CAN_AddTxMessage(&g_can[index], &hdr, data, &mailbox) != HAL_OK) {
        return hwCAN_HwError;
    }

    if (NeonRTOS_SyncObjWait(&CAN_TxDone_Sync[index], timeout) != NeonRTOS_OK) {
        return hwCAN_Timeout;
    }

    return hwCAN_OK;
}

bool CAN_isInit(hwCAN_Index index)
{
    if (index >= hwCAN_Index_MAX) {
        return false;
    }

    return CAN_Init_Status[index];
}

#endif /* CAN1_BASE */

#endif /* STM32F3 */