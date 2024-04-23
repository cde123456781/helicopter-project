//*****************************************************************************
//
// ADCdemo1.c - Simple interrupt driven program which samples with AIN0
//
// Author:  P.J. Bones  UCECE
// Last modified:   8.2.2018
//
//*****************************************************************************
// Based on the 'convert' series from 2016
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/pwm.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"
#include "utils/ustdlib.h"
#include "circBufT.h"
#include "OrbitOLED/OrbitOLEDInterface.h"

#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "buttons4.h"
#include "inc/hw_ints.h"
#include "stdio.h"
#include "stdlib.h"

//*****************************************************************************
// Constants
//*****************************************************************************
#define BUF_SIZE 10
#define SAMPLE_RATE_HZ 120

#define SYSTICK_RATE_HZ 100
#define SLOWTICK_RATE_HZ 4
#define MAX_STR_LEN 100
//---USB Serial comms: UART0, Rx:PA0 , Tx:PA1
#define BAUD_RATE 9600
#define UART_USB_BASE           UART0_BASE
#define UART_USB_PERIPH_UART    SYSCTL_PERIPH_UART0
#define UART_USB_PERIPH_GPIO    SYSCTL_PERIPH_GPIOA
#define UART_USB_GPIO_BASE      GPIO_PORTA_BASE
#define UART_USB_GPIO_PIN_RX    GPIO_PIN_0
#define UART_USB_GPIO_PIN_TX    GPIO_PIN_1
#define UART_USB_GPIO_PINS      UART_USB_GPIO_PIN_RX | UART_USB_GPIO_PIN_TX

#define BITS 40960 // 2^12 * 10
#define VOLTRANGE 33 // 3.3v * 10

//********************************************************
// Prototypes
//********************************************************
void SysTickIntHandler (void);
void initClock (void);
void initSysTick (void);
void initDisplay (void);
void initialiseUSB_UART (void);
void UARTSend (char *pucBuffer);
void displayButtonState (char *butStr, char *stateStr,
    uint8_t numPushes, uint8_t charLine);

//*****************************************************************************
// Global variables
//*****************************************************************************
static circBuf_t g_inBuffer;        // Buffer of size BUF_SIZE integers (sample values)
static uint32_t g_ulSampCnt;    // Counter for the interrupts

char statusStr[MAX_STR_LEN + 1];
volatile uint8_t slowTick = false;

int16_t percentage;
int16_t yawAngle;
int8_t yawSlot;

uint16_t helicopterLandedValue;

uint8_t state;


//*****************************************************************************
//
// The interrupt handler for the for SysTick interrupt.
//
//*****************************************************************************
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

/*void
ButtonIntHandler(void)
{
    updateButtons();
}*/

//*****************************************************************************
//
// The handler for the ADC conversion complete interrupt.
// Writes to the circular buffer.
//
//*****************************************************************************
void
ADCIntHandler(void)
{
    uint32_t ulValue;

    //
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

//*****************************************************************************
//
// The handler for Port B Interrupts
//
//*****************************************************************************
void
PortBIntHandler(void)
{
    uint8_t newState = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0 |GPIO_PIN_1)
    uint8_t a = newState & 0x01;
    uint8_t b = newState & 0x10;
    if ((state == 0x00) && (newState == 0x01)) {
        //cw
        yawSlot ++;
    }
    else if ((state == 0x01) && (newState == 0x11)) {
        //cw
        yawSlot ++;
    }
    else if ((state == 0x11) && (newState == 0x10)) {
        //cw
        yawSlot ++;
    }
    else if ((state == 0x10) && (newState == 0x00)) {
        //cw
        yawSlot ++;
    }
    else {
        //acw
        yawSlot --;
    }


}




//*****************************************************************************
// Initialisation functions for the clock (incl. SysTick), ADC, display
//*****************************************************************************
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

void
initADC (void)
{
    //
    // The ADC1 peripheral must be enabled for configuration and use.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    // Enable sample sequence 3 with a processor signal trigger.  Sequence 3
    // will do a single sample when the processor sends a signal to start the
    // conversion.
    ADCSequenceConfigure(ADC1_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);


    //
    // Configure step 0 on sequence 3.  Sample channel 9 (ADC_CTL_CH9) in
    // single-ended mode (default) and configure the interrupt flag
    // (ADC_CTL_IE) to be set when the sample is done.  Tell the ADC logic
    // that this is the last conversion on sequence 3 (ADC_CTL_END).  Sequence
    // 3 has only one programmable step.  Sequence 1 and 2 have 4 steps, and
    // sequence 0 has 8 programmable steps.  Since we are only doing a single
    // conversion using sequence 3 we will only configure step 0.  For more
    // on the ADC sequences and steps, refer to the LM3S1968 datasheet.
    ADCSequenceStepConfigure(ADC1_BASE, 3, 0, ADC_CTL_CH9 | ADC_CTL_IE |
                             ADC_CTL_END);

    //
    // Since sample sequence 3 is now configured, it must be enabled.
    ADCSequenceEnable(ADC1_BASE, 3);

    //
    // Register the interrupt handler
    ADCIntRegister (ADC1_BASE, 3, ADCIntHandler);

    //
    // Enable interrupts for ADC1 sequence 3 (clears any outstanding interrupts)
    ADCIntEnable(ADC1_BASE, 3);
}

void
initDisplay (void)
{
    // intialise the Orbit OLED display
    OLEDInitialise ();
}


void initYawMonitor (void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);


    GPIOIntRegister(GPIO_PORTB_BASE, PortBIntHandler);

    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_BOTH_EDGES);

    GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 );



}

void
clearDisplay(void) {
    char string[17] = "                ";
    OLEDStringDraw (string, 0, 0);
    OLEDStringDraw (string, 0, 1);
    OLEDStringDraw (string, 0, 2);
    OLEDStringDraw (string, 0, 3);
}

//*****************************************************************************
//
// Function to display the mean ADC value (10-bit value, note) and sample count.
//
//*****************************************************************************
void
displayMeanVal(uint16_t meanVal, uint32_t count)
{

    char string[17];  // 16 characters across the display

    OLEDStringDraw ("ADC demo 1", 0, 0);

    // Form a new string for the line.  The maximum width specified for the
    //  number field ensures it is displayed right justified.
    usnprintf (string, sizeof(string), "Mean = %4d", meanVal);

    // Update line on display.
    OLEDStringDraw (string, 0, 1);

    usnprintf (string, sizeof(string), "Sample# %5d", count);
    OLEDStringDraw (string, 0, 3);
}

void
initialiseUSB_UART (void)
{
    //
    // Enable GPIO port A which is used for UART0 pins.
    //
    SysCtlPeripheralEnable(UART_USB_PERIPH_UART);
    SysCtlPeripheralEnable(UART_USB_PERIPH_GPIO);
    //
    // Select the alternate (UART) function for these pins.
    //
    GPIOPinTypeUART(UART_USB_GPIO_BASE, UART_USB_GPIO_PINS);
    GPIOPinConfigure (GPIO_PA0_U0RX);
    GPIOPinConfigure (GPIO_PA1_U0TX);

    UARTConfigSetExpClk(UART_USB_BASE, SysCtlClockGet(), BAUD_RATE,
            UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
            UART_CONFIG_PAR_NONE);
    UARTFIFOEnable(UART_USB_BASE);
    UARTEnable(UART_USB_BASE);
}


//**********************************************************************
// Transmit a string via UART0
//**********************************************************************
void
UARTSend (char *pucBuffer)
{
    // Loop while there are more characters to send.
    while(*pucBuffer)
    {
        // Write the next character to the UART Tx FIFO.
        UARTCharPut(UART_USB_BASE, *pucBuffer);
        pucBuffer++;
    }
}




// Set the helicopter's grounded value
void
setHelicopterLandedValue (uint16_t landedValue)
{
    helicopterLandedValue = landedValue;
}

// Calc percentage
void
calcPercentage(uint16_t meanVal, uint16_t volt)
{
    percentage = (((helicopterLandedValue - meanVal) * 100) / volt);


}

void
displayAltitude(void)
{

    char string[17];  // 16 characters across the display

    OLEDStringDraw ("Altitude %", 0, 0);

    // Form a new string for the line.  The maximum width specified for the
    //  number field ensures it is displayed right justified.
    usnprintf (string, sizeof(string), "altitude = %4d", percentage);

    // Update line on display.
    OLEDStringDraw (string, 0, 2);

}

uint32_t
calcMean(void)
{
    uint16_t i;
    // Background task: calculate the (approximate) mean of the values in the
    // circular buffer and display it, together with the sample number.
    uint16_t sum = 0;
    for (i = 0; i < BUF_SIZE; i++)
        sum = sum + readCircBuf (&g_inBuffer);
    // Calculate and display the rounded mean of the buffer contents

    //displayMeanVal (meanVal, g_ulSampCnt);
    return (2 * sum + BUF_SIZE) / 2 / BUF_SIZE;
}


int
main(void)
{

    uint8_t displayMode = 0;

    initButtons ();
    initClock ();
    initADC ();
    initDisplay ();
    initCircBuf (&g_inBuffer, BUF_SIZE);


    initialiseUSB_UART ();

    uint16_t meanVal;

    uint16_t volt = BITS/VOLTRANGE; //approx 1241

    //
    // Enable interrupts to the processor.
    IntMasterEnable();
    SysCtlDelay (SysCtlClockGet() / 6);
    meanVal = calcMean();
    setHelicopterLandedValue(meanVal);

    while (1)
    {


        meanVal = calcMean();

        calcPercentage(meanVal, volt);


        if (displayMode == 0)
         {
             displayAltitude();
         } else if (displayMode == 1) {
             displayMeanVal (meanVal, g_ulSampCnt);
         }

        if(checkButton (LEFT) == PUSHED)
        {
            setHelicopterLandedValue(meanVal);
        }

        if(checkButton (UP) == PUSHED)
        {
            clearDisplay();
            displayMode++;
            displayMode = displayMode % 3;
        }



        SysCtlDelay (SysCtlClockGet() / 96);  // Update display at ~ 32 Hz

        if (slowTick)
        {
            slowTick = false;
            // Form and send a status message to the console
            //usprintf (statusStr, "Mean=%2d samples=%2d | \r\n", 0, g_ulSampCnt);
            usprintf (statusStr, "Mean=%4d sample# =%5d | \r\n", meanVal, g_ulSampCnt);
            UARTSend (statusStr);
        }
    }
}

