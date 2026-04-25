#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

#include "soc.h"

#include "NeonRTOS.h"

#include "RTC/RTC.h"

#ifdef STM32C0

#define RTC_MUTEX_ACCESS_TIMEOUT     500

#define RTC_IRQ_NVIC_PRIORITY        5
#define RTC_IRQ_NVIC_SUB_PRIORITY    0

static RTC_HandleTypeDef g_rtc[hwRTC_Index_MAX];

static bool RTC_HW_Init_Status[hwRTC_Index_MAX] = {false};
static NeonRTOS_LockObj_t rtc_access_mutex[hwRTC_Index_MAX];

onAlarmEventCallback Alarm_Event_Callback[hwRTC_Alarm_Channel_Index_MAX] = {NULL};

#define RTC_MUTEX_LOCK(a, b)  \
    if (NeonRTOS_LockObjLock(&rtc_access_mutex[a], b) != NeonRTOS_OK) { return hwRTC_MutexTimeout; }

#define RTC_MUTEX_UNLOCK(a)   \
    NeonRTOS_LockObjUnlock(&rtc_access_mutex[a]);

static void UnixToRTC(time_t t, RTC_TimeTypeDef *time, RTC_DateTypeDef *date)
{
    struct tm tm_time;
    gmtime_r(&t, &tm_time);

    time->Hours          = tm_time.tm_hour;
    time->Minutes        = tm_time.tm_min;
    time->Seconds        = tm_time.tm_sec;
    time->DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    time->StoreOperation = RTC_STOREOPERATION_RESET;

    date->Year    = tm_time.tm_year - 100;
    date->Month   = tm_time.tm_mon + 1;
    date->Date    = tm_time.tm_mday;
    date->WeekDay = tm_time.tm_wday + 1;

    if (date->WeekDay == 1) {
        date->WeekDay = RTC_WEEKDAY_SUNDAY;
    }
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
    for (int i = 0; i < hwRTC_Index_MAX; i++) {
        if (&g_rtc[i] == hrtc) {
            return (hwRTC_Index)i;
        }
    }

    return hwRTC_Index_MAX;
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
    hwRTC_Index index = RTC_IndexFromHandle(hrtc);

    if (index >= hwRTC_Index_MAX) {
        return;
    }

    if (Alarm_Event_Callback[hwRTC_Alarm_Channel_Index_0]) {
        Alarm_Event_Callback[hwRTC_Alarm_Channel_Index_0](
            index,
            hwRTC_Alarm_Channel_Index_0
        );
    }
}

void RTC_IRQHandler(void)
{
    HAL_RTC_AlarmIRQHandler(&g_rtc[hwRTC_Index_0]);
}

static void RTC_Enable_Clock_Source(void)
{
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();

#if defined(RCC_RTCCLKSOURCE_LSE)
    /*
     * 如果你板子有 32.768kHz LSE，可以改用 LSE。
     * Nucleo C031C6 預設比較保險是 LSI。
     */
#endif

#if defined(RCC_RTCCLKSOURCE_LSI)
    __HAL_RCC_LSI_ENABLE();

    while (__HAL_RCC_GET_FLAG(RCC_FLAG_LSIRDY) == RESET) {
    }

    __HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSI);
#endif

    __HAL_RCC_RTC_ENABLE();
}

hwRTC_OpResult RTC_Timer_Init(hwRTC_Index index)
{
    if (index >= hwRTC_Index_MAX) {
        return hwRTC_InvalidParameter;
    }

    if (RTC_HW_Init_Status[index]) {
        return hwRTC_OK;
    }

    if (NeonRTOS_LockObjCreate(&rtc_access_mutex[index]) != NeonRTOS_OK) {
        return hwRTC_MemoryError;
    }

    NeonRTOS_LockObjUnlock(&rtc_access_mutex[index]);

    RTC_Enable_Clock_Source();

    g_rtc[index].Instance = RTC;

    g_rtc[index].Init.HourFormat = RTC_HOURFORMAT_24;

    /*
     * LSI 約 32kHz：
     * 32000 / (127 + 1) / (249 + 1) = 1Hz
     *
     * 如果改用 LSE 32768Hz，請改回：
     * AsynchPrediv = 127
     * SynchPrediv  = 255
     */
    g_rtc[index].Init.AsynchPrediv = 127;
    g_rtc[index].Init.SynchPrediv  = 249;

    g_rtc[index].Init.OutPut = RTC_OUTPUT_DISABLE;

#if defined(RTC_OUTPUT_POLARITY_HIGH)
    g_rtc[index].Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
#endif

#if defined(RTC_OUTPUT_TYPE_OPENDRAIN)
    g_rtc[index].Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
#endif

    if (HAL_RTC_Init(&g_rtc[index]) != HAL_OK) {
        NeonRTOS_LockObjDelete(&rtc_access_mutex[index]);
        rtc_access_mutex[index] = NULL;
        return hwRTC_HwError;
    }

    HAL_NVIC_SetPriority(
        RTC_IRQn,
        RTC_IRQ_NVIC_PRIORITY,
        RTC_IRQ_NVIC_SUB_PRIORITY
    );
    HAL_NVIC_EnableIRQ(RTC_IRQn);

    RTC_HW_Init_Status[index] = true;

    return hwRTC_OK;
}

hwRTC_OpResult RTC_Timer_DeInit(hwRTC_Index index)
{
    if (index >= hwRTC_Index_MAX) {
        return hwRTC_InvalidParameter;
    }

    if (!RTC_HW_Init_Status[index]) {
        return hwRTC_OK;
    }

    HAL_NVIC_DisableIRQ(RTC_IRQn);

    HAL_RTC_DeactivateAlarm(&g_rtc[index], RTC_ALARM_A);

    HAL_RTC_DeInit(&g_rtc[index]);

    NeonRTOS_LockObjDelete(&rtc_access_mutex[index]);
    rtc_access_mutex[index] = NULL;

    RTC_HW_Init_Status[index] = false;

    return hwRTC_OK;
}

hwRTC_OpResult RTC_Timer_Read(hwRTC_Index index, time_t *unix_time)
{
    if (index >= hwRTC_Index_MAX || unix_time == NULL) {
        return hwRTC_InvalidParameter;
    }

    if (!RTC_HW_Init_Status[index]) {
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
    if (index >= hwRTC_Index_MAX) {
        return hwRTC_InvalidParameter;
    }

    if (!RTC_HW_Init_Status[index]) {
        return hwRTC_NotInit;
    }

    RTC_TimeTypeDef time;
    RTC_DateTypeDef date;

    UnixToRTC(unix_time, &time, &date);

    RTC_MUTEX_LOCK(index, RTC_MUTEX_ACCESS_TIMEOUT);

    if (HAL_RTC_SetTime(&g_rtc[index], &time, RTC_FORMAT_BIN) != HAL_OK) {
        RTC_MUTEX_UNLOCK(index);
        return hwRTC_HwError;
    }

    if (HAL_RTC_SetDate(&g_rtc[index], &date, RTC_FORMAT_BIN) != HAL_OK) {
        RTC_MUTEX_UNLOCK(index);
        return hwRTC_HwError;
    }

    RTC_MUTEX_UNLOCK(index);

    return hwRTC_OK;
}

hwRTC_OpResult RTC_Timer_Set_Alarm(
    hwRTC_Index index,
    hwRTC_Alarm_Channel_Index alarm_ch,
    time_t alarm_unix_time,
    onAlarmEventCallback cb
)
{
    if (index >= hwRTC_Index_MAX) {
        return hwRTC_InvalidParameter;
    }

    if (alarm_ch >= hwRTC_Alarm_Channel_Index_MAX) {
        return hwRTC_InvalidParameter;
    }

    if (!RTC_HW_Init_Status[index]) {
        return hwRTC_NotInit;
    }

    RTC_AlarmTypeDef alarm = {0};
    RTC_TimeTypeDef time;
    RTC_DateTypeDef date;

    UnixToRTC(alarm_unix_time, &time, &date);

    RTC_MUTEX_LOCK(index, RTC_MUTEX_ACCESS_TIMEOUT);

    alarm.AlarmTime = time;
    alarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
    alarm.Alarm     = RTC_ALARM_A;

    alarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;

    Alarm_Event_Callback[alarm_ch] = cb;

    if (HAL_RTC_SetAlarm_IT(&g_rtc[index], &alarm, RTC_FORMAT_BIN) != HAL_OK) {
        Alarm_Event_Callback[alarm_ch] = NULL;
        RTC_MUTEX_UNLOCK(index);
        return hwRTC_HwError;
    }

    RTC_MUTEX_UNLOCK(index);

    return hwRTC_OK;
}

hwRTC_OpResult RTC_Timer_Clear_Alarm(
    hwRTC_Index index,
    hwRTC_Alarm_Channel_Index alarm_ch_index
)
{
    if (index >= hwRTC_Index_MAX) {
        return hwRTC_InvalidParameter;
    }

    if (alarm_ch_index >= hwRTC_Alarm_Channel_Index_MAX) {
        return hwRTC_InvalidParameter;
    }

    if (!RTC_HW_Init_Status[index]) {
        return hwRTC_NotInit;
    }

    RTC_MUTEX_LOCK(index, RTC_MUTEX_ACCESS_TIMEOUT);

    Alarm_Event_Callback[alarm_ch_index] = NULL;

    switch (alarm_ch_index) {
        case hwRTC_Alarm_Channel_Index_0:
            HAL_RTC_DeactivateAlarm(&g_rtc[index], RTC_ALARM_A);
            break;

        default:
            RTC_MUTEX_UNLOCK(index);
            return hwRTC_InvalidParameter;
    }

    RTC_MUTEX_UNLOCK(index);

    return hwRTC_OK;
}

#endif // STM32C0