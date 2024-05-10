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
//#include "switch.h"



#define BLUE_LED  GPIO_PIN_2


//*****************************************************************************
// Global variables
//*****************************************************************************

// Keeping this here instead of having it in circBufT.c cause that's made by some other fucker
circBuf_t g_inBuffer;        // Buffer of size BUF_SIZE integers (sample values)


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



// Could probably be moved to a buttons.c module
void
pollButtons(void)
{
    if (checkButton (RIGHT) == PUSHED)
     {
        tailSetPoint -= 15.0;

        if (tailSetPoint <= -180) {
            tailSetPoint = 180 + (tailSetPoint - -180) ;
        }

     }

    if(checkButton (LEFT) == PUSHED)
    {
        //debugLED(); // LED not on upon start, but on after left button pressed
        tailSetPoint += 15.0;
        if (tailSetPoint > 180) {

            tailSetPoint = -180 + (tailSetPoint - 180) ;
        }
    }

    if(checkButton (UP) == PUSHED)
    {
        mainSetPoint += 10.0;
        if (mainSetPoint > 100) {
            mainSetPoint = 100;
        }
    }

    if(checkButton (DOWN) == PUSHED)
    {
        mainSetPoint -= 10.0;
        if (mainSetPoint < 0) {
            mainSetPoint = 0;
        }
    }
}

void takeOff(void)
{

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
    initialiseMainPWM ();
    initialiseTailPWM ();


    initialiseUSB_UART ();

    uint16_t meanVal;

    uint16_t volt = getVolt(); //approx 1241

    SysCtlDelay (SysCtlClockGet() / 6);

    meanVal = calcMean();
    setHelicopterLandedValue(meanVal);

    enableTailPWMOutput();
    enableMainPWMOutput();


    //for testing
    mainSetPoint = 51;
    // Enable interrupts to the processor.
    checkRefStartup();
    IntMasterEnable();
//    discoverReference();
//    goToReference();
    while (1)
    {
        IntMasterDisable();

        calculateYawAngle();
        tailSensorValue = yawAngle;

        meanVal = calcMean();

        calcPercentageAltitude(meanVal, volt);
        mainSensorValue = percentageAltitude;
        IntMasterEnable();
        discoverReference();

        pollButtons();
        if (displayMode == 0)
         {
             displayAltitude(percentageAltitude);
             displayYawAngle(yawAngle, yawAngleSubDegree);
             displayPWM(mainDutyCycle, tailDutyCycle);
         } else if (displayMode == 1) {
             displayMeanVal (meanVal, numSamples);
         }
//
//        if(checkButton (LEFT) == PUSHED)
//        {
//            //debugLED(); // LED not on upon start, but on after left button pressed
//            setHelicopterLandedValue(meanVal);
//        }
//
//        if(checkButton (UP) == PUSHED)
//        {
//            clearDisplay();
//            displayMode++;
//            displayMode = displayMode % 3;
//        }

        checkControlFlag();

        SysCtlDelay (SysCtlClockGet() / 96);  // Update display at ~ 32 Hz

//        displayUART (tailSetPoint, yawAngle,
//                     mainSetPoint, percentageAltitude,
//                     mainDutyCycle, tailDutyCycle, 1);



    }
}


