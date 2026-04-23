
#ifndef I2C_INDEX_H
#define I2C_INDEX_H

typedef enum hwI2C_Index_t
{
  hwI2C_Index_0 = 0,
  hwI2C_Index_1,
  hwI2C_Index_2,
  hwI2C_Index_3,
#if defined (I2C5_BASE)
  hwI2C_Index_4,
#endif
#if defined (I2C6_BASE)
  hwI2C_Index_5,
#endif
  hwI2C_Index_MAX,
}hwI2C_Index;

#endif //I2C_INDEX_H