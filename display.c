/**
 * @file    display.c
 * @authors  Bryson Chen & Dylan Carlson
 * @date    4 May 2024
 *
 * This module defines all functions, variables and macros for displaying information on the OLED
 *
*/


#include <stdint.h>

#include "inc/hw_memmap.h"

#include "utils/ustdlib.h"
#include "OrbitOLED/OrbitOLEDInterface.h"

//********************************************************
// Macros
//********************************************************


#define YAW_DISPLAY_STRING "Yaw = %3d.%03d  "

//********************************************************
// Global variables
//********************************************************
int32_t yawAngle;
uint16_t yawAngleSubDegree;
int16_t yawCount;
int32_t yawState;
//********************************************************
// Functions
//********************************************************


// Function for initialising the Orbit OLED display
void
initDisplay (void)
{
    OLEDInitialise ();
}


// Function for clearing the display
void
clearDisplay(void) {
    char string[17] = "                ";
    OLEDStringDraw (string, 0, 0);
    OLEDStringDraw (string, 0, 1);
    OLEDStringDraw (string, 0, 2);
    OLEDStringDraw (string, 0, 3);
}


// Function for displaying the mean value on the display
void
displayMeanVal(uint16_t meanVal, uint32_t count)
{

    char string[17];  // 16 characters across the display

    OLEDStringDraw ("ADC demo 1", 0, 0);

    usnprintf (string, sizeof(string), "Mean = %4d", meanVal);

    OLEDStringDraw (string, 0, 1);

    usnprintf (string, sizeof(string), "Sample# %5d", count);
    OLEDStringDraw (string, 0, 3);
}


// Function for displaying the mean value on the display
void
displayAltitude(int16_t percentageAltitude)
{

    char string[17];

    usnprintf (string, sizeof(string), "altitude = %4d", percentageAltitude);

    OLEDStringDraw (string, 0, 2);

}


// Function for displaying the yaw angle on the display
void
displayYawAngle(int32_t yawAngle, uint16_t yawAngleSubDegree)
{
    char string[17];

    usnprintf (string, sizeof(string), YAW_DISPLAY_STRING, yawAngle, yawAngleSubDegree);

    OLEDStringDraw (string, 0, 3);
}

