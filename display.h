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

#include "inc/hw_memmap.h"


#include "utils/ustdlib.h"
#include "OrbitOLED/OrbitOLEDInterface.h"

//********************************************************
// Macros
//********************************************************


#define YAW_DISPLAY_STRING "Yaw=%3d.%03d deg " //3dp for YAW_ANGLE_SCALE = 1000


//********************************************************
// Prototypes
//********************************************************

// Function for initialising the Orbit OLED display
void initDisplay (void);

// Function for clearing the display
void clearDisplay (void);

// Function for displaying the mean value on the display
void displayMeanVal(uint16_t meanVal, uint32_t count);

// Function for displaying the altitude on the display
void displayAltitude(int16_t percentageAltitude);

// Function for displaying the yaw angle on the displa
void displayYawAngle(int32_t yawAngle, uint16_t yawAngleSubDegree);

// Function for displaying the PWM duty cycles for tail and main rotor on the display
void displayPWM(uint8_t mainDutyCycle, uint8_t tailDutyCycle);

#endif //DISPLAY_H
