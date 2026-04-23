
#ifndef SPI_DEF_H
#define SPI_DEF_H

typedef enum hwSPI_OpMode_t
{
  hwSPI_OpMode_Polarity0_Phase0 = 0,
  hwSPI_OpMode_Polarity0_Phase1 = 1,
  hwSPI_OpMode_Polarity1_Phase0 = 2,
  hwSPI_OpMode_Polarity1_Phase1 = 3,
  hwSPI_OpMode_MAX = 4,
}hwSPI_OpMode;

typedef enum hwSPI_OpResult_t
{
  hwSPI_OK = 0,
  hwSPI_NotInit = -1,
  hwSPI_InvalidParameter = -2,
  hwSPI_HwError = -3,
  hwSPI_MemoryError = -4,
  hwSPI_MutexTimeout = -5,
  hwSPI_SlaveTimeout = -6,
  hwSPI_Unsupport = -7,
}hwSPI_OpResult;

#endif //SPI_DEF_H