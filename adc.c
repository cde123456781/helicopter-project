/**
 * @file    adc.c
 * @authors  Bryson Chen & Dylan Carlson
 * @date    30 April 2024
 *
 * This header file contains all definitions for functions, variables and macros for ADC related calculations of the ENCE361 Helicopter project
 *
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



//********************************************************
// Global variables
//********************************************************

uint32_t ulValue;
circBuf_t g_inBuffer;



//*****************************************************************************
// Constants
//*****************************************************************************

#define BUF_SIZE 10
#define BITS 40960 // 2^12 * 10
#define VOLT_RANGE 33 // 3.3v * 10

//*****************************************************************************
// Functions
//*****************************************************************************

/*
The handler for an ADC interrupt. Reads a discrete sample and writes it to 
a circular buffer.
*/
void
ADCIntHandler(void)
{

    ADCSequenceDataGet(ADC1_BASE, 3, &ulValue);

    writeCircBuf (&g_inBuffer, ulValue);

    ADCIntClear(ADC1_BASE, 3);
}

/*
Initialises and configures the analog-to-digital converter.
*/
void initADC (void)
{

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    ADCSequenceConfigure(ADC1_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

    ADCSequenceStepConfigure(ADC1_BASE, 3, 0, ADC_CTL_CH9 | ADC_CTL_IE |
                             ADC_CTL_END);

    ADCSequenceEnable(ADC1_BASE, 3);

    ADCIntRegister (ADC1_BASE, 3, ADCIntHandler);

    ADCIntEnable(ADC1_BASE, 3);

}



/*
 * Calculates the mean of the values stored in the circular buffer
 */
uint32_t calcMean(void)
{
//credit to ENCE361 labs author P.J. Bones UCECE
    uint16_t i;

    uint16_t sum = 0;
    for (i = 0; i < BUF_SIZE; i++)
        sum = sum + readCircBuf (&g_inBuffer);

    return (2 * sum + BUF_SIZE) / 2 / BUF_SIZE;
}


/*
 * Calculates and returns the ADC value for 1V
 */
uint16_t getVolt()
{
    return BITS/VOLT_RANGE;
}


