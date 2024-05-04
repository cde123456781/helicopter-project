/**
 * @file    altitude.h
 * @authors  Bryson Chen & Dylan Carlson
 * @date    30 April 2024
 *
 * This header file contains all definitions for functions, variables and macros for altitude related calculations of the ENCE361 Helicopter project
 *
*/


#ifndef ALTITUDE_H
#define ALTITUDE_H

#include <stdint.h>



//********************************************************
// Global variables
//********************************************************
int16_t percentageAltitude;

uint16_t helicopterLandedValue;
//********************************************************
// Prototypes
//********************************************************

// Set the helicopter's grounded value
void setHelicopterLandedValue (uint16_t landedValue);

// Calculate percentage altitude
void calcPercentageAltitude(uint16_t meanVal, uint16_t volt);



#endif //ALTITUDE_H
