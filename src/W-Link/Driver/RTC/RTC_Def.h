
#ifndef RTC_DEF_H
#define RTC_DEF_H

#define SECONDS_FROM_1970_TO_2000 946684800

typedef enum hwRTC_OpResult_t
{
  hwRTC_OK = 0,
  hwRTC_NotInit = -1,
  hwRTC_InvalidParameter = -2,
  hwRTC_HwError = -3,
  hwRTC_MemoryError = -4,
  hwRTC_MutexTimeout = -5,
  hwRTC_Unsupport = -6,
}hwRTC_OpResult;

#endif //RTC_DEF_H