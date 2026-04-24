#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "soc.h"

#include "NeonRTOS.h"

#include "SPI/SPI_Master.h"

#ifdef STM32F3

#include "SPI/SPI_Pin.h"
#include "GPIO/Device/GPIO_STM32.h"

#define SPI_MASTER_SYNC_TIMEOUT             100
#define SPI_MASTER_MUTEX_ACCESS_TIMEOUT     500

#define SPI_IRQ_NVIC_PRIORITY 5
#define SPI_IRQ_NVIC_SUB_PRIORITY 0

NeonRTOS_LockObj_t Spi_Master_Access_Mutex[hwSPI_Index_MAX];
NeonRTOS_SyncObj_t Spi_Master_Send_SyncHandle[hwSPI_Index_MAX];
NeonRTOS_SyncObj_t Spi_Master_Recv_SyncHandle[hwSPI_Index_MAX];

#define SPI_MASTER_MUTEX_LOCK(a, b)  if (NeonRTOS_LockObjLock(&Spi_Master_Access_Mutex[a], b)!=NeonRTOS_OK) {return hwSPI_MutexTimeout;}
#define SPI_MASTER_MUTEX_UNLOCK(a)   NeonRTOS_LockObjUnlock(&Spi_Master_Access_Mutex[a]);

static bool Spi_Master_Init_Status[hwSPI_Index_MAX] = {false};
static bool Spi_Master_Use_CS[hwSPI_Index_MAX] = {false};

static SPI_HandleTypeDef g_spi[hwSPI_Index_MAX];

static const uint32_t Spi_Master_Baudrate_Prescaler_Table[] = {
    SPI_BAUDRATEPRESCALER_2,
    SPI_BAUDRATEPRESCALER_4,
    SPI_BAUDRATEPRESCALER_8,
    SPI_BAUDRATEPRESCALER_16,
    SPI_BAUDRATEPRESCALER_32,
    SPI_BAUDRATEPRESCALER_64,
    SPI_BAUDRATEPRESCALER_128,
    SPI_BAUDRATEPRESCALER_256
};

uint32_t STM32_SPI_GetAF(hwSPI_Index spi, hwGPIO_Pin pin)
{
    for (size_t i = 0; i < sizeof(SPI_Pin_AF_Map) / sizeof(SPI_Pin_AF_Map[0]); i++) {
        if (SPI_Pin_AF_Map[i].spi == spi &&
            SPI_Pin_AF_Map[i].pin == pin) {
            return SPI_Pin_AF_Map[i].af;
        }
    }

    return 0;
}

static hwSPI_Index SPI_IndexFromHandle(SPI_HandleTypeDef *hspi)
{
    for (int i = 0; i < hwSPI_Index_MAX; i++) {
        if (&g_spi[i] == hspi) {
            return (hwSPI_Index)i;
        }
    }

    return hwSPI_Index_MAX;
}

SPI_TypeDef *SPI_Map_Soc_Base(hwSPI_Index index)
{
    switch(index)
    {
#if defined(SPI1_BASE)
        case hwSPI_Index_0:
            return SPI1;
#endif

#if defined(SPI2_BASE)
        case hwSPI_Index_1:
            return SPI2;
#endif

#if defined(SPI3_BASE)
        case hwSPI_Index_2:
            return SPI3;
#endif

        default:
            break;
    }

    return NULL;
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
    hwSPI_Index idx = SPI_IndexFromHandle(hspi);

    if(idx >= hwSPI_Index_MAX) {
        return;
    }

    NeonRTOS_SyncObjSignalFromISR(&Spi_Master_Recv_SyncHandle[idx]);
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    hwSPI_Index idx = SPI_IndexFromHandle(hspi);

    if(idx >= hwSPI_Index_MAX) {
        return;
    }

    NeonRTOS_SyncObjSignalFromISR(&Spi_Master_Send_SyncHandle[idx]);
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    hwSPI_Index idx = SPI_IndexFromHandle(hspi);

    if(idx >= hwSPI_Index_MAX) {
        return;
    }

    NeonRTOS_SyncObjSignalFromISR(&Spi_Master_Send_SyncHandle[idx]);
    NeonRTOS_SyncObjSignalFromISR(&Spi_Master_Recv_SyncHandle[idx]);
}

static void SPI_HAL_IRQHandler(hwSPI_Index index)
{
    if(index < hwSPI_Index_MAX) {
        HAL_SPI_IRQHandler(&g_spi[index]);
    }
}

#if defined(SPI1_BASE)
void SPI1_IRQHandler(void)
{
    SPI_HAL_IRQHandler(hwSPI_Index_0);
}
#endif

#if defined(SPI2_BASE)
void SPI2_IRQHandler(void)
{
    SPI_HAL_IRQHandler(hwSPI_Index_1);
}
#endif

#if defined(SPI3_BASE)
void SPI3_IRQHandler(void)
{
    SPI_HAL_IRQHandler(hwSPI_Index_2);
}
#endif

static uint32_t SPI_Master_Get_Clock_Freq(hwSPI_Index index)
{
    switch(index)
    {
#if defined(SPI1_BASE)
        case hwSPI_Index_0:
            return HAL_RCC_GetPCLK2Freq();
#endif

#if defined(SPI2_BASE)
        case hwSPI_Index_1:
#endif
#if defined(SPI3_BASE)
        case hwSPI_Index_2:
#endif
            return HAL_RCC_GetPCLK1Freq();

        default:
            return 0;
    }
}

static uint32_t SPI_Master_Get_BaudratePrescaler(hwSPI_Index index, uint32_t clock_rate_hz)
{
    uint32_t spi_clk = SPI_Master_Get_Clock_Freq(index);

    if(spi_clk == 0 || clock_rate_hz == 0) {
        return SPI_BAUDRATEPRESCALER_256;
    }

    uint8_t prescaler_rank = 0;
    uint8_t last_index = (sizeof(Spi_Master_Baudrate_Prescaler_Table) /
                          sizeof(Spi_Master_Baudrate_Prescaler_Table[0])) - 1;

    uint32_t spi_hz = spi_clk / 2;

    while((spi_hz > clock_rate_hz) && (prescaler_rank < last_index)) {
        spi_hz /= 2;
        prescaler_rank++;
    }

    return Spi_Master_Baudrate_Prescaler_Table[prescaler_rank];
}

hwSPI_OpResult SPI_Master_Init(hwSPI_Index index, uint32_t clock_rate_hz, hwSPI_OpMode opMode)
{
    if(opMode >= hwSPI_OpMode_MAX) {
        return hwSPI_InvalidParameter;
    }

    if(index >= hwSPI_Index_MAX) {
        return hwSPI_InvalidParameter;
    }

    if(Spi_Master_Init_Status[index] == true) {
        return hwSPI_OK;
    }

    SPI_Pin_Def pin_def = SPI_Pin_Def_Table[index][SPI_Index_Map_Alt[index]];

    GPIO_TypeDef *miso_soc_base = GPIO_Map_Soc_Base(pin_def.miso_pin);
    GPIO_TypeDef *mosi_soc_base = GPIO_Map_Soc_Base(pin_def.mosi_pin);
    GPIO_TypeDef *sclk_soc_base = GPIO_Map_Soc_Base(pin_def.sclk_pin);

    uint16_t miso_soc_pin = GPIO_Map_Soc_Pin(pin_def.miso_pin);
    uint16_t mosi_soc_pin = GPIO_Map_Soc_Pin(pin_def.mosi_pin);
    uint16_t sclk_soc_pin = GPIO_Map_Soc_Pin(pin_def.sclk_pin);

    GPIO_TypeDef *cs_soc_base = NULL;
    uint16_t cs_soc_pin = 0;

    if(miso_soc_pin == 0 || miso_soc_base == NULL ||
       mosi_soc_pin == 0 || mosi_soc_base == NULL ||
       sclk_soc_pin == 0 || sclk_soc_base == NULL) {
        return hwSPI_InvalidParameter;
    }

    if(pin_def.cs_pin != hwGPIO_Pin_NC) {
        cs_soc_base = GPIO_Map_Soc_Base(pin_def.cs_pin);
        cs_soc_pin = GPIO_Map_Soc_Pin(pin_def.cs_pin);

        if(cs_soc_pin == 0 || cs_soc_base == NULL) {
            return hwSPI_InvalidParameter;
        }
    }

    uint32_t mosi_af = STM32_SPI_GetAF(index, pin_def.mosi_pin);
    uint32_t miso_af = STM32_SPI_GetAF(index, pin_def.miso_pin);
    uint32_t sclk_af = STM32_SPI_GetAF(index, pin_def.sclk_pin);
    uint32_t cs_af = 0;

    if(mosi_af == 0 || miso_af == 0 || sclk_af == 0) {
        return hwSPI_InvalidParameter;
    }

    if(pin_def.cs_pin != hwGPIO_Pin_NC) {
        cs_af = STM32_SPI_GetAF(index, pin_def.cs_pin);

        if(cs_af == 0) {
            return hwSPI_InvalidParameter;
        }
    }

    SPI_TypeDef *spi_soc_base = SPI_Map_Soc_Base(index);

    if(spi_soc_base == NULL) {
        return hwSPI_InvalidParameter;
    }

    if(NeonRTOS_SyncObjCreate(&Spi_Master_Send_SyncHandle[index]) != NeonRTOS_OK) {
        return hwSPI_MemoryError;
    }

    if(NeonRTOS_SyncObjCreate(&Spi_Master_Recv_SyncHandle[index]) != NeonRTOS_OK) {
        NeonRTOS_SyncObjDelete(&Spi_Master_Send_SyncHandle[index]);
        return hwSPI_MemoryError;
    }

    if(NeonRTOS_LockObjCreate(&Spi_Master_Access_Mutex[index]) != NeonRTOS_OK) {
        NeonRTOS_SyncObjDelete(&Spi_Master_Send_SyncHandle[index]);
        NeonRTOS_SyncObjDelete(&Spi_Master_Recv_SyncHandle[index]);
        return hwSPI_MemoryError;
    }

    NeonRTOS_LockObjUnlock(&Spi_Master_Access_Mutex[index]);

    GPIO_Enable_RCC_Clock(miso_soc_base);
    GPIO_Enable_RCC_Clock(mosi_soc_base);
    GPIO_Enable_RCC_Clock(sclk_soc_base);

    if(pin_def.cs_pin != hwGPIO_Pin_NC) {
        GPIO_Enable_RCC_Clock(cs_soc_base);
    }

    switch(index)
    {
#if defined(SPI1_BASE)
        case hwSPI_Index_0:
            __HAL_RCC_SPI1_CLK_ENABLE();
            break;
#endif

#if defined(SPI2_BASE)
        case hwSPI_Index_1:
            __HAL_RCC_SPI2_CLK_ENABLE();
            break;
#endif

#if defined(SPI3_BASE)
        case hwSPI_Index_2:
            __HAL_RCC_SPI3_CLK_ENABLE();
            break;
#endif

        default:
            break;
    }

    GPIO_InitTypeDef g_spi_miso = {0};
    g_spi_miso.Pin       = miso_soc_pin;
    g_spi_miso.Mode      = GPIO_MODE_AF_PP;
    g_spi_miso.Pull      = GPIO_NOPULL;
    g_spi_miso.Speed     = GPIO_SPEED_FREQ_HIGH;
    g_spi_miso.Alternate = miso_af;
    HAL_GPIO_Init(miso_soc_base, &g_spi_miso);

    GPIO_InitTypeDef g_spi_mosi = {0};
    g_spi_mosi.Pin       = mosi_soc_pin;
    g_spi_mosi.Mode      = GPIO_MODE_AF_PP;
    g_spi_mosi.Pull      = GPIO_NOPULL;
    g_spi_mosi.Speed     = GPIO_SPEED_FREQ_HIGH;
    g_spi_mosi.Alternate = mosi_af;
    HAL_GPIO_Init(mosi_soc_base, &g_spi_mosi);

    GPIO_InitTypeDef g_spi_sclk = {0};
    g_spi_sclk.Pin       = sclk_soc_pin;
    g_spi_sclk.Mode      = GPIO_MODE_AF_PP;
    g_spi_sclk.Pull      = GPIO_NOPULL;
    g_spi_sclk.Speed     = GPIO_SPEED_FREQ_HIGH;
    g_spi_sclk.Alternate = sclk_af;
    HAL_GPIO_Init(sclk_soc_base, &g_spi_sclk);

    if (SPI_Pin_Def_Table[index][SPI_Index_Map_Alt[index]].cs_pin != hwGPIO_Pin_NC)
    {
        GPIO_InitTypeDef g_spi_cs = {0};
        g_spi_cs.Pin       = cs_soc_pin;
        g_spi_cs.Mode      = GPIO_MODE_AF_PP;
        g_spi_cs.Pull      = GPIO_NOPULL;
        g_spi_cs.Speed     = GPIO_SPEED_FREQ_HIGH;
        g_spi_cs.Alternate = cs_af;
        HAL_GPIO_Init(cs_soc_base, &g_spi_cs);
    }

    g_spi[index].Instance = spi_soc_base;

    g_spi[index].Init.Mode = SPI_MODE_MASTER;
    g_spi[index].Init.Direction = SPI_DIRECTION_2LINES;
    g_spi[index].Init.DataSize = SPI_DATASIZE_8BIT;

    switch(opMode)
    {
        case hwSPI_OpMode_Polarity0_Phase0:
            g_spi[index].Init.CLKPolarity = SPI_POLARITY_LOW;
            g_spi[index].Init.CLKPhase = SPI_PHASE_1EDGE;
            break;

        case hwSPI_OpMode_Polarity0_Phase1:
            g_spi[index].Init.CLKPolarity = SPI_POLARITY_LOW;
            g_spi[index].Init.CLKPhase = SPI_PHASE_2EDGE;
            break;

        case hwSPI_OpMode_Polarity1_Phase0:
            g_spi[index].Init.CLKPolarity = SPI_POLARITY_HIGH;
            g_spi[index].Init.CLKPhase = SPI_PHASE_1EDGE;
            break;

        case hwSPI_OpMode_Polarity1_Phase1:
            g_spi[index].Init.CLKPolarity = SPI_POLARITY_HIGH;
            g_spi[index].Init.CLKPhase = SPI_PHASE_2EDGE;
            break;

        default:
            return hwSPI_InvalidParameter;
    }

    g_spi[index].Init.NSS = SPI_NSS_SOFT;
    g_spi[index].Init.FirstBit = SPI_FIRSTBIT_MSB;
    g_spi[index].Init.TIMode = SPI_TIMODE_DISABLE;
    g_spi[index].Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    g_spi[index].Init.BaudRatePrescaler = SPI_Master_Get_BaudratePrescaler(index, clock_rate_hz);

#if defined(SPI_CRC_LENGTH_DATASIZE)
    g_spi[index].Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
#endif

#if defined(SPI_NSS_PULSE_DISABLE)
    g_spi[index].Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
#endif

    if(HAL_SPI_Init(&g_spi[index]) != HAL_OK) {
        NeonRTOS_LockObjDelete(&Spi_Master_Access_Mutex[index]);
        NeonRTOS_SyncObjDelete(&Spi_Master_Send_SyncHandle[index]);
        NeonRTOS_SyncObjDelete(&Spi_Master_Recv_SyncHandle[index]);
        return hwSPI_HwError;
    }

    switch(index)
    {
#if defined(SPI1_BASE)
        case hwSPI_Index_0:
            HAL_NVIC_SetPriority(SPI1_IRQn, SPI_IRQ_NVIC_PRIORITY, SPI_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(SPI1_IRQn);
            break;
#endif

#if defined(SPI2_BASE)
        case hwSPI_Index_1:
            HAL_NVIC_SetPriority(SPI2_IRQn, SPI_IRQ_NVIC_PRIORITY, SPI_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(SPI2_IRQn);
            break;
#endif

#if defined(SPI3_BASE)
        case hwSPI_Index_2:
            HAL_NVIC_SetPriority(SPI3_IRQn, SPI_IRQ_NVIC_PRIORITY, SPI_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(SPI3_IRQn);
            break;
#endif

        default:
            break;
    }

    gpio_pin_init_status[pin_def.miso_pin] = true;
    gpio_pin_init_status[pin_def.mosi_pin] = true;
    gpio_pin_init_status[pin_def.sclk_pin] = true;

    if(pin_def.cs_pin != hwGPIO_Pin_NC) {
        gpio_pin_init_status[pin_def.cs_pin] = true;
        Spi_Master_Use_CS[index] = true;
    }

    Spi_Master_Init_Status[index] = true;

    return hwSPI_OK;
}

hwSPI_OpResult SPI_Master_DeInit(hwSPI_Index index)
{
    if(index >= hwSPI_Index_MAX) {
        return hwSPI_InvalidParameter;
    }

    if(Spi_Master_Init_Status[index] == false) {
        return hwSPI_OK;
    }

    SPI_Pin_Def pin_def = SPI_Pin_Def_Table[index][SPI_Index_Map_Alt[index]];

    GPIO_TypeDef *miso_soc_base = GPIO_Map_Soc_Base(pin_def.miso_pin);
    GPIO_TypeDef *mosi_soc_base = GPIO_Map_Soc_Base(pin_def.mosi_pin);
    GPIO_TypeDef *sclk_soc_base = GPIO_Map_Soc_Base(pin_def.sclk_pin);

    uint16_t miso_soc_pin = GPIO_Map_Soc_Pin(pin_def.miso_pin);
    uint16_t mosi_soc_pin = GPIO_Map_Soc_Pin(pin_def.mosi_pin);
    uint16_t sclk_soc_pin = GPIO_Map_Soc_Pin(pin_def.sclk_pin);

    GPIO_TypeDef *cs_soc_base = NULL;
    uint16_t cs_soc_pin = 0;

    if(miso_soc_pin == 0 || miso_soc_base == NULL ||
       mosi_soc_pin == 0 || mosi_soc_base == NULL ||
       sclk_soc_pin == 0 || sclk_soc_base == NULL) {
        return hwSPI_InvalidParameter;
    }

    if(Spi_Master_Use_CS[index] == true) {
        cs_soc_base = GPIO_Map_Soc_Base(pin_def.cs_pin);
        cs_soc_pin = GPIO_Map_Soc_Pin(pin_def.cs_pin);

        if(cs_soc_pin == 0 || cs_soc_base == NULL) {
            return hwSPI_InvalidParameter;
        }
    }

    switch(index)
    {
#if defined(SPI1_BASE)
        case hwSPI_Index_0:
            HAL_NVIC_DisableIRQ(SPI1_IRQn);
            break;
#endif

#if defined(SPI2_BASE)
        case hwSPI_Index_1:
            HAL_NVIC_DisableIRQ(SPI2_IRQn);
            break;
#endif

#if defined(SPI3_BASE)
        case hwSPI_Index_2:
            HAL_NVIC_DisableIRQ(SPI3_IRQn);
            break;
#endif

        default:
            break;
    }

    HAL_SPI_DeInit(&g_spi[index]);

    HAL_GPIO_DeInit(miso_soc_base, miso_soc_pin);
    HAL_GPIO_DeInit(mosi_soc_base, mosi_soc_pin);
    HAL_GPIO_DeInit(sclk_soc_base, sclk_soc_pin);

    if(Spi_Master_Use_CS[index] == true) {
        HAL_GPIO_DeInit(cs_soc_base, cs_soc_pin);
    }

    switch(index)
    {
#if defined(SPI1_BASE)
        case hwSPI_Index_0:
            __HAL_RCC_SPI1_CLK_DISABLE();
            break;
#endif

#if defined(SPI2_BASE)
        case hwSPI_Index_1:
            __HAL_RCC_SPI2_CLK_DISABLE();
            break;
#endif

#if defined(SPI3_BASE)
        case hwSPI_Index_2:
            __HAL_RCC_SPI3_CLK_DISABLE();
            break;
#endif

        default:
            break;
    }

    NeonRTOS_LockObjDelete(&Spi_Master_Access_Mutex[index]);
    NeonRTOS_SyncObjDelete(&Spi_Master_Send_SyncHandle[index]);
    NeonRTOS_SyncObjDelete(&Spi_Master_Recv_SyncHandle[index]);

    gpio_pin_init_status[pin_def.miso_pin] = false;
    gpio_pin_init_status[pin_def.mosi_pin] = false;
    gpio_pin_init_status[pin_def.sclk_pin] = false;

    if(Spi_Master_Use_CS[index] == true) {
        gpio_pin_init_status[pin_def.cs_pin] = false;
        Spi_Master_Use_CS[index] = false;
    }

    Spi_Master_Init_Status[index] = false;

    return hwSPI_OK;
}

hwSPI_OpResult SPI_Change_Frequency(hwSPI_Index index, uint32_t clock_rate_hz)
{
    if(index >= hwSPI_Index_MAX) {
        return hwSPI_InvalidParameter;
    }

    if(Spi_Master_Init_Status[index] == false) {
        return hwSPI_NotInit;
    }

    if(SPI_Map_Soc_Base(index) == NULL) {
        return hwSPI_InvalidParameter;
    }

    SPI_MASTER_MUTEX_LOCK(index, SPI_MASTER_MUTEX_ACCESS_TIMEOUT);

    g_spi[index].Init.BaudRatePrescaler = SPI_Master_Get_BaudratePrescaler(index, clock_rate_hz);

    if(HAL_SPI_Init(&g_spi[index]) != HAL_OK) {
        SPI_MASTER_MUTEX_UNLOCK(index);
        return hwSPI_HwError;
    }

    SPI_MASTER_MUTEX_UNLOCK(index);

    return hwSPI_OK;
}

hwSPI_OpResult SPI_Change_Mode(hwSPI_Index index, hwSPI_OpMode opMode)
{
    if(opMode >= hwSPI_OpMode_MAX) {
        return hwSPI_InvalidParameter;
    }

    if(index >= hwSPI_Index_MAX) {
        return hwSPI_InvalidParameter;
    }

    if(Spi_Master_Init_Status[index] == false) {
        return hwSPI_NotInit;
    }

    if(SPI_Map_Soc_Base(index) == NULL) {
        return hwSPI_InvalidParameter;
    }

    SPI_MASTER_MUTEX_LOCK(index, SPI_MASTER_MUTEX_ACCESS_TIMEOUT);

    switch(opMode)
    {
        case hwSPI_OpMode_Polarity0_Phase0:
            g_spi[index].Init.CLKPolarity = SPI_POLARITY_LOW;
            g_spi[index].Init.CLKPhase = SPI_PHASE_1EDGE;
            break;

        case hwSPI_OpMode_Polarity0_Phase1:
            g_spi[index].Init.CLKPolarity = SPI_POLARITY_LOW;
            g_spi[index].Init.CLKPhase = SPI_PHASE_2EDGE;
            break;

        case hwSPI_OpMode_Polarity1_Phase0:
            g_spi[index].Init.CLKPolarity = SPI_POLARITY_HIGH;
            g_spi[index].Init.CLKPhase = SPI_PHASE_1EDGE;
            break;

        case hwSPI_OpMode_Polarity1_Phase1:
            g_spi[index].Init.CLKPolarity = SPI_POLARITY_HIGH;
            g_spi[index].Init.CLKPhase = SPI_PHASE_2EDGE;
            break;

        default:
            SPI_MASTER_MUTEX_UNLOCK(index);
            return hwSPI_InvalidParameter;
    }

    if(HAL_SPI_Init(&g_spi[index]) != HAL_OK) {
        SPI_MASTER_MUTEX_UNLOCK(index);
        return hwSPI_HwError;
    }

    SPI_MASTER_MUTEX_UNLOCK(index);

    return hwSPI_OK;
}

hwSPI_OpResult SPI_Master_WriteByte(hwSPI_Index index, uint8_t dat)
{
    if(index >= hwSPI_Index_MAX) {
        return hwSPI_InvalidParameter;
    }

    if(Spi_Master_Init_Status[index] == false) {
        return hwSPI_NotInit;
    }

    SPI_MASTER_MUTEX_LOCK(index, SPI_MASTER_MUTEX_ACCESS_TIMEOUT);

    NeonRTOS_SyncObjClear(&Spi_Master_Send_SyncHandle[index]);

    if(HAL_SPI_Transmit_IT(&g_spi[index], &dat, 1) != HAL_OK) {
        SPI_MASTER_MUTEX_UNLOCK(index);
        return hwSPI_HwError;
    }

    if(NeonRTOS_SyncObjWait(&Spi_Master_Send_SyncHandle[index], SPI_MASTER_SYNC_TIMEOUT) != NeonRTOS_OK) {
        SPI_MASTER_MUTEX_UNLOCK(index);
        return hwSPI_SlaveTimeout;
    }

    SPI_MASTER_MUTEX_UNLOCK(index);

    return hwSPI_OK;
}

hwSPI_OpResult SPI_Master_ReadByte(hwSPI_Index index, uint8_t* dat)
{
    if(index >= hwSPI_Index_MAX) {
        return hwSPI_InvalidParameter;
    }

    if(dat == NULL) {
        return hwSPI_InvalidParameter;
    }

    if(Spi_Master_Init_Status[index] == false) {
        return hwSPI_NotInit;
    }

    SPI_MASTER_MUTEX_LOCK(index, SPI_MASTER_MUTEX_ACCESS_TIMEOUT);

    NeonRTOS_SyncObjClear(&Spi_Master_Recv_SyncHandle[index]);

    if(HAL_SPI_Receive_IT(&g_spi[index], dat, 1) != HAL_OK) {
        SPI_MASTER_MUTEX_UNLOCK(index);
        return hwSPI_HwError;
    }

    if(NeonRTOS_SyncObjWait(&Spi_Master_Recv_SyncHandle[index], SPI_MASTER_SYNC_TIMEOUT) != NeonRTOS_OK) {
        SPI_MASTER_MUTEX_UNLOCK(index);
        return hwSPI_SlaveTimeout;
    }

    SPI_MASTER_MUTEX_UNLOCK(index);

    return hwSPI_OK;
}

hwSPI_OpResult SPI_Master_TransferByte(hwSPI_Index index, uint8_t wr_dat, uint8_t* rd_dat)
{
    if(index >= hwSPI_Index_MAX) {
        return hwSPI_InvalidParameter;
    }

    if(rd_dat == NULL) {
        return hwSPI_InvalidParameter;
    }

    if(Spi_Master_Init_Status[index] == false) {
        return hwSPI_NotInit;
    }

    SPI_MASTER_MUTEX_LOCK(index, SPI_MASTER_MUTEX_ACCESS_TIMEOUT);

    NeonRTOS_SyncObjClear(&Spi_Master_Send_SyncHandle[index]);
    NeonRTOS_SyncObjClear(&Spi_Master_Recv_SyncHandle[index]);

    if(HAL_SPI_TransmitReceive_IT(&g_spi[index], &wr_dat, rd_dat, 1) != HAL_OK) {
        SPI_MASTER_MUTEX_UNLOCK(index);
        return hwSPI_HwError;
    }

    if(NeonRTOS_SyncObjWait(&Spi_Master_Send_SyncHandle[index], SPI_MASTER_SYNC_TIMEOUT) != NeonRTOS_OK) {
        SPI_MASTER_MUTEX_UNLOCK(index);
        return hwSPI_SlaveTimeout;
    }

    if(NeonRTOS_SyncObjWait(&Spi_Master_Recv_SyncHandle[index], SPI_MASTER_SYNC_TIMEOUT) != NeonRTOS_OK) {
        SPI_MASTER_MUTEX_UNLOCK(index);
        return hwSPI_SlaveTimeout;
    }

    SPI_MASTER_MUTEX_UNLOCK(index);

    return hwSPI_OK;
}

hwSPI_OpResult SPI_Master_DummyByte(hwSPI_Index index)
{
    if(index >= hwSPI_Index_MAX) {
        return hwSPI_InvalidParameter;
    }

    if(Spi_Master_Init_Status[index] == false) {
        return hwSPI_NotInit;
    }

    return SPI_Master_WriteByte(index, 0);
}

#endif // STM32F3