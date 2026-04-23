#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "soc.h"

#include "NeonRTOS.h"

#include "I2C/I2C_Master.h"

#ifdef STM32F2

#include "I2C/I2C_Pin.h"

#include "GPIO/GPIO_STM32.h"

#define I2C_IRQ_NVIC_PRIORITY 5
#define I2C_IRQ_NVIC_SUB_PRIORITY 0

#define I2C_MASTER_STANDARD_MODE_CLK_FREQUENCY      100000
#define I2C_MASTER_FAST_MODE_CLK_FREQUENCY          400000

static bool I2C_Master_Init_Status[hwI2C_Index_MAX] = {false};
static hwI2C_Speed_Mode I2C_Clock_Speed_Mode[hwI2C_Index_MAX] = {hwI2C_Standard_Mode};
static NeonRTOS_SyncObj_t I2C_Master_Done_SyncHandle[hwI2C_Index_MAX];

static I2C_HandleTypeDef g_i2c[hwI2C_Index_MAX];

uint32_t STM32_I2C_GetAF(hwI2C_Index I2C, hwGPIO_Pin pin)
{
    for (size_t i = 0; i < sizeof(I2C_Pin_AF_Map)/sizeof(I2C_Pin_AF_Map[0]); i++) {
        if (I2C_Pin_AF_Map[i].i2c == I2C &&
            I2C_Pin_AF_Map[i].pin  == pin)
            return I2C_Pin_AF_Map[i].af;
    }
    return 0;
}

static hwI2C_Index I2C_IndexFromHandle(I2C_HandleTypeDef *hI2C)
{
    for(int i=0;i<hwI2C_Index_MAX;i++){
        if(&g_i2c[i] == hI2C) return (hwI2C_Index)i;
    }
    return hwI2C_Index_MAX;
}

I2C_TypeDef * I2C_Map_Soc_Base(hwI2C_Index index)
{
    switch(index)
    {
        case hwI2C_Index_0: return I2C1;
        case hwI2C_Index_1: return I2C2;
        case hwI2C_Index_2: return I2C3;
        default: break;
    }
    return NULL;
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c_cb)
{
    for (int i = 0; i < hwI2C_Index_MAX; i++)
    {
        if (&g_i2c[i] == hi2c_cb)
        {
            NeonRTOS_SyncObjSignalFromISR(&I2C_Master_Done_SyncHandle[i]);
            break;
        }
    }
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c_cb)
{
    for (int i = 0; i < hwI2C_Index_MAX; i++)
    {
        if (&g_i2c[i] == hi2c_cb)
        {
            NeonRTOS_SyncObjSignalFromISR(&I2C_Master_Done_SyncHandle[i]);
            break;
        }
    }
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c_cb)
{
    for (int i = 0; i < hwI2C_Index_MAX; i++)
    {
        if (&g_i2c[i] == hi2c_cb)
        {
            NeonRTOS_SyncObjSignalFromISR(&I2C_Master_Done_SyncHandle[i]);
            break;
        }
    }
}

// UART IRQ 統一入口
static void I2C_HAL_EV_IRQHandler(hwI2C_Index index)
{
    HAL_I2C_EV_IRQHandler(&g_i2c[index]);
}

// UART IRQ 統一入口
static void I2C_HAL_ER_IRQHandler(hwI2C_Index index)
{
    HAL_I2C_ER_IRQHandler(&g_i2c[index]);
}

void I2C1_EV_IRQHandler(void){ I2C_HAL_EV_IRQHandler(hwI2C_Index_0); }
void I2C1_ER_IRQHandler(void){ I2C_HAL_ER_IRQHandler(hwI2C_Index_0); }
void I2C2_EV_IRQHandler(void){ I2C_HAL_EV_IRQHandler(hwI2C_Index_1); }
void I2C2_ER_IRQHandler(void){ I2C_HAL_ER_IRQHandler(hwI2C_Index_1); }
void I2C3_EV_IRQHandler(void){ I2C_HAL_EV_IRQHandler(hwI2C_Index_2); }
void I2C3_ER_IRQHandler(void){ I2C_HAL_ER_IRQHandler(hwI2C_Index_2); }

hwI2C_OpResult I2C_Master_Init(hwI2C_Index index, hwI2C_Speed_Mode speed_mode)
{
    if(index>=hwI2C_Index_MAX)
    {
        return hwI2C_InvalidParameter;
    }

    if(I2C_Master_Init_Status[index]==true)
    {
        return hwI2C_OK;
    }

    if(speed_mode>=hwI2C_Speed_Mode_MAX)
    {
        return hwI2C_InvalidParameter;
    }
    
    if(speed_mode==hwI2C_High_Speed_Mode)
    {
        return hwI2C_Unsupport;
    }

    GPIO_TypeDef * sda_soc_base = GPIO_Map_Soc_Base(I2C_Pin_Def_Table[index][I2C_Index_Map_Alt[index]].sda_pin);
    GPIO_TypeDef * scl_soc_base = GPIO_Map_Soc_Base(I2C_Pin_Def_Table[index][I2C_Index_Map_Alt[index]].scl_pin);
    uint16_t sda_soc_pin = GPIO_Map_Soc_Pin(I2C_Pin_Def_Table[index][I2C_Index_Map_Alt[index]].sda_pin);
    uint16_t scl_soc_pin = GPIO_Map_Soc_Pin(I2C_Pin_Def_Table[index][I2C_Index_Map_Alt[index]].scl_pin);

    if(sda_soc_pin==0 || sda_soc_base==NULL || scl_soc_pin==0 || scl_soc_base==NULL)
    {
            return hwI2C_InvalidParameter;
    }

    uint32_t sda_af = STM32_UART_GetAF(index, I2C_Pin_Def_Table[index][I2C_Index_Map_Alt[index]].sda_pin);
    uint32_t scl_af = STM32_UART_GetAF(index, I2C_Pin_Def_Table[index][I2C_Index_Map_Alt[index]].scl_pin);

    if(sda_af==0 || scl_af==0)
    {
            return hwI2C_InvalidParameter;
    }

    I2C_TypeDef * i2c_soc_base = I2C_Map_Soc_Base(index);
    if(i2c_soc_base==NULL)
    {
            return hwI2C_InvalidParameter;
    }

    if(NeonRTOS_SyncObjCreate(&I2C_Master_Done_SyncHandle[index])!=NeonRTOS_OK)
    {
            return hwI2C_MemoryError;
    }

    GPIO_Enable_RCC_Clock(sda_soc_base);
    GPIO_Enable_RCC_Clock(scl_soc_base);

    GPIO_InitTypeDef g_i2c_sda = {0};
    g_i2c_sda.Pin       = sda_soc_pin;
    g_i2c_sda.Mode      = GPIO_MODE_AF_PP;
    g_i2c_sda.Pull      = GPIO_NOPULL;
    g_i2c_sda.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    g_i2c_sda.Alternate = sda_af;
    HAL_GPIO_Init(sda_soc_base, &g_i2c_sda);

    GPIO_InitTypeDef g_i2c_scl = {0};
    g_i2c_scl.Pin       = scl_soc_pin;
    g_i2c_scl.Mode      = GPIO_MODE_AF_PP;
    g_i2c_scl.Pull      = GPIO_NOPULL;
    g_i2c_scl.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    g_i2c_scl.Alternate = scl_af;
    HAL_GPIO_Init(scl_soc_base, &g_i2c_scl);

    switch(index)
    {
        case hwI2C_Index_0:
            __HAL_RCC_I2C1_CLK_ENABLE();
            break;
        case hwI2C_Index_1:
            __HAL_RCC_I2C2_CLK_ENABLE();
            break;
        case hwI2C_Index_2:
            __HAL_RCC_I2C3_CLK_ENABLE();
            break;
    }

    g_i2c[index].Instance = i2c_soc_base;
    switch(speed_mode)
    {
        case hwI2C_Standard_Mode:
            g_i2c[index].Init.ClockSpeed = I2C_MASTER_STANDARD_MODE_CLK_FREQUENCY;
            break;
        case hwI2C_Fast_Mode:
            g_i2c[index].Init.ClockSpeed = I2C_MASTER_FAST_MODE_CLK_FREQUENCY;
            break;
    }
    g_i2c[index].Init.OwnAddress1 = 0;
    g_i2c[index].Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    g_i2c[index].Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    g_i2c[index].Init.OwnAddress2 = 0;
    g_i2c[index].Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    g_i2c[index].Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    if(HAL_I2C_Init(&g_i2c[index])!=HAL_OK)
    {
        NeonRTOS_SyncObjDelete(&I2C_Master_Done_SyncHandle[index]);
        return hwI2C_HwError;
    }

    switch(index)
    {       
        case hwI2C_Index_0:
            HAL_NVIC_SetPriority(I2C1_EV_IRQn, I2C_IRQ_NVIC_PRIORITY, I2C_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
            HAL_NVIC_SetPriority(I2C1_ER_IRQn, I2C_IRQ_NVIC_PRIORITY, I2C_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
            break;
        case hwI2C_Index_1:
            HAL_NVIC_SetPriority(I2C2_EV_IRQn, I2C_IRQ_NVIC_PRIORITY, I2C_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
            HAL_NVIC_SetPriority(I2C2_ER_IRQn, I2C_IRQ_NVIC_PRIORITY, I2C_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(I2C2_ER_IRQn);
            break;
        case hwI2C_Index_2:
            HAL_NVIC_SetPriority(I2C3_EV_IRQn, I2C_IRQ_NVIC_PRIORITY, I2C_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(I2C3_EV_IRQn);
            HAL_NVIC_SetPriority(I2C3_ER_IRQn, I2C_IRQ_NVIC_PRIORITY, I2C_IRQ_NVIC_SUB_PRIORITY);
            HAL_NVIC_EnableIRQ(I2C3_ER_IRQn);
            break;
    }

    gpio_pin_init_status[I2C_Pin_Def_Table[index][I2C_Index_Map_Alt[index]].sda_pin] = true;
    gpio_pin_init_status[I2C_Pin_Def_Table[index][I2C_Index_Map_Alt[index]].scl_pin] = true;

    I2C_Master_Init_Status[index] = true;

    return hwI2C_OK;
}

hwI2C_OpResult I2C_Master_DeInit(hwI2C_Index index)
{
    if(index>=hwI2C_Index_MAX)
    {
        return hwI2C_InvalidParameter;
    }

    if(I2C_Master_Init_Status[index]==false)
    {
        return hwI2C_OK;
    }
    
    GPIO_TypeDef * sda_soc_base = GPIO_Map_Soc_Base(I2C_Pin_Def_Table[index][I2C_Index_Map_Alt[index]].sda_pin);
    GPIO_TypeDef * scl_soc_base = GPIO_Map_Soc_Base(I2C_Pin_Def_Table[index][I2C_Index_Map_Alt[index]].scl_pin);
    uint16_t sda_soc_pin = GPIO_Map_Soc_Pin(I2C_Pin_Def_Table[index][I2C_Index_Map_Alt[index]].sda_pin);
    uint16_t scl_soc_pin = GPIO_Map_Soc_Pin(I2C_Pin_Def_Table[index][I2C_Index_Map_Alt[index]].scl_pin);

    if(sda_soc_pin==0 || sda_soc_base==NULL || scl_soc_pin==0 || scl_soc_base==NULL)
    {
            return hwI2C_InvalidParameter;
    }

    switch(index)
    {       
        case hwI2C_Index_0:
            HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);
            HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);
            break;
        case hwI2C_Index_1:
            HAL_NVIC_DisableIRQ(I2C2_EV_IRQn);
            HAL_NVIC_DisableIRQ(I2C2_ER_IRQn);
            break;
        case hwI2C_Index_2:
            HAL_NVIC_DisableIRQ(I2C3_EV_IRQn);
            HAL_NVIC_DisableIRQ(I2C3_ER_IRQn);
            break;
    }

    HAL_I2C_DeInit(&g_i2c[index]);

    switch(index)
    {
        case hwI2C_Index_0:
            __HAL_RCC_I2C1_CLK_DISABLE();
            break;
        case hwI2C_Index_1:
            __HAL_RCC_I2C2_CLK_DISABLE();
            break;
        case hwI2C_Index_2:
            __HAL_RCC_I2C3_CLK_DISABLE();
            break;
    }

    NeonRTOS_SyncObjDelete(&I2C_Master_Done_SyncHandle[index]);

    HAL_GPIO_DeInit(sda_soc_base, sda_soc_pin);
    HAL_GPIO_DeInit(scl_soc_base, scl_soc_pin);

    gpio_pin_init_status[I2C_Pin_Def_Table[index][I2C_Index_Map_Alt[index]].sda_pin] = false;
    gpio_pin_init_status[I2C_Pin_Def_Table[index][I2C_Index_Map_Alt[index]].scl_pin] = false;

    I2C_Master_Init_Status[index] = false;

    return hwI2C_OK;
}


hwI2C_OpResult I2C_Master_Reset(hwI2C_Index index)
{
    if(index>=hwI2C_Index_MAX)
    {
        return hwI2C_InvalidParameter;
    }
    if(I2C_Master_Init_Status[index]==false){return hwI2C_NotInit;}
    
    hwI2C_OpResult op_status = hwI2C_OK;
    
    op_status = I2C_Master_DeInit(index);
    if(op_status<hwI2C_OK)
    {
            return op_status;
    }
    
    op_status = I2C_Master_Init(index, I2C_Clock_Speed_Mode[index]);
    if(op_status<hwI2C_OK)
    {
            return op_status;
    }
    
    return op_status;
}

hwI2C_OpResult I2C_Master_Read(hwI2C_Index index, uint8_t address, uint8_t* read_dat, uint8_t read_len, bool stop, NeonRTOS_Time_t timeoutMs)
{
    if(index>=hwI2C_Index_MAX)
    {
        return hwI2C_InvalidParameter;
    }

    if(I2C_Master_Init_Status[index]==false)
    {
        return hwI2C_NotInit;
    }
    
    if(read_dat==NULL && read_len>0)
    {
        return hwI2C_InvalidParameter;
    }
    
    if(read_len<=0)
    {
        return hwI2C_InvalidParameter;
    }
    
    if (HAL_I2C_Master_Receive_IT(&g_i2c[index], address << 1, read_dat, read_len) != HAL_OK)
    {
        return hwI2C_BusError;
    }

    if (NeonRTOS_SyncObjWait(&I2C_Master_Done_SyncHandle[index], timeoutMs) != NeonRTOS_OK)
    {
        HAL_I2C_Abort_IT(&g_i2c[index]);
        return hwI2C_SlaveTimeout;
    }

    return hwI2C_OK;
}

hwI2C_OpResult I2C_Master_Write(hwI2C_Index index, uint8_t address, uint8_t* write_dat, uint8_t write_len, bool stop, NeonRTOS_Time_t timeoutMs)
{
    if(index>=hwI2C_Index_MAX)
    {
        return hwI2C_InvalidParameter;
    }
    if(I2C_Master_Init_Status[index]==false){return hwI2C_NotInit;}
    
    if(write_dat==NULL && write_len>0)
    {
        return hwI2C_InvalidParameter;
    }
    
    if (HAL_I2C_Master_Transmit_IT(&g_i2c[index], address << 1, write_dat, write_len) != HAL_OK)
    {
        return hwI2C_BusError;
    }

    if (NeonRTOS_SyncObjWait(&I2C_Master_Done_SyncHandle[index], timeoutMs) != NeonRTOS_OK)
    {
        HAL_I2C_Abort_IT(&g_i2c[index]);
        return hwI2C_SlaveTimeout;
    }

    return hwI2C_OK;
}

bool I2C_Master_isInit(hwI2C_Index index)
{
    if(index>=hwI2C_Index_MAX)
    {
        return false;
    }
    return I2C_Master_Init_Status[index];
}

#endif //STM32F2