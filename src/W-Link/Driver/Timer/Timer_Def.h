
#ifndef TIMER_DEF_H
#define TIMER_DEF_H

#include <stdbool.h>

typedef enum hwTimer_OpMode_t
{
  hwTimer_Mode_NotInit = -1,
  hwTimer_Mode_Idle = 0,
  hwTimer_Mode_OneShout = 1,
  hwTimer_Mode_Period = 2,
}hwTimer_OpMode;

typedef enum hwTimer_OpResult_t
{
  hwTimer_OK = 0,
  hwTimer_NotInit = -1,
  hwTimer_InvalidParameter = -2,
  hwTimer_HwError = -3,
  hwTimer_MemoryError = -4,
  hwTimer_MutexTimeout = -5,
  hwTimer_Unsupport = -6,
}hwTimer_OpResult;

typedef void(* onTimerEventHandler)(hwTimer_Index index);

#endif //TIMER_DEF_H