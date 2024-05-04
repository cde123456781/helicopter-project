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
#define SLOWTICK_RATE_HZ 4

#define SYSTICK_RATE_HZ 100
//********************************************************
// Global variables
//********************************************************

extern g_ulSampCnt;
volatile uint8_t slowTick;


//********************************************************
// Prototypes
//********************************************************

// The function handles systick interrupts
void SysTickIntHandler(void);


// This function initialises the systick timer and interrupts
void initClock (void)

#endif //SYSTICK_H
