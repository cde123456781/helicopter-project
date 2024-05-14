/**
 * @file    display.c
 * @authors  Bryson Chen & Dylan Carlson
 * @date    4 May 2024
 *
 * This module defines all functions, variables and macros for displaying information on the OLED
 *
*/


#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"

#include "utils/ustdlib.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "display.h"

//********************************************************
// Macros
//********************************************************


#define YAW_DISPLAY_STRING "Yaw=%3d.%03d deg " //3dp for YAW_ANGLE_SCALE = 1000
#define DISPLAY_RATE 32 //Measured in Hz

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
updateDisplay(int16_t percentageAltitude, int32_t yawAngle, uint16_t yawAngleSubDegree, uint8_t mainDutyCycle, uint8_t tailDutyCycle)
{

    if (updateDisplayFlag) {
        displayAltitude(percentageAltitude);
        displayYawAngle(yawAngle, yawAngleSubDegree);
        displayPWM(mainDutyCycle, tailDutyCycle);
        updateDisplayFlag = false;
    }
}


// Function for displaying the mean value on the display
void
displayAltitude(int16_t percentageAltitude)
{

    char string[17];

    usnprintf (string, sizeof(string), "altitude = %d %%  ", percentageAltitude);

    OLEDStringDraw (string, 0, 0);

}


// Function for displaying the yaw angle on the display
void
displayYawAngle(int32_t yawAngle, uint16_t yawAngleSubDegree)
{
    char string[17];

    usnprintf (string, sizeof(string), YAW_DISPLAY_STRING, yawAngle, yawAngleSubDegree);

    OLEDStringDraw (string, 0, 1);
}


// Function for displaying the PWM duty cycles for tail and main rotor on the display
void
displayPWM(uint8_t mainDutyCycle, uint8_t tailDutyCycle)
{
    char string[17];

    usnprintf (string, sizeof(string), "MainPWM = %d %%", mainDutyCycle);

    OLEDStringDraw (string, 0, 2);

    usnprintf (string, sizeof(string), "TailPWM = %d %%", tailDutyCycle);

    OLEDStringDraw (string, 0, 3);
}



