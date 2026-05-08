/*
This LSM6DSO driver is based on STMicroelectronics stm32-lsm6dso / LSM6DSO component driver.
Original copyright: STMicroelectronics.
Original license: BSD-3-Clause.

Modifications:
- Ported IO layer to W-Link / NeonRTOS I2C interface.
- Adapted configuration macros and operation status mapping.
- Removed STM32 BSP object/context dependency.
*/

#include <stdint.h>
#include <stdbool.h>

float LSM6DSO_From_FS2_To_Mg(int16_t lsb)
{
  return ((float)lsb) * 0.061f;
}

float LSM6DSO_From_FS4_To_Mg(int16_t lsb)
{
  return ((float)lsb) * 0.122f;
}

float LSM6DSO_From_FS8_To_Mg(int16_t lsb)
{
  return ((float)lsb) * 0.244f;
}

float LSM6DSO_From_FS16_To_Mg(int16_t lsb)
{
  return ((float)lsb) *0.488f;
}

float LSM6DSO_From_FS125_To_Mdps(int16_t lsb)
{
  return ((float)lsb) *4.375f;
}

float LSM6DSO_From_FS500_To_Mdps(int16_t lsb)
{
  return ((float)lsb) *17.50f;
}

float LSM6DSO_From_FS250_To_Mdps(int16_t lsb)
{
  return ((float)lsb) *8.750f;
}

float LSM6DSO_From_FS1000_To_Mdps(int16_t lsb)
{
  return ((float)lsb) *35.0f;
}

float LSM6DSO_From_FS2000_To_Mdps(int16_t lsb)
{
  return ((float)lsb) *70.0f;
}

float LSM6DSO_From_LSB_To_Celsius(int16_t lsb)
{
  return (((float)lsb / 256.0f) + 25.0f);
}

float LSM6DSO_From_LSB_To_nSec(int16_t lsb)
{
  return ((float)lsb * 25000.0f);
}
