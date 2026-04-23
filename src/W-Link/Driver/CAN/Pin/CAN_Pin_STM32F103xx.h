
#ifndef CAN_PIN_STM32F103xx_H
#define CAN_PIN_STM32F103xx_H

#include "CAN_Pin_STM32.h"

typedef enum {
    CAN_Pinset_DEFAULT = 0,
    CAN_Pinset_MAX
} CAN_Pinset_t;

#ifndef CONFIG_CAN0_PINSET
#define CONFIG_CAN0_PINSET CAN_Pinset_DEFAULT
#endif //CONFIG_CAN0_PINSET

#ifndef CONFIG_CAN1_PINSET
#define CONFIG_CAN1_PINSET CAN_Pinset_DEFAULT
#endif //CONFIG_CAN1_PINSET

const CAN_Pinset_t CAN_Index_Map_Alt[hwCAN_Index_MAX] = {
    CONFIG_CAN0_PINSET,
    CONFIG_CAN1_PINSET
};

const CAN_Pin_Def CAN_Pin_Def_Table[hwCAN_Index_MAX][CAN_Pinset_MAX] =
{
    /* ================= CAN1 (CAN0) ================= */
    {
        /* DEFAULT */
        { hwGPIO_Pin_A12, hwGPIO_Pin_A11 },

        /* ALT */
        { hwGPIO_Pin_B9, hwGPIO_Pin_B8 },

        /* ALT */
        { hwGPIO_Pin_D1, hwGPIO_Pin_D0 },
    },

    /* ================= CAN2 (CAN1) ================= */
    {
        /* DEFAULT */
        { hwGPIO_Pin_B6,  hwGPIO_Pin_B5 },

        /* ALT */
        { hwGPIO_Pin_B13, hwGPIO_Pin_B12 },

        /* ALT */
        { hwGPIO_Pin_NC,  hwGPIO_Pin_NC },
    },
};

#endif //CAN_PIN_STM32F103xx_H