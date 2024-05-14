/**
 * @file    yaw.h
 * @authors  Bryson Chen & Dylan Carlson
 * @date    29 April 2024
 *
 * This header file contains all definitions for functions, variables and macros for yaw calculation of the ENCE361 Helicopter project
 *
*/


#ifndef YAW_H
#define YAW_H

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
#include "control.h"


// Yaw Constants
#define NUM_SLOTS 112
#define TRIGGERS_PER_SLOT 4

// Set in powers of 10 depending on how many floating points desired (min 10)
#define YAW_ANGLE_SCALE 1000
#define YAW_TICKS 40

//********************************************************
// Global variables
//********************************************************
int32_t yawAngle;
uint16_t yawAngleSubDegree;
int16_t yawCount;
int32_t yawState;
int8_t isRefFound;
int32_t yawReference;

volatile uint8_t performReferenceSearchFlag;
//********************************************************
// Prototypes
//********************************************************

//interrupt handler for finding yawState, defines direction of rotation via quadrature
void YawIntHandler(void);

//Calculates yaw angle range -180 - 180
void calculateYawAngle(void);

//initialise yaw interrupts
void initYawMonitor (void);

// This interrupt is triggered when discoverReference is called when the helicopter is
// directly facing the reference point. Stops discoverReference, and sets yawReference and resets yawCount
void YawReferenceIntHandler(void);


// This function sets the tailSetPoint to the yawReference which will cause
// the helicopter to move to the point via control functions
void
goToReference(void);


// This function makes the helicopter continuously rotate until it has found the
// reference position via an interrupt
void
discoverReference(void);

// deprecated func
//void checkRefStartup(void);
#endif //YAW_H
