
#ifndef CAN_DEF_H
#define CAN_DEF_H

typedef enum hwCAN_OpResult_t
{
  hwCAN_OK = 0,
  hwCAN_NotInit = -1,
  hwCAN_InvalidParameter = -2,
  hwCAN_HwError = -4,
  hwCAN_MemoryError = -5,
  hwCAN_MutexTimeout = -6,
  hwCAN_Timeout = -7,
  hwCAN_BusError = -8,
  hwCAN_Unsupport = -9,
} hwCAN_OpResult;

#endif //CAN_DEF_H