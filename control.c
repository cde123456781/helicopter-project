/**
 * @file    control.c
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
#include "control.h"


//********************************************************
// Global variables
//********************************************************

uint8_t tailDutyCycle;
uint8_t mainDutyCycle;



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


/*
 * Get the desired duty cycle for the main motor PWM
 */
float getMainDutyCycle (float setPoint, float sensorValue)
{
    static float integral = 0;
    static float previousSensorValue = 0;

    float error = setPoint - sensorValue;
    float proportionalTerm = KP_MAIN * error;
    float integralTerm = KI_MAIN * error * DELTA_T;
    float derivativeTerm = KD_MAIN * (previousSensorValue - sensorValue) / DELTA_T;

    float duty = proportionalTerm + (integral + integralTerm) + derivativeTerm + GRAVITY;

    // Saturation checks
    if (duty > MAX_OUTPUT) {
        duty = MAX_OUTPUT;
    } else if (duty < MIN_OUTPUT) {
        duty = MIN_OUTPUT;
    } else {
        integral += integralTerm;
    }

    previousSensorValue = sensorValue;

    return duty;

}


/*
 * Get the desired duty cycle for the tail motor PWM
 */
float getTailDutyCycle (float setPoint, float sensorValue, float mainDutyCycle)
{
    static float integral = 0;
    static float previousSensorValue = 0;

    float error = setPoint - sensorValue;
    float proportionalTerm = KP_TAIL * error;
    float integralTerm = KI_TAIL * error * DELTA_T;
    float derivativeTerm = KD_TAIL * (previousSensorValue - sensorValue) / DELTA_T;

    float duty = proportionalTerm + (integral + integralTerm) + derivativeTerm + COUPLING * mainDutyCycle;


    // Saturation checks
    if (duty > MAX_OUTPUT) {
        duty = MAX_OUTPUT;
    } else if (duty < MIN_OUTPUT) {
        duty = MIN_OUTPUT;
    } else {
        integral += integralTerm;
    }

    previousSensorValue = sensorValue;

    return duty;

}






