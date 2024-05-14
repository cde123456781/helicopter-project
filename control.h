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

volatile bool landingTimeFlag;

//*****************************************************************************
// Constants
//*****************************************************************************

#define KU_MAIN 10
//#define MAIN_PERIOD 0.1
#define KP_MAIN 2//(0.6 * KU_MAIN) 5, 4, 3
#define KI_MAIN 0.2//(2 * KU_MAIN)/MAIN_PERIOD //0.05, 0.5
#define KD_MAIN 0//0.075 * KU_MAIN * MAIN_PERIOD

#define KP_TAIL 2 // SET TO 5 before 8, 4, 3
#define KI_TAIL 0.3 // 0.125, 0.5, 0.6
#define KD_TAIL 0

#define DELTA_T 0.01


#define MAX_OUTPUT 70 // HeliSims are limited to 70 (lect 11.20). May need to change this for
                      // actual?
#define MIN_OUTPUT 2

#define GRAVITY 33 // WAS 41 before
#define COUPLING 0.80

#define FALL_TICKS 50
//*****************************************************************************
// Functions
//*****************************************************************************



/*
 * Get the desired duty cycle for the main motor PWM
 */
float
getMainDutyCycle (float setPoint, float sensorValue);



/*
 * Get the desired duty cycle for the tail motor PWM
 */
float
getTailDutyCycle (float setPoint, float sensorValue, float mainDutyCycle);



/*
 * Adjusts the PWM for both the main and tail motors at a set schedule when flag is set by systick
 */
void
checkControlFlag(void);


#endif //CONTROL_H











