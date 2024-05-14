/**
 * @file    systick.c
 * @authors  Bryson Chen & Dylan Carlson
 * @date    4 May 2024
 *
 * This module defines all functions, variables and macros for the systick clock
 *
*/


#include <stdint.h>
#include <stdbool.h>

#include "driverlib/adc.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "buttons4.h"
#include "inc/hw_memmap.h"
#include "systick.h"
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


#define SYSTICK_RATE_HZ 100
#define SAMPLE_RATE_HZ 120


//********************************************************
// Global variables
//********************************************************
uint32_t numSamples;


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
    numSamples++;

    controlFlag = true;

    static uint8_t UARTTickCount = 0;
    static uint8_t yawTickCount = 0;
    static uint8_t displayTickCount = 0;
    static uint8_t fallTickCount = 0;
    const uint8_t ticksForUART = SYSTICK_RATE_HZ / UART_DISPLAY_RATE;
    const uint8_t ticksForDisplay = SYSTICK_RATE_HZ / DISPLAY_RATE;


    updateButtons ();       // Poll the buttons


    updateSwitch();

    if (++displayTickCount >= ticksForDisplay)
    {
        displayTickCount = 0;
        updateDisplayFlag = true;
    }


    if (++UARTTickCount >= ticksForUART)
    {                       // Signal a slow tick
        UARTTickCount = 0;
        sendUARTFlag = true;
    }

    if (++yawTickCount >= YAW_TICKS)
        {                       // Signal a slow tick
            yawTickCount = 0;
            performReferenceSearchFlag = true;

        }

    if (++fallTickCount >= FALL_TICKS )
    {
        fallTickCount = 0;
        landingTimeFlag = true;
    }

}


// This function initialises the systick timer and interrupts
void
initClock (void)
{
    // Set the clock rate to 20 MHz
    SysCtlClockSet (SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);

    // Set the PWM clock rate (using the prescaler)
    SysCtlPWMClockSet(PWM_DIVIDER_CODE);

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
