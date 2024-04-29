/**
 * @file    yaw.h
 * @authors  Bryson Chen & Dylan Carlson
 * @date    29 April 2024
 *
 * This module defines all functions for yaw calculation of the ENCE361 Helicopter project
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

//initialise yaw interrupts
void initYawMonitor (void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
    {

    }

    GPIOIntRegister(GPIO_PORTB_BASE, YawIntHandler);

    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_BOTH_EDGES);

    GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 );

}
