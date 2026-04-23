
#ifndef PWM_DEF_H
#define PWM_DEF_H

#define PWM_MAX_DUTY  1000
#define PWM_MIN_DUTY  0

typedef enum hwPWM_OpResult_t
{
  hwPWM_OK = 0,
  hwPWM_InvalidParameter = -1,
  hwPWM_NotInit = -2,
  hwPWM_NotTurnOn = -3,
  hwPWM_SlaveTimeout = -4,
  hwPWM_HwError = -5,
  hwPWM_Unsupport = -6,
}hwPWM_OpResult;

typedef enum hwPWM_Step_Direction_t
{
  hwPWM_Step_Dir_Down = 0,
  hwPWM_Step_Dir_Up = 1,
  hwPWM_Step_Dir_MAX = 2
}hwPWM_Step_Direction;

#endif //PWM_DEF_H