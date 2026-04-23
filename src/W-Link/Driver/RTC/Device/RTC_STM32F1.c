
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "soc.h"

#include "NeonRTOS.h"

#include "RTC/RTC.h"

#ifdef STM32F1

#define RTC_MUTEX_ACCESS_TIMEOUT     500

static RTC_HandleTypeDef g_rtc[hwRTC_Index_MAX];

static bool RTC_HW_Init_Status[hwRTC_Index_MAX] = {false};
static NeonRTOS_LockObj_t rtc_access_mutex[hwRTC_Index_MAX];

#ifdef RTC_SUPPORT_ALARM
onAlarmEventCallback Alarm_Event_Callback[hwRTC_Alarm_Channel_Index_MAX] = {NULL};
#endif //RTC_SUPPORT_ALARM

#define RTC_MUTEX_LOCK(a, b)  if (NeonRTOS_LockObjLock(&rtc_access_mutex[a], b)!=NeonRTOS_OK) {return hwRTC_MutexTimeout;}
#define RTC_MUTEX_UNLOCK(a)   NeonRTOS_LockObjUnlock(&rtc_access_mutex[a]);

static void UnixToRTC(time_t t, RTC_TimeTypeDef *time, RTC_DateTypeDef *date)
{
    struct tm tm_time;
    gmtime_r(&t, &tm_time);

    time->Hours   = tm_time.tm_hour;
    time->Minutes = tm_time.tm_min;
    time->Seconds = tm_time.tm_sec;

    date->Year  = tm_time.tm_year - 100; // since 2000
    date->Month = tm_time.tm_mon + 1;
    date->Date  = tm_time.tm_mday;
    date->WeekDay = tm_time.tm_wday + 1;
}

static time_t RTCToUnix(RTC_TimeTypeDef *time, RTC_DateTypeDef *date)
{
    struct tm tm_time = {0};

    tm_time.tm_year = date->Year + 100;
    tm_time.tm_mon  = date->Month - 1;
    tm_time.tm_mday = date->Date;
    tm_time.tm_hour = time->Hours;
    tm_time.tm_min  = time->Minutes;
    tm_time.tm_sec  = time->Seconds;

    return mktime(&tm_time);
}

static hwRTC_Index RTC_IndexFromHandle(RTC_HandleTypeDef *hrtc)
{
    for(int i=0;i<hwRTC_Index_MAX;i++){
        if(&g_rtc[i] == hrtc) return (hwRTC_Index)i;
    }
    return hwRTC_Index_MAX;
}

hwRTC_OpResult RTC_Timer_Init(hwRTC_Index index)
{
    if (index >= hwRTC_Index_MAX)
    {
        return hwRTC_InvalidParameter;
    }

    if (RTC_HW_Init_Status[index])
    {
        return hwRTC_OK;
    }

    if (NeonRTOS_LockObjCreate(&rtc_access_mutex[index]) != NeonRTOS_OK)
    {
        return hwRTC_MemoryError;
    }
    NeonRTOS_LockObjUnlock(&rtc_access_mutex[index]);

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_RCC_BKP_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();

    /* 啟用 LSE */
    __HAL_RCC_LSE_CONFIG(RCC_LSE_ON);
    while (__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) == RESET)
    {
    }

    /* 選 RTC 時鐘來源 */
    __HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);
    __HAL_RCC_RTC_ENABLE();

    g_rtc[index].Instance = RTC;

    if (HAL_RTC_Init(&g_rtc[index]) != HAL_OK)
    {
        return hwRTC_HwError;
    }

    /* 等待同步 */
    if (HAL_RTC_WaitForSynchro(&g_rtc[index]) != HAL_OK)
    {
        return hwRTC_HwError;
    }

    /* 設 prescaler = 32767 -> 1Hz */
    __HAL_RTC_WRITEPROTECTION_DISABLE(&g_rtc[index]);

    if (RTC_EnterInitMode(&g_rtc[index]) != HAL_OK)
    {
        __HAL_RTC_WRITEPROTECTION_ENABLE(&g_rtc[index]);
        return hwRTC_HwError;
    }

    g_rtc[index].Instance->PRLH = (uint16_t)(32767 >> 16);
    g_rtc[index].Instance->PRLL = (uint16_t)(32767 & 0xFFFF);

    if (RTC_ExitInitMode(&g_rtc[index]) != HAL_OK)
    {
        __HAL_RTC_WRITEPROTECTION_ENABLE(&g_rtc[index]);
        return hwRTC_HwError;
    }

    __HAL_RTC_WRITEPROTECTION_ENABLE(&g_rtc[index]);

    RTC_HW_Init_Status[index] = true;

    return hwRTC_OK;
}

hwRTC_OpResult RTC_Timer_DeInit(hwRTC_Index index)
{
    if (index >= hwRTC_Index_MAX)
    {
        return hwRTC_InvalidParameter;
    }

    if (!RTC_HW_Init_Status[index])
    {
        return hwRTC_OK;
    }

    HAL_RTC_DeInit(&g_rtc[index]);

    NeonRTOS_LockObjDelete(&rtc_access_mutex[index]);
    rtc_access_mutex[index] = NULL;

    RTC_HW_Init_Status[index] = false;
    return hwRTC_OK;
}

hwRTC_OpResult RTC_Timer_Read(hwRTC_Index index, time_t *unix_time)
{
    if (index >= hwRTC_Index_MAX || unix_time == NULL)
	{
        return hwRTC_InvalidParameter;
	}

    if (!RTC_HW_Init_Status[index])
	{
        return hwRTC_NotInit;
	}

    RTC_TimeTypeDef time;
    RTC_DateTypeDef date;

    RTC_MUTEX_LOCK(index, RTC_MUTEX_ACCESS_TIMEOUT);

    HAL_RTC_GetTime(&g_rtc[index], &time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&g_rtc[index], &date, RTC_FORMAT_BIN);

    *unix_time = RTCToUnix(&time, &date);

    RTC_MUTEX_UNLOCK(index);

    return hwRTC_OK;
}

hwRTC_OpResult RTC_Timer_Write(hwRTC_Index index, time_t unix_time)
{
    if (index >= hwRTC_Index_MAX)
	{
        return hwRTC_InvalidParameter;
	}

    if (!RTC_HW_Init_Status[index])
	{
        return hwRTC_NotInit;
	}

    RTC_TimeTypeDef time;
    RTC_DateTypeDef date;

    UnixToRTC(unix_time, &time, &date);

    RTC_MUTEX_LOCK(index, RTC_MUTEX_ACCESS_TIMEOUT);

    HAL_RTC_SetTime(&g_rtc[index], &time, RTC_FORMAT_BIN);
    HAL_RTC_SetDate(&g_rtc[index], &date, RTC_FORMAT_BIN);

    RTC_MUTEX_UNLOCK(index);

    return hwRTC_OK;
}

#endif //STM32F1