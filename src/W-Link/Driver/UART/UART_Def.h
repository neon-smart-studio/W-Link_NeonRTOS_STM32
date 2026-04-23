
#ifndef UART_DEF_H
#define UART_DEF_H

#include <stdbool.h>

typedef enum hwUART_OpResult_t
{
  hwUART_OK = 0,
  hwUART_NotInit = -1,
  hwUART_InvalidParameter = -2,
  hwUART_Busy = -3,
  hwUART_MemoryError = -4,
  hwUART_MutexTimeout = -5,
  hwUART_BusError = -6,
  hwUART_HwError = -7,
  hwUART_Unsupport = -8,
}hwUART_OpResult;

typedef enum {
  UART_Parity_None = 0,		/*!<parity disable */
  UART_Parity_Odd = 1,		/*!<odd parity enable */
  UART_Parity_Even = 2,		/*!<even paroty enable */
  UART_Parity_MAX = 3,		/*!<even paroty enable */
} UART_Parity;

#endif //UART_DEF_H