
#ifndef I2C_MASTER_STM32_H
#define I2C_MASTER_STM32_H

#include "soc.h"

#include "I2C/I2C_Def.h"

#include "I2C/I2C_Index.h"

//internal use
uint32_t I2C_Master_Get_Timing(hwI2C_Index index, hwI2C_Speed_Mode speed_mode);

#endif //I2C_MASTER_STM32_H