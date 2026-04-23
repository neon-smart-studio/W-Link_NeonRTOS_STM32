
#ifndef I2C_DEF_H
#define I2C_DEF_H

typedef enum hwI2C_Speed_Mode_t
{
  hwI2C_Standard_Mode = 0,
  hwI2C_Fast_Mode = 1,
  hwI2C_High_Speed_Mode = 2,
  hwI2C_Speed_Mode_MAX = 3,
} hwI2C_Speed_Mode;

typedef enum hwI2C_OpResult_t
{
  hwI2C_OK = 0,
  hwI2C_NotInit = -1,
  hwI2C_InvalidParameter = -2,
  hwI2C_HwError = -4,
  hwI2C_MemoryError = -5,
  hwI2C_MutexTimeout = -6,
  hwI2C_SlaveTimeout = -7,
  hwI2C_BusError = -8,
  hwI2C_Unsupport = -9,
} hwI2C_OpResult;

#endif //I2C_DEF_H