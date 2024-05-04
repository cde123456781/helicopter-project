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
#include "circBufT.h"

#include "stdio.h"
#include "stdlib.h"


//********************************************************
// Global variables
//********************************************************
uint32_t ulValue;
//********************************************************
// Prototypes
//********************************************************
/*
The handler for an ADC interrupt. Reads a discrete sample and writes it to 
a circular buffer.
*/
void ADCIntHandler(void);

/*
Initialises the analog-to-digital converter.
*/
void initADC (void);



#endif //ADC_H
