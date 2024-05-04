/**
 * @file    altitude.c
 * @authors  Bryson Chen & Dylan Carlson
 * @date    5 May 2024
 *
 * This modules contains all functions, variables and macros for altitude related calculations of the ENCE361 Helicopter project
 *
*/


#include <stdint.h>
#include "altitude.h"

//********************************************************
// Global variables
//********************************************************
int16_t percentageAltitude;

uint16_t helicopterLandedValue;
//********************************************************
// Prototypes
//********************************************************

// Set the helicopter's grounded value
void
setHelicopterLandedValue (uint16_t landedValue)
{
    helicopterLandedValue = landedValue;
}

// Calculate percentage altitude
void
calcPercentageAltitude(uint16_t meanVal, uint16_t volt)
{
    percentageAltitude = (((helicopterLandedValue - meanVal) * 100) / volt);

}


