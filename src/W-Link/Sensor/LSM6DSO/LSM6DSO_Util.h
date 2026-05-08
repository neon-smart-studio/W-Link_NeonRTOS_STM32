/*
This LSM6DSO driver is based on STMicroelectronics stm32-lsm6dso / LSM6DSO component driver.
Original copyright: STMicroelectronics.
Original license: BSD-3-Clause.

Modifications:
- Ported IO layer to W-Link / NeonRTOS I2C interface.
- Adapted configuration macros and operation status mapping.
- Removed STM32 BSP object/context dependency.
*/

#ifndef LSM6DSO_UTIL_H
#define LSM6DSO_UTIL_H

extern float LSM6DSO_From_FS2_To_Mg(int16_t lsb);
extern float LSM6DSO_From_FS4_To_Mg(int16_t lsb);
extern float LSM6DSO_From_FS8_To_Mg(int16_t lsb);
extern float LSM6DSO_From_FS16_To_Mg(int16_t lsb);
extern float LSM6DSO_From_FS125_To_Mdps(int16_t lsb);
extern float LSM6DSO_From_FS500_To_Mdps(int16_t lsb);
extern float LSM6DSO_From_FS250_To_Mdps(int16_t lsb);
extern float LSM6DSO_From_FS1000_To_Mdps(int16_t lsb);
extern float LSM6DSO_From_FS2000_To_Mdps(int16_t lsb);
extern float LSM6DSO_From_LSB_To_Celsius(int16_t lsb);
extern float LSM6DSO_From_LSB_To_nSec(int16_t lsb);

#endif //LSM6DSO_UTIL_H
