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
volatile uint8_t controlFlag;

float tailSetPoint;     //used for control functions
float tailSensorValue;  //

float mainSetPoint;     //used for control functions
float mainSensorValue;  //

//*****************************************************************************
// Constants
//*****************************************************************************

#define KU_MAIN 10
//#define MAIN_PERIOD 0.1
#define KP_MAIN 10//(0.6 * KU_MAIN)
#define KI_MAIN 0//(2 * KU_MAIN)/MAIN_PERIOD
#define KD_MAIN 0//0.075 * KU_MAIN * MAIN_PERIOD

#define KP_TAIL 0
#define KI_TAIL 0
#define KD_TAIL 0

#define DELTA_T 0.01


#define MAX_OUTPUT 70 // HeliSims are limited to 70 (lect 11.20). May need to change this for
                      // actual?
#define MIN_OUTPUT 0

#define GRAVITY 41
#define COUPLING 0.81

//*****************************************************************************
// Functions
//*****************************************************************************

float
getMainDutyCycle (float setPoint, float sensorValue);

float
getTailDutyCycle (float setPoint, float sensorValue, float mainDutyCycle);

#endif //CONTROL_H











