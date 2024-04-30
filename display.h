/**
 * @file    display.h
 * @authors  Bryson Chen & Dylan Carlson
 * @date    30 April 2024
 *
 * This header file contains all definitions for functions, variables and macros for displaying information in the ENCE361 Helicopter project
 *
*/


#ifndef DISPLAY_H
#define DISPLAY_H

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



#include "utils/ustdlib.h"
#include "OrbitOLED/OrbitOLEDInterface.h"

#define YAW_DISPLAY_STRING "Yaw = %3d.%03d  "

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

// Function for initialising the Orbit OLED display
void initDisplay (void);

// Function for clearing the display
void clearDisplay (void);

// Function for displaying the mean value
void displayMeanVal(uint16_t meanVal, uint32_t count);

void displayAltitude(void);

void displayYawAngle(void);

#endif //DISPLAY_H
