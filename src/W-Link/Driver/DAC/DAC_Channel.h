
#ifndef DAC_CHANNEL_H
#define DAC_CHANNEL_H

typedef enum hwDAC_Channel_Index_t
{
#if defined(DAC_BASE)
  hwDAC_Channel_Index_0 = 0,
  hwDAC_Channel_Index_1,
#endif
  hwDAC_Channel_Index_MAX,
}hwDAC_Channel_Index;

#endif //DAC_CHANNEL_H