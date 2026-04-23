#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "soc.h"
#include "NeonRTOS.h"
#include "CAN/CAN.h"

#ifdef STM32F0

#if defined(CAN1_BASE)

#include "CAN/CAN_Pin.h"
#include "GPIO/Device/GPIO_STM32.h"

#define CAN_IRQ_NVIC_PRIORITY      5
#define CAN_IRQ_NVIC_SUB_PRIORITY  0

static bool CAN_Init_Status[hwCAN_Index_MAX];
static NeonRTOS_SyncObj_t CAN_TxDone_Sync[hwCAN_Index_MAX];
static NeonRTOS_MsgQ_t    CAN_RxQueue[hwCAN_Index_MAX];

static CAN_HandleTypeDef g_can[hwCAN_Index_MAX];

static uint32_t CAN_CalcPrescaler(uint32_t can_clk, uint32_t bitrate)
{
    const uint32_t tq = 14; // 1 + 11 + 2

    uint32_t prescaler = can_clk / (bitrate * tq);

    if (prescaler == 0) prescaler = 1;
    if (prescaler > 1024) prescaler = 1024;

    return prescaler;
}

uint32_t STM32_CAN_GetAF(hwCAN_Index can, hwGPIO_Pin pin)
{
    for (size_t i = 0; i < sizeof(CAN_Pin_AF_Map) / sizeof(CAN_Pin_AF_Map[0]); i++) {
        if (CAN_Pin_AF_Map[i].can == can &&
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

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
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

static void CAN_HAL_IRQHandler(hwCAN_Index index)
{
    HAL_CAN_IRQHandler(&g_can[index]);
}

void CEC_CAN_IRQHandler(void)
{
    CAN_HAL_IRQHandler(hwCAN_Index_0);
}

hwCAN_OpResult CAN_Init(hwCAN_Index index)
{
    if (index >= hwCAN_Index_MAX) {
        return hwCAN_InvalidParameter;
    }

    if (index != hwCAN_Index_0) {
        return hwCAN_InvalidParameter;
    }

    if (CAN_Init_Status[index]) {
        return hwCAN_OK;
    }

    GPIO_TypeDef *tx_soc_base =
        GPIO_Map_Soc_Base(CAN_Pin_Def_Table[index][CAN_Index_Map_Alt[index]].tx_pin);
    GPIO_TypeDef *rx_soc_base =
        GPIO_Map_Soc_Base(CAN_Pin_Def_Table[index][CAN_Index_Map_Alt[index]].rx_pin);

    uint16_t tx_soc_pin =
        GPIO_Map_Soc_Pin(CAN_Pin_Def_Table[index][CAN_Index_Map_Alt[index]].tx_pin);
    uint16_t rx_soc_pin =
        GPIO_Map_Soc_Pin(CAN_Pin_Def_Table[index][CAN_Index_Map_Alt[index]].rx_pin);

    if (tx_soc_pin == 0 || tx_soc_base == NULL ||
        rx_soc_pin == 0 || rx_soc_base == NULL) {
        return hwCAN_InvalidParameter;
    }

    uint32_t tx_af =
        STM32_CAN_GetAF(index, CAN_Pin_Def_Table[index][CAN_Index_Map_Alt[index]].tx_pin);
    uint32_t rx_af =
        STM32_CAN_GetAF(index, CAN_Pin_Def_Table[index][CAN_Index_Map_Alt[index]].rx_pin);

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

    GPIO_InitTypeDef g_can_tx = {0};
    g_can_tx.Pin       = tx_soc_pin;
    g_can_tx.Mode      = GPIO_MODE_AF_PP;
    g_can_tx.Pull      = GPIO_NOPULL;
    g_can_tx.Speed     = GPIO_SPEED_FREQ_HIGH;
    g_can_tx.Alternate = tx_af;
    HAL_GPIO_Init(tx_soc_base, &g_can_tx);

    GPIO_InitTypeDef g_can_rx = {0};
    g_can_rx.Pin       = rx_soc_pin;
    g_can_rx.Mode      = GPIO_MODE_AF_PP;
    g_can_rx.Pull      = GPIO_NOPULL;
    g_can_rx.Speed     = GPIO_SPEED_FREQ_HIGH;
    g_can_rx.Alternate = rx_af;
    HAL_GPIO_Init(rx_soc_base, &g_can_rx);

    __HAL_RCC_CAN1_CLK_ENABLE();

    uint32_t can_clk = HAL_RCC_GetPCLK1Freq();

    g_can[index].Instance = can_soc_base;
    
    g_can[index].Init.Prescaler = CAN_CalcPrescaler(can_clk, 500000); // 500kbps
    g_can[index].Init.Mode = CAN_MODE_NORMAL;
    g_can[index].Init.SyncJumpWidth = CAN_SJW_1TQ;
    g_can[index].Init.TimeSeg1 = CAN_BS1_11TQ;
    g_can[index].Init.TimeSeg2 = CAN_BS2_2TQ;
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
        CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
        HAL_CAN_Stop(&g_can[index]);
        HAL_CAN_DeInit(&g_can[index]);
        NeonRTOS_SyncObjDelete(&CAN_TxDone_Sync[index]);
        NeonRTOS_MsgQDelete(&CAN_RxQueue[index]);
        return hwCAN_HwError;
    }

    HAL_NVIC_SetPriority(CEC_CAN_IRQn, CAN_IRQ_NVIC_PRIORITY, CAN_IRQ_NVIC_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(CEC_CAN_IRQn);

    gpio_pin_init_status[CAN_Pin_Def_Table[index][CAN_Index_Map_Alt[index]].tx_pin] = true;
    gpio_pin_init_status[CAN_Pin_Def_Table[index][CAN_Index_Map_Alt[index]].rx_pin] = true;

    CAN_Init_Status[index] = true;

    return hwCAN_OK;
}

hwCAN_OpResult CAN_DeInit(hwCAN_Index index)
{
    if (index >= hwCAN_Index_MAX) {
        return hwCAN_InvalidParameter;
    }

    if (index != hwCAN_Index_0) {
        return hwCAN_InvalidParameter;
    }

    if (CAN_Init_Status[index] == false) {
        return hwCAN_OK;
    }

    GPIO_TypeDef *tx_soc_base =
        GPIO_Map_Soc_Base(CAN_Pin_Def_Table[index][CAN_Index_Map_Alt[index]].tx_pin);
    GPIO_TypeDef *rx_soc_base =
        GPIO_Map_Soc_Base(CAN_Pin_Def_Table[index][CAN_Index_Map_Alt[index]].rx_pin);

    uint16_t tx_soc_pin =
        GPIO_Map_Soc_Pin(CAN_Pin_Def_Table[index][CAN_Index_Map_Alt[index]].tx_pin);
    uint16_t rx_soc_pin =
        GPIO_Map_Soc_Pin(CAN_Pin_Def_Table[index][CAN_Index_Map_Alt[index]].rx_pin);

    if (tx_soc_pin == 0 || tx_soc_base == NULL ||
        rx_soc_pin == 0 || rx_soc_base == NULL) {
        return hwCAN_InvalidParameter;
    }

    HAL_NVIC_DisableIRQ(CEC_CAN_IRQn);

    HAL_CAN_Stop(&g_can[index]);
    HAL_CAN_DeInit(&g_can[index]);

    __HAL_RCC_CAN1_CLK_DISABLE();

    HAL_GPIO_DeInit(tx_soc_base, tx_soc_pin);
    HAL_GPIO_DeInit(rx_soc_base, rx_soc_pin);

    NeonRTOS_SyncObjDelete(&CAN_TxDone_Sync[index]);
    NeonRTOS_MsgQDelete(&CAN_RxQueue[index]);

    gpio_pin_init_status[CAN_Pin_Def_Table[index][CAN_Index_Map_Alt[index]].tx_pin] = false;
    gpio_pin_init_status[CAN_Pin_Def_Table[index][CAN_Index_Map_Alt[index]].rx_pin] = false;

    CAN_Init_Status[index] = false;

    return hwCAN_OK;
}

hwCAN_OpResult CAN_Read(hwCAN_Index index, uint8_t *buf, uint32_t timeout)
{
    if (index >= hwCAN_Index_MAX) {
        return hwCAN_InvalidParameter;
    }

    if (NeonRTOS_QueueReceive(&CAN_RxQueue[index], buf, timeout) != NeonRTOS_OK) {
        return hwCAN_Timeout;
    }

    return hwCAN_OK;
}

hwCAN_OpResult CAN_Write(hwCAN_Index index, uint32_t id, uint8_t *data, uint8_t len, uint32_t timeout)
{
    if (index >= hwCAN_Index_MAX || data == NULL || len > 8) {
        return hwCAN_InvalidParameter;
    }

    CAN_TxHeaderTypeDef hdr = {0};
    hdr.StdId = id;
    hdr.IDE   = CAN_ID_STD;
    hdr.RTR   = CAN_RTR_DATA;
    hdr.DLC   = len;

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

#endif // CAN1_BASE

#endif // STM32F0