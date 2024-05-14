/**
 * @file    systick.h
 * @authors  Bryson Chen & Dylan Carlson
 * @date    30 April 2024
 *
 * This header file contains all definitions for functions, variables and macros for systick
*/


#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>
#include <stdbool.h>

#include "driverlib/adc.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "buttons4.h"
#include "inc/hw_memmap.h"
#include "pwm.h"
#include "uart.h"
#include "control.h"
#include "yaw.h"
#include "switch.h"
#include "display.h"
#include "protocols.h"


//********************************************************
// Macros
//********************************************************

#define SAMPLE_RATE_HZ 120
#define SYSTICK_RATE_HZ 100


//********************************************************
// Global variables
//********************************************************

uint32_t numSamples;


//********************************************************
// Prototypes
//********************************************************

// The function handles systick interrupts
void SysTickIntHandler(void);


// This function initialises the systick timer and interrupts
void initClock (void);

#endif //SYSTICK_H
