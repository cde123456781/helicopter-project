/**
 * @file    adc.h
 * @authors  Bryson Chen & Dylan Carlson
 * @date    30 April 2024
 *
 * This header file contains all definitions for functions, variables and macros for ADC related calculations of the ENCE361 Helicopter project
 *
*/


#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "inc/hw_memmap.h"
#include "driverlib/adc.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"

#include "stdio.h"
#include "stdlib.h"


#define BITS 40960 // 2^12 * 10
#define VOLT_RANGE 33 // 3.3v * 10



//********************************************************
// Global variables
//********************************************************

//********************************************************
// Prototypes
//********************************************************


#endif //ADC_H
