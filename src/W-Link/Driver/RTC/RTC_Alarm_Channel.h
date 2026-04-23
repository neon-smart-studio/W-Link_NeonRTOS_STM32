
#ifndef RTC_ALARM_CHANNEL_H
#define RTC_ALARM_CHANNEL_H

typedef enum {
#if defined(RTC_ALARM_A)
    hwRTC_Alarm_Channel_Index_0 = 0, // Alarm A
#endif
#if defined(RTC_ALARM_B)
    hwRTC_Alarm_Channel_Index_1,     // Alarm B
#endif
    hwRTC_Alarm_Channel_Index_MAX
} hwRTC_Alarm_Channel_Index;

#endif //RTC_ALARM_CHANNEL_H