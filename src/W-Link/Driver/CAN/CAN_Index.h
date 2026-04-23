
#ifndef CAN_INDEX_H
#define CAN_INDEX_H

typedef enum {
#if defined (CAN1_BASE)
    hwCAN_Index_0 = 0,
#endif
#if defined (CAN2_BASE)
    hwCAN_Index_1,
#endif
#if defined (CAN3_BASE)
    hwCAN_Index_2,
#endif
    hwCAN_Index_MAX
} hwCAN_Index;

#endif //CAN_INDEX_H