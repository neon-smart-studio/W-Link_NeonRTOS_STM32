#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "soc.h"

#include "NeonRTOS.h"

#include "I2C/I2C_Master.h"

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
