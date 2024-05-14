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


#include "driverlib/pwm.h"

#include "driverlib/debug.h"


#include "yaw.h"
#include "adc.h"
#include "systick.h"
#include "altitude.h"
#include "display.h"
#include "control.h"
#include "pwm.h"
#include "uart.h"
#include "protocols.h"
#include "switch.h"
#include "buttons4.h"



#define BLUE_LED  GPIO_PIN_2


//*****************************************************************************
// Global variables
//*****************************************************************************


circBuf_t g_inBuffer;        // Buffer of size BUF_SIZE integers (sample values)

//used for debugging purposes only
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





int
main(void)
{

    initDebugLED();

    // Initialisation functions
    initButtons ();
    initSwitch();
    initClock ();
    initADC ();
    initDisplay ();
    initCircBuf (&g_inBuffer, BUF_SIZE);
    initYawMonitor();
    initialiseMainPWM ();
    initialiseTailPWM ();
    initProc();
    initialiseUSB_UART ();

    uint16_t meanVal;

    uint16_t volt = getVolt(); //approx 1241

    SysCtlDelay (SysCtlClockGet() / 6); // A short delay to allow initialisation to complete

    meanVal = calcMean();
    setHelicopterLandedValue(meanVal);




    IntMasterEnable();
    while (1)
    {
        IntMasterDisable();

        calculateYawAngle();
        tailSensorValue = yawAngle;

        meanVal = calcMean();

        calcPercentageAltitude(meanVal, volt);
        mainSensorValue = percentageAltitude;

        IntMasterEnable();

        // Determines if there has been a change in buttons or switches
        pollButtons();
        pollSwitch();

        determineProc();
        updateDisplay(percentageAltitude, yawAngle, yawAngleSubDegree, mainDutyCycle, tailDutyCycle);



        if ((!isLanding) && (!isTakingOff) && (!isHovering)) {
            tailDutyCycle = 0;
            mainDutyCycle = 0;
        } else {
            checkControlFlag();
        }

        displayUART (tailSetPoint, yawAngle,
                     mainSetPoint, percentageAltitude,
                     mainDutyCycle, tailDutyCycle, isLanding, isHovering, isTakingOff);



    }
}


