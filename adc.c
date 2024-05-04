/**
 * @file    adc.c
 * @authors  Bryson Chen & Dylan Carlson
 * @date    30 April 2024
 *
 * This header file contains all definitions for functions, variables and macros for ADC related calculations of the ENCE361 Helicopter project
 *
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "inc/hw_memmap.h"
#include "driverlib/adc.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"
#include "circBufT.h"
#include "adc.h"

#include "stdio.h"
#include "stdlib.h"
uint32_t ulValue;
circBuf_t g_inBuffer;

/*
The handler for an ADC interrupt. Reads a discrete sample and writes it to 
a circular buffer.
*/
void
ADCIntHandler(void)
{

    // Get the single sample from ADC1.  ADC_BASE is defined in
    // inc/hw_memmap.h
    ADCSequenceDataGet(ADC1_BASE, 3, &ulValue);
    //
    // Place it in the circular buffer (advancing write index)
    writeCircBuf (&g_inBuffer, ulValue);
    //

    // Clean up, clearing the interrupt
    ADCIntClear(ADC1_BASE, 3);
}

/*
Initialises the analog-to-digital converter.
*/
void initADC (void)
{
    //
    // The ADC1 peripheral must be enabled for configuration and use.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    // Enable sample sequence 3 with a processor signal trigger.  Sequence 3
    // will do a single sample when the processor sends a signal to start the
    // conversion.
    ADCSequenceConfigure(ADC1_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);


    // Configure step 0 on sequence 3.  Sample channel 9 (ADC_CTL_CH9) in
    // single-ended mode (default) and configure the interrupt flag
    // (ADC_CTL_IE) to be set when the sample is done.  Tell the ADC logic
    // that this is the last conversion on sequence 3 (ADC_CTL_END). 
    ADCSequenceStepConfigure(ADC1_BASE, 3, 0, ADC_CTL_CH9 | ADC_CTL_IE |
                             ADC_CTL_END);

    //Enable sequence 3 to ADC1_BASE
    ADCSequenceEnable(ADC1_BASE, 3);

    //
    // Register the interrupt handler
    ADCIntRegister (ADC1_BASE, 3, ADCIntHandler);

    //
    // Enable interrupts for ADC1 sequence 3 (clears any outstanding interrupts)
    ADCIntEnable(ADC1_BASE, 3);

}
