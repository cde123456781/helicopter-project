#ifndef CONTROL_H
#define CONTROL_H

/**
 * @file    control.H
 * @authors  Bryson Chen & Dylan Carlson
 * @date    7 May 2024
 *
 * This file contains all definitions for functions, variables and macros for the controls for the motors
*/

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h" //Needed for pin configure
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"


#include "stdlib.h"
#include "pwm.h"

//********************************************************
// Global variables
//********************************************************

uint8_t tailDutyCycle;
uint8_t mainDutyCycle;

float setpoint;
float sensorValue;

//*****************************************************************************
// Constants
//*****************************************************************************

#define KP_MAIN
#define KI_MAIN
#define KD_MAIN

#define KP_TAIL
#define KI_TAIL
#define KD_TAIL

#define DELTA_T


#define MAX_OUTPUT 70 // HeliSims are limited to 70 (lect 11.20). May need to change this for
                      // actual?
#define MIN_OUTPUT 0

#define GRAVITY 33
#define COUPLING 0.88

//*****************************************************************************
// Functions
//*****************************************************************************

float
getMainDutyCycle (float setPoint, float sensorValue);

float
getTailDutyCycle (float setPoint, float sensorValue, float mainDutyCycle);

#endif //CONTROL_H











