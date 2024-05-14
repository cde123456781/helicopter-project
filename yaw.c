/**
 * @file    yaw.c
 * @authors  Bryson Chen & Dylan Carlson
 * @date    29 April 2024
 *
 * This module defines all functions, variables and macros for yaw calculation of the ENCE361 Helicopter project
 *
*/


#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"

#include "stdio.h"
#include "stdlib.h"
#include "yaw.h"
#include "switch.h"
#include "uart.h"
#include "control.h"


// Yaw Constants
#define NUM_SLOTS 112
#define TRIGGERS_PER_SLOT 4

// Set in powers of 10 depending on how many floating points desired (min 10)
#define YAW_ANGLE_SCALE 1000

//********************************************************
// Global variables
//********************************************************
int32_t yawAngle;
uint16_t yawAngleSubDegree;
int16_t yawCount;
int32_t yawState;
int8_t isRefFound;
int32_t yawReference;
//********************************************************
// Prototypes
//********************************************************

//interrupt handler for finding yawState, defines direction of rotation via quadrature
void YawIntHandler(void)
{
    int32_t newState = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0 |GPIO_PIN_1);

    if ((yawState == 0b00) && (newState == 0b01)) {
        //acw
        yawCount --;
    }
    else if ((yawState == 0b01) && (newState == 0b11)) {
        //acw
        yawCount --;
    }
    else if ((yawState == 0b11) && (newState == 0b10)) {
        //acw
        yawCount --;
    }
    else if ((yawState == 0b10) && (newState == 0b00)) {
        //acw
        yawCount --;
    }
    else {
        //cw
        yawCount ++;
    }
    yawState = newState;

    GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_0 |GPIO_PIN_1);

}

//Calculates yaw angle range -180 - 180
void calculateYawAngle(void)
{
    //GPIOIntDisable(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 );

    // Splits the revolution into 448
    int32_t anglePerYawCount = 360 * YAW_ANGLE_SCALE / (NUM_SLOTS * TRIGGERS_PER_SLOT);
    int32_t totalNumTriggers = NUM_SLOTS * TRIGGERS_PER_SLOT;
    // This check is necessary because of the way modulo works in C
    if (yawCount > 0) {
        if ((yawCount % totalNumTriggers) > (totalNumTriggers / 2)) {
            yawAngle = ((yawCount % totalNumTriggers) - totalNumTriggers) * anglePerYawCount;
            yawAngleSubDegree = ( -1 * yawAngle) % YAW_ANGLE_SCALE;
        }
        //positive
        else {
            yawAngle = (yawCount % totalNumTriggers) * anglePerYawCount;
            yawAngleSubDegree = (yawAngle % YAW_ANGLE_SCALE);
        }
    } else {
        // Negative
        if ( ((-1 * yawCount) % totalNumTriggers) < (totalNumTriggers/ 2)) {
            yawAngle = -1 * ((-1 * yawCount) % totalNumTriggers) * anglePerYawCount;

            yawAngleSubDegree = (-1 * yawAngle) % YAW_ANGLE_SCALE;
        }
        //positive
        else {
            yawAngle = -1 * (((-1 * yawCount) % totalNumTriggers) - totalNumTriggers) * anglePerYawCount;

            yawAngleSubDegree = -1 * (-1 * yawAngle) % YAW_ANGLE_SCALE;
        }

    }

    yawAngle = yawAngle / 1000;
    //GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 );
}

// This interrupt is triggered when discoverReference is called when the helicopter is
// directly facing the reference point. Stops discoverReference, and sets yawReference and resets yawCount
void YawReferenceIntHandler(void)
{
    isRefFound = 1;
    yawCount = 0;
    yawReference = 0;

    tailSetPoint = 0;


    GPIOIntClear(GPIO_PORTC_BASE, GPIO_PIN_4);
    GPIOIntDisable(GPIO_PORTC_BASE, GPIO_PIN_4);
}

//initialise yaw interrupts
void initYawMonitor (void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB) || (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC)))
    {

    }

    //quadrature signals

    GPIOIntRegister(GPIO_PORTB_BASE, YawIntHandler);

    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_BOTH_EDGES);

    GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 );

    //reference signal

    GPIOIntRegister(GPIO_PORTC_BASE, YawReferenceIntHandler);

    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4);

    GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_4 , GPIO_FALLING_EDGE);

    GPIOIntEnable(GPIO_PORTC_BASE, GPIO_PIN_4 );
    isRefFound = 0;
}




// This function makes the helicopter continuously rotate until it has found the
// reference position via an interrupt
void
discoverReference(void)
{
    if (performReferenceSearchFlag)
    {
        performReferenceSearchFlag = false;
        if (isRefFound == 0)
        {
            tailSetPoint -= 2;

            if (tailSetPoint <= -180) {
                tailSetPoint = 180 + (tailSetPoint - -180) ;
            }
        }

    }


}


// This function sets the tailSetPoint to the yawReference which will cause
// the helicopter to move to the point via control functions
void
goToReference(void)
{

    if (performReferenceSearchFlag)
    {
        performReferenceSearchFlag = false;
        if (tailSetPoint != yawReference)
        {
            tailSetPoint = yawReference;
//            if (tailSensorValue < 0)
//            {
//                tailSetPoint += 1;
//            } else {
//                tailSetPoint -= 1;
//            }

        }

    }


}
