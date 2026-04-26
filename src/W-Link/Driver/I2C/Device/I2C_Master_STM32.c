#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "soc.h"

#include "NeonRTOS.h"

#include "I2C/I2C_Master.h"

#ifdef STM32F0
#define TIMING_VAL_48M_CLK_100KHZ  0x10805E89  // Standard mode with Rise Time = 400ns and Fall Time = 100ns
#define TIMING_VAL_48M_CLK_400KHZ  0x00901850  // Fast mode with Rise Time = 250ns and Fall Time = 100ns
#define TIMING_VAL_48M_CLK_1MHZ    0x00700818  // Fast mode Plus with Rise Time = 60ns and Fall Time = 100ns
#define I2C_PCLK_48M               48000000    // 48 MHz    
#endif //STM32F0

#ifdef STM32F7
#define TIMING_VAL_54M_CLK_100KHZ  0x10916998  // Standard mode with Rise time = 120ns, Fall time = 120ns
#define TIMING_VAL_54M_CLK_400KHZ  0x00B11B54  // Fast Mode with Rise time = 120ns, Fall time = 120ns
#define TIMING_VAL_54M_CLK_1MHZ    0x0090091B  // Fast Mode Plus with Rise time = 120ns, Fall time = 10ns
#define I2C_PCLK_54M               54000000    // 54 MHz
#endif //STM32F7

#ifdef STM32F3
#define TIMING_VAL_64M_CLK_100KHZ  0x10B17DB4  // Standard mode with Rise time = 120ns, Fall time = 120ns
#define TIMING_VAL_64M_CLK_400KHZ  0x00E22163  // Fast Mode with Rise time = 120ns, Fall time = 120ns
#define TIMING_VAL_64M_CLK_1MHZ    0x00A00D1E  // Fast Mode Plus with Rise time = 120ns, Fall time = 10ns
#define I2C_PCLK_64M               64000000    // 64 MHz

#define TIMING_VAL_72M_CLK_100KHZ  0x10D28DCB  // Standard mode with Rise time = 120ns, Fall time = 120ns
#define TIMING_VAL_72M_CLK_400KHZ  0x00F32571  // Fast Mode with Rise time = 120ns, Fall time = 120ns
#define TIMING_VAL_72M_CLK_1MHZ    0x00C00D24  // Fast Mode Plus with Rise time = 120ns, Fall time = 10ns
#define I2C_PCLK_72M               72000000    // 72 MHz
#endif //STM32F3

#ifdef STM32G0
#define TIMING_VAL_64M_CLK_100KHZ  0xC0311319  // Standard mode with Rise Time = 400ns and Fall Time = 100ns
#define TIMING_VAL_64M_CLK_400KHZ  0x10B1102E  // Fast mode with Rise Time = 250ns and Fall Time = 100ns
#define TIMING_VAL_64M_CLK_1MHZ    0x00710B1E  // Fast mode Plus with Rise Time = 60ns and Fall Time = 100ns
#define I2C_PCLK_64M               64000000    // 64 MHz
#endif //STM32G0

#ifdef STM32G4
#define TIMING_VAL_160M_CLK_100KHZ  0x90A13E56  // Standard mode with Rise Time = 400ns and Fall Time = 100ns
#define TIMING_VAL_160M_CLK_400KHZ  0x30D2153A  // Fast mode with Rise Time = 250ns and Fall Time = 100ns
#define TIMING_VAL_160M_CLK_1MHZ    0x10830F28  // Fast mode Plus with Rise Time = 60ns and Fall Time = 100ns
#define I2C_PCLK_160M               160000000   // 160 MHz
#endif //STM32G4

#ifdef STM32H7
#define TIMING_VAL_140M_CLK_100KHZ 0x70B1445E   // Standard mode with Rise Time = 400ns and Fall Time = 100ns
#define TIMING_VAL_140M_CLK_400KHZ 0x30C11233   // Fast mode with Rise Time = 250ns and Fall Time = 100ns
#define TIMING_VAL_140M_CLK_1MHZ   0x00F41B47   // Fast mode Plus with Rise Time = 60ns and Fall Time = 100ns
#define I2C_PCLK_140M              140000000    // 140 MHz

#define TIMING_VAL_137M5_CLK_100KHZ 0x70B1445B   // Standard mode with Rise Time = 400ns and Fall Time = 100ns
#define TIMING_VAL_137M5_CLK_400KHZ 0x80510815   // Fast mode with Rise Time = 250ns and Fall Time = 100ns
#define TIMING_VAL_137M5_CLK_1MHZ   0x00F41A46   // Fast mode Plus with Rise Time = 60ns and Fall Time = 100ns
#define I2C_PCLK_137M5              137500000    // 137.5 MHz

#define TIMING_VAL_120M_CLK_100KHZ 0x40E15676   // Standard mode with Rise Time = 400ns and Fall Time = 100ns
#define TIMING_VAL_120M_CLK_400KHZ 0x20C11434   // Fast mode with Rise Time = 250ns and Fall Time = 100ns
#define TIMING_VAL_120M_CLK_1MHZ   0x00C31536   // Fast mode Plus with Rise Time = 60ns and Fall Time = 100ns
#define I2C_PCLK_120M              120000000    // 120 MHz

#define TIMING_VAL_112M5_CLK_100KHZ 0x80813142   // Standard mode with Rise Time = 400ns and Fall Time = 100ns
#define TIMING_VAL_112M5_CLK_400KHZ 0x20D11337   // Fast mode with Rise Time = 250ns and Fall Time = 100ns
#define TIMING_VAL_112M5_CLK_1MHZ   0x00C31538   // Fast mode Plus with Rise Time = 60ns and Fall Time = 100ns
#define I2C_PCLK_112M5              112500000    // 112.5 MHz

#define TIMING_VAL_100M_CLK_100KHZ 0x50A14159   // Standard mode with Rise Time = 400ns and Fall Time = 100ns
#define TIMING_VAL_100M_CLK_400KHZ 0x20B11130   // Fast mode with Rise Time = 250ns and Fall Time = 100ns
#define TIMING_VAL_100M_CLK_1MHZ   0x00A21232   // Fast mode Plus with Rise Time = 60ns and Fall Time = 100ns
#define I2C_PCLK_100M              100000000    // 100 MHz
#endif //STM32H7

#ifdef STM32L0
#define TIMING_VAL_32M_CLK_100KHZ  0x20602938  // Standard mode with Rise Time = 400ns and Fall Time = 100ns
#define TIMING_VAL_32M_CLK_400KHZ  0x00B0122A  // Fast mode with Rise Time = 250ns and Fall Time = 100ns
#define TIMING_VAL_32M_CLK_1MHZ    0x0030040E  // Fast mode Plus with Rise Time = 60ns and Fall Time = 100ns
#define I2C_PCLK_32M               32000000    // 32 MHz
#endif //STM32L0

#ifdef STM32L4
#define TIMING_VAL_80M_CLK_100KHZ 0x30C14E6B   // Standard mode with Rise Time = 400ns and Fall Time = 100ns
#define TIMING_VAL_80M_CLK_400KHZ 0x10D1143A   // Fast mode with Rise Time = 250ns and Fall Time = 100ns
#define TIMING_VAL_80M_CLK_1MHZ   0x00810E27   // Fast mode Plus with Rise Time = 60ns and Fall Time = 100ns
#define I2C_PCLK_80M              80000000     // 80 MHz

#define TIMING_VAL_48M_CLK_100KHZ 0x20A03E55   // Standard mode with Rise Time = 400ns and Fall Time = 100ns
#define TIMING_VAL_48M_CLK_400KHZ 0x10800C21   // Fast mode with Rise Time = 250ns and Fall Time = 100ns
#define TIMING_VAL_48M_CLK_1MHZ   0x00500816   // Fast mode Plus with Rise Time = 60ns and Fall Time = 100ns
#define I2C_PCLK_48M              48000000     // 48 MHz

#define TIMING_VAL_120M_CLK_100KHZ 0x107075B0  // Standard mode with Rise Time = 400ns and Fall Time = 100ns
#define TIMING_VAL_120M_CLK_400KHZ 0x00501E6C  // Fast mode with Rise Time = 250ns and Fall Time = 100ns
#define TIMING_VAL_120M_CLK_1MHZ   0x00200A26  // Fast mode Plus with Rise Time = 60ns and Fall Time = 100ns
#define I2C_PCLK_120M              120000000   // 120 MHz
#endif //STM32L4

#ifdef STM32L5
#define TIMING_VAL_110M_CLK_100KHZ 0x40E15676   // Standard mode with Rise Time = 400ns and Fall Time = 100ns
#define TIMING_VAL_110M_CLK_400KHZ 0x20C11434   // Fast mode with Rise Time = 250ns and Fall Time = 100ns
#define TIMING_VAL_110M_CLK_1MHZ   0x00C31536   // Fast mode Plus with Rise Time = 60ns and Fall Time = 100ns
#define I2C_PCLK_110M              110000000    // 110 MHz
#endif //STM32L5

#ifdef STM32U5
#define TIMING_VAL_160M_CLK_100KHZ 0x90A13E56   // Standard mode with Rise Time = 400ns and Fall Time = 100ns
#define TIMING_VAL_160M_CLK_400KHZ 0x30D2153A   // Fast mode with Rise Time = 250ns and Fall Time = 100ns
#define TIMING_VAL_160M_CLK_1MHZ   0x10830F28   // Fast mode Plus with Rise Time = 60ns and Fall Time = 100ns
#define I2C_PCLK_160M              160000000    // 160 MHz
#endif //STM32U5

#ifdef STM32WB
#define TIMING_VAL_64M_CLK_100KHZ 0x10707DBC   // Standard mode with Rise Time = 400ns and Fall Time = 100ns
#define TIMING_VAL_64M_CLK_400KHZ 0x00602173   // Fast mode with Rise Time = 250ns and Fall Time = 100ns
#define TIMING_VAL_64M_CLK_1MHZ   0x00300B29   // Fast mode Plus with Rise Time = 60ns and Fall Time = 100ns
#define I2C_PCLK_64M              64000000     // 64 MHz

#define TIMING_VAL_32M_CLK_100KHZ 0x00707CBB   // Standard mode with Rise Time = 400ns and Fall Time = 100ns
#define TIMING_VAL_32M_CLK_400KHZ 0x00300F38   // Fast mode with Rise Time = 250ns and Fall Time = 100ns
#define TIMING_VAL_32M_CLK_1MHZ   0x00100413   // Fast mode Plus with Rise Time = 60ns and Fall Time = 100ns
#define I2C_PCLK_32M              32000000     // 32 MHz
#endif //STM32WB

#ifdef STM32WL
#define TIMING_VAL_48M_CLK_100KHZ  0x20E03F53  // Standard mode with Rise Time = 640ns and Fall Time = 20ns
#define TIMING_VAL_48M_CLK_400KHZ  0x20500817  // Fast mode with Rise Time = 250ns and Fall Time = 100ns
#define TIMING_VAL_48M_CLK_1MHZ    0x00500A18  // Fast mode Plus with Rise Time = 60ns and Fall Time = 100ns
#define I2C_PCLK_48M               48000000    // 48 MHz
#endif //STM32WL

uint32_t I2C_Master_Get_Timing(hwI2C_Index index, hwI2C_Speed_Mode speed_mode)
{
    uint32_t tim = 0;

    uint32_t pclk = I2C_Get_PCLK(index);

    switch (pclk) {
#if defined (I2C_PCLK_32M)
        case I2C_PCLK_32M:
            switch (speed_mode) {
                case hwI2C_Standard_Mode:
                    tim = TIMING_VAL_32M_CLK_100KHZ;
                    break;
                case hwI2C_Fast_Mode:
                    tim = TIMING_VAL_32M_CLK_400KHZ;
                    break;
                case hwI2C_High_Speed_Mode:
                    tim = TIMING_VAL_32M_CLK_1MHZ;
                    break;
            }
            break;
#endif
#if defined (I2C_PCLK_48M)
        case I2C_PCLK_48M:
            switch (speed_mode) {
                case hwI2C_Standard_Mode:
                    tim = TIMING_VAL_48M_CLK_100KHZ;
                    break;
                case hwI2C_Fast_Mode:
                    tim = TIMING_VAL_48M_CLK_400KHZ;
                    break;
                case hwI2C_High_Speed_Mode:
                    tim = TIMING_VAL_48M_CLK_1MHZ;
                    break;
            }
            break;
#endif
#if defined (I2C_PCLK_54M)
        case I2C_PCLK_54M:
            switch (speed_mode) {
                case hwI2C_Standard_Mode:
                    tim = TIMING_VAL_54M_CLK_100KHZ;
                    break;
                case hwI2C_Fast_Mode:
                    tim = TIMING_VAL_54M_CLK_400KHZ;
                    break;
                case hwI2C_High_Speed_Mode:
                    tim = TIMING_VAL_54M_CLK_1MHZ;
                    break;
            }
            break;
#endif
#if defined(I2C_PCLK_64M)
        case I2C_PCLK_64M:
            switch (speed_mode) {
                case hwI2C_Standard_Mode:
                    tim = TIMING_VAL_64M_CLK_100KHZ;
                    break;
                case hwI2C_Fast_Mode:
                    tim = TIMING_VAL_64M_CLK_400KHZ;
                    break;
                case hwI2C_High_Speed_Mode:
                    tim = TIMING_VAL_64M_CLK_1MHZ;
                    break;
            }
            break;
#endif
#if defined (I2C_PCLK_72M)
        case I2C_PCLK_72M:
            switch (speed_mode) {
                case hwI2C_Standard_Mode:
                    tim = TIMING_VAL_72M_CLK_100KHZ;
                    break;
                case hwI2C_Fast_Mode:
                    tim = TIMING_VAL_72M_CLK_400KHZ;
                    break;
                case hwI2C_High_Speed_Mode:
                    tim = TIMING_VAL_72M_CLK_1MHZ;
                    break;
            }
            break;
#endif
#if defined (I2C_PCLK_80M)
        case I2C_PCLK_80M:
            switch (speed_mode) {
                case hwI2C_Standard_Mode:
                    tim = TIMING_VAL_80M_CLK_100KHZ;
                    break;
                case hwI2C_Fast_Mode:
                    tim = TIMING_VAL_80M_CLK_400KHZ;
                    break;
                case hwI2C_High_Speed_Mode:
                    tim = TIMING_VAL_80M_CLK_1MHZ;
                    break;
            }
            break;
#endif
#if defined (I2C_PCLK_100M)
        case I2C_PCLK_100M:
            switch (speed_mode) {
                case hwI2C_Standard_Mode:
                    tim = TIMING_VAL_100M_CLK_100KHZ;
                    break;
                case hwI2C_Fast_Mode:
                    tim = TIMING_VAL_100M_CLK_400KHZ;
                    break;
                case hwI2C_High_Speed_Mode:
                    tim = TIMING_VAL_100M_CLK_1MHZ;
                    break;
            }
            break;
#endif
#if defined (I2C_PCLK_110M)
        case I2C_PCLK_110M:
            switch (speed_mode) {
                case hwI2C_Standard_Mode:
                    tim = TIMING_VAL_110M_CLK_100KHZ;
                    break;
                case hwI2C_Fast_Mode:
                    tim = TIMING_VAL_110M_CLK_400KHZ;
                    break;
                case hwI2C_High_Speed_Mode:
                    tim = TIMING_VAL_110M_CLK_1MHZ;
                    break;
            }
            break;
#endif
#if defined (I2C_PCLK_112M5)
        case I2C_PCLK_112M5:
            switch (speed_mode) {
                case hwI2C_Standard_Mode:
                    tim = TIMING_VAL_112M5_CLK_100KHZ;
                    break;
                case hwI2C_Fast_Mode:
                    tim = TIMING_VAL_112M5_CLK_400KHZ;
                    break;
                case hwI2C_High_Speed_Mode:
                    tim = TIMING_VAL_112M5_CLK_1MHZ;
                    break;
            }
            break;
#endif
#if defined (I2C_PCLK_120M)
        case I2C_PCLK_120M:
            switch (speed_mode) {
                case hwI2C_Standard_Mode:
                    tim = TIMING_VAL_120M_CLK_100KHZ;
                    break;
                case hwI2C_Fast_Mode:
                    tim = TIMING_VAL_120M_CLK_400KHZ;
                    break;
                case hwI2C_High_Speed_Mode:
                    tim = TIMING_VAL_120M_CLK_1MHZ;
                    break;
            }
            break;
#endif
#if defined (I2C_PCLK_137M5)
        case I2C_PCLK_137M5:
            switch (speed_mode) {
                case hwI2C_Standard_Mode:
                    tim = TIMING_VAL_137M5_CLK_100KHZ;
                    break;
                case hwI2C_Fast_Mode:
                    tim = TIMING_VAL_137M5_CLK_400KHZ;
                    break;
                case hwI2C_High_Speed_Mode:
                    tim = TIMING_VAL_137M5_CLK_1MHZ;
                    break;
            }
            break;
#endif
#if defined (I2C_PCLK_140M)
        case I2C_PCLK_140M:
            switch (speed_mode) {
                case hwI2C_Standard_Mode:
                    tim = TIMING_VAL_140M_CLK_100KHZ;
                    break;
                case hwI2C_Fast_Mode:
                    tim = TIMING_VAL_140M_CLK_400KHZ;
                    break;
                case hwI2C_High_Speed_Mode:
                    tim = TIMING_VAL_140M_CLK_1MHZ;
                    break;
            }
            break;
#endif
#if defined (I2C_PCLK_160M)
        case I2C_PCLK_160M:
            switch (speed_mode) {
                case hwI2C_Standard_Mode:
                    tim = TIMING_VAL_160M_CLK_100KHZ;
                    break;
                case hwI2C_Fast_Mode:
                    tim = TIMING_VAL_160M_CLK_400KHZ;
                    break;
                case hwI2C_High_Speed_Mode:
                    tim = TIMING_VAL_160M_CLK_1MHZ;
                    break;
            }
            break;
#endif
    }
    
    return tim;
}
