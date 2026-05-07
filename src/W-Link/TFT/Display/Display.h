#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

#include <stdint.h>

#if defined(CONFIG_DISPLAY_GC9A01)
#include "GC9A01/GC9A01.h"
#elif defined(CONFIG_DISPLAY_HX8357B) || defined(CONFIG_DISPLAY_HX8357D)
#include "HX8357x/HX8357x.h"
#elif defined(CONFIG_DISPLAY_ILI9225) || defined(CONFIG_DISPLAY_ILI9163) || \
      defined(CONFIG_DISPLAY_ILI9341) || defined(CONFIG_DISPLAY_ILI9481) || \
      defined(CONFIG_DISPLAY_ILI9486) || defined(CONFIG_DISPLAY_ILI9488)
#include "ILI9xxx/ILI9xxx.h"
#elif defined(CONFIG_DISPLAY_ST7735) || defined(CONFIG_DISPLAY_ST7789) || \
      defined(CONFIG_DISPLAY_ST7796S) || defined(CONFIG_DISPLAY_ST7796HV)
#include "ST77xx/ST77xx.h"
#endif

typedef enum {
    Display_OK = 0,
    Display_NotInit = -1,
    Display_InvalidParameter = -2,
    Display_MemoryError = -3,
    Display_HwError = -4,
    Display_MutexTimeout = -5,
    Display_SlaveTimeout = -6,
    Display_SyncTimeout = -7,
    Display_Unsupport = -8,
} Display_Result;

typedef union {
    struct {
        uint16_t blue  : 5;
        uint16_t green : 6;
        uint16_t red   : 5;
    };
    uint16_t full;
} Display_Color16_RGB565;

Display_Result Display_Init(void);
Display_Result Display_Power_On(void);
Display_Result Display_Power_Off(void);

Display_Result Display_SetWindow(int16_t x1, int16_t x2, int16_t y1, int16_t y2);
Display_Result Display_Draw(int16_t x1, int16_t x2, int16_t y1, int16_t y2, Display_Color16_RGB565 *data);
Display_Result Display_DrawPixel(int16_t x, int16_t y, Display_Color16_RGB565 *data);

Display_Result Display_VerticalScroll_Definition(uint16_t topFixedLines, uint16_t scrollLines, uint16_t bottomFixedLines);
Display_Result Display_VerticalScroll_StartLine(uint16_t startLine);

#endif