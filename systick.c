/**
 * @file    systick.c
 * @authors  Bryson Chen & Dylan Carlson
 * @date    4 May 2024
 *
 * This module defines all functions, variables and macros for the systick clock
 *
*/


#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>
#include <stdbool.h>

#include "driverlib/adc.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "buttons4.h"



//********************************************************
// Macros
//********************************************************


#define SYSTICK_RATE_HZ 100
#define SLOWTICK_RATE_HZ 4

//********************************************************
// Global variables
//********************************************************
uint32_t g_ulSampCnt;
volatile uint8_t slowTick = false;


//********************************************************
// Functions
//********************************************************


// This function handles systick interrupts
void
SysTickIntHandler(void)
{
    //
    // Initiate a conversion
    //
    ADCProcessorTrigger(ADC1_BASE, 3);
    g_ulSampCnt++;

    static uint8_t tickCount = 0;
    const uint8_t ticksPerSlow = SYSTICK_RATE_HZ / SLOWTICK_RATE_HZ;

    updateButtons ();       // Poll the buttons
    if (++tickCount >= ticksPerSlow)
    {                       // Signal a slow tick
        tickCount = 0;
        slowTick = true;
    }
}


// This function initialises the systick timer and interrupts
void
initClock (void)
{
    // Set the clock rate to 20 MHz
    SysCtlClockSet (SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
    //
    // Set up the period for the SysTick timer.  The SysTick timer period is
    // set as a function of the system clock.
    SysTickPeriodSet(SysCtlClockGet() / SAMPLE_RATE_HZ);
    //
    // Register the interrupt handler
    SysTickIntRegister(SysTickIntHandler);
    //
    //SysTickIntRegister(ButtonIntHandler);
    // Enable interrupt and device
    SysTickIntEnable();
    SysTickEnable();
}
