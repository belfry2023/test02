#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__
#include "main.h"

typedef struct
{
    float kp; // Proportional gain
    float ki; // Integral gain
    float kd; // Derivative gain
    float setpoint; // Desired value
    float integral; // Integral term
    float previous_error; // Previous error for derivative calculation
} PIDController;

#endif // __CONTROLLER_H__