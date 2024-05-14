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
// Constants
//********************************************************

#define MAX_OUTPUT 70

#define MIN_OUTPUT 2


//********************************************************
// Global variables
//********************************************************

uint8_t tailDutyCycle;
uint8_t mainDutyCycle;

float tailSetPoint; //setPoint can be described as desired yaw.
float tailSensorValue;  //sensor value can be described as current yaw.

float mainSetPoint;
float mainSensorValue;


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
    if (error > 10) {
        error = 10;
    } else if (error < -10) {
        error = -10;
    }

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
    static float error = 0;
    static float integral = 0;
    static float previousSensorValue = 0;

    error = setPoint - sensorValue;
    if (error > 180) {
        error = (error - 360);
    } else if (error < -180) {
        error = (error + 360);
    }

    if (error < -15) {
        error = -15;
    } else if (error > 15) {
        error = 15;
    }



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


/*
 * Adjusts the PWM for both the main and tail motors at a set schedule when flag is set by systick
 */
void
checkControlFlag(void)
{
    if (controlFlag == true)
    {
        mainDutyCycle = getMainDutyCycle (mainSetPoint, mainSensorValue);

        tailDutyCycle = getTailDutyCycle (tailSetPoint, tailSensorValue, mainDutyCycle);
        setTailPWM(tailDutyCycle);
        setMainPWM(mainDutyCycle);
    }


}







