/**
 * @file    main.c
 * @authors  Bryson Chen & Dylan Carlson
 * @date    30 April 2024
 *
 * This is the main.c file for the ENCE361 helicopter control project.
 * Designed for use on the Tiva C series board (tm4c123gh6pm) by Texas Instruments
 *
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
//#include "inc/tm4c123gh6pm.h"
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
#include "yaw.h"
#include "adc.h"

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

#define BLUE_LED  GPIO_PIN_2

// ADC constants
#define BITS 40960 // 2^12 * 10
#define VOLT_RANGE 33 // 3.3v * 10


//// Yaw Constants
//#define NUM_SLOTS 112
//#define TRIGGERS_PER_SLOT 4

// Set in powers of 10 depending on how many floating points desired (min 10)
//#define YAW_ANGLE_SCALE 1000
#define YAW_DISPLAY_STRING "Yaw = %3d.%03d  "

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
//circBuf_t g_inBuffer;        // Buffer of size BUF_SIZE integers (sample values)
static uint32_t g_ulSampCnt;    // Counter for the interrupts

char statusStr[MAX_STR_LEN + 1];
volatile uint8_t slowTick = false;

int16_t percentageAltitude;

uint16_t helicopterLandedValue;

int32_t testcount = 0;


void
initDebugLED(void)
{
    // Enable GPIO Port F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Set up the specific port pin as medium strength current & pull-down config.
    // Refer to TivaWare peripheral lib user manual for set up for configuration options
    GPIOPadConfigSet(GPIO_PORTF_BASE, BLUE_LED, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPD);

    // Set data direction register as output
    GPIODirModeSet(GPIO_PORTF_BASE, BLUE_LED, GPIO_DIR_MODE_OUT);

    // Write a zero to the output pin 3 on port F
    GPIOPinWrite(GPIO_PORTF_BASE, BLUE_LED, 0x00);
}

void
debugLED(void)
{
    GPIOPinWrite(GPIO_PORTF_BASE,  BLUE_LED, BLUE_LED);
}

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
initDisplay (void)
{
    // intialise the Orbit OLED display
    OLEDInitialise ();
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

// Calc percentage altitude
void
calcPercentageAltitude(uint16_t meanVal, uint16_t volt)
{
    percentageAltitude = (((helicopterLandedValue - meanVal) * 100) / volt);
    //debugLED();

}

void
displayAltitude(void)
{

    char string[17];  // 16 characters across the display

    //OLEDStringDraw ("Altitude %", 0, 0);

    // Form a new string for the line.  The maximum width specified for the
    //  number field ensures it is displayed right justified.
    usnprintf (string, sizeof(string), "altitude = %4d", percentageAltitude);

    // Update line on display.
    OLEDStringDraw (string, 0, 2);

}

void
displayYawAngle(void)
{
    char string[17];  // 16 characters across the display

    //OLEDStringDraw ("Altitude %", 0, 0);

    // Form a new string for the line.  The maximum width specified for the
    //  number field ensures it is displayed right justified.
    usnprintf (string, sizeof(string), YAW_DISPLAY_STRING, yawAngle, yawAngleSubDegree);

    // Update line on display.
    OLEDStringDraw (string, 0, 3);
}

uint32_t
calcMean(void)
{

    uint16_t i;
    // Background task: calculate the (approximate) mean of the values in the
    // circular buffer
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
    initDebugLED();
    initButtons ();
    initClock ();
    initADC ();
    initDisplay ();
    initCircBuf (&g_inBuffer, BUF_SIZE);
    initYawMonitor();


    initialiseUSB_UART ();

    uint16_t meanVal;

    uint16_t volt = BITS/VOLT_RANGE; //approx 1241


    SysCtlDelay (SysCtlClockGet() / 6);

    meanVal = calcMean();
    setHelicopterLandedValue(meanVal);

    //
    // Enable interrupts to the processor.
    IntMasterEnable();
    while (1)
    {
        IntMasterDisable();
        calculateYawAngle();
        meanVal = calcMean();

        calcPercentageAltitude(meanVal, volt);
        IntMasterEnable();


        if (displayMode == 0)
         {
             displayAltitude();
             displayYawAngle();
         } else if (displayMode == 1) {
             displayMeanVal (meanVal, g_ulSampCnt);
         }

        if(checkButton (LEFT) == PUSHED)
        {
            //debugLED(); // LED not on upon start, but on after left button pressed
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

            usprintf (statusStr, "Mean=%4d sample# =%5d | \r\n", meanVal, g_ulSampCnt);
            UARTSend (statusStr);
            usprintf (statusStr, "Altitude=%4d | \r\n", percentageAltitude);
            UARTSend (statusStr);
            usprintf (statusStr, "buffVal=%4d | \r\n", ulValue);
            UARTSend (statusStr);


        }

    }
}


