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
#include "driverlib/debug.h"

#include "stdio.h"




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

void
initDisplay (void)
{
    // intialise the Orbit OLED display
    OLEDInitialise ();
}

void
clearDisplay(void) {
    // Set each line on the OLED to blank
    char string[17] = "                ";
    OLEDStringDraw (string, 0, 0);
    OLEDStringDraw (string, 0, 1);
    OLEDStringDraw (string, 0, 2);
    OLEDStringDraw (string, 0, 3);
}



void
displayMeanVal(uint16_t meanVal, uint32_t count)
{

    char string[17];  // 16 characters across the display

    OLEDStringDraw ("ADC demo 1", 0, 0);

    // Form a new string for the line.  The maximum width specified for the
    //  number field ensures it is displayed right justified.
    usnprintf (string, sizeof(string), "Mean = %4d", meanVal);

    // Update line on display.
    OLEDStringDraw (string, 0, 1);

    usnprintf (string, sizeof(string), "Sample# %5d", count);
    OLEDStringDraw (string, 0, 3);
}


void
displayAltitude(int16_t percentageAltitude)
{

    char string[17];  // 16 characters across the display

    //OLEDStringDraw ("Altitude %", 0, 0);

    // Form a new string for the line.  The maximum width specified for the
    //  number field ensures it is displayed right justified.
    usnprintf (string, sizeof(string), "altitude = %4d", percentageAltitude);

    // Update line on display.
    OLEDStringDraw (string, 0, 2);

}



void
displayYawAngle(int32_t yawAngle, uint16_t yawAngleSubDegree)
{
    char string[17];  // 16 characters across the display

    //OLEDStringDraw ("Altitude %", 0, 0);

    // Form a new string for the line.  The maximum width specified for the
    //  number field ensures it is displayed right justified.
    usnprintf (string, sizeof(string), YAW_DISPLAY_STRING, yawAngle, yawAngleSubDegree);

    // Update line on display.
    OLEDStringDraw (string, 0, 3);
}

