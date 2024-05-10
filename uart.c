/**
 * @file    uart.c
 * @authors  Bryson Chen & Dylan Carlson
 * @date    9 May 2024
 *
 * This module defines all functions, variables and macros for UART
 *
*/


#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#include "utils/ustdlib.h"

#include "uart.h"

//*****************************************************************************
// Constants
//*****************************************************************************


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


#define UART_DISPLAY_RATE 4


//********************************************************
// Functions
//********************************************************


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


void
displayUART (int32_t desiredYaw, int32_t actualYaw, int16_t desiredAltitude, int16_t actualAltitude, uint8_t mainDuty, uint8_t tailDuty, uint8_t operatingMode)
{
    if (sendUARTFlag)
        {
            char statusStr[MAX_STR_LEN + 1];

            sendUARTFlag = false;
            // Form and send a status message to the console
            usprintf (statusStr, "DesiredYaw=%4d deg ActualYaw=%5d deg | \r\n", desiredYaw, actualYaw);
            UARTSend (statusStr);
//            usprintf (statusStr, "DesiredAltitude=%4d %% ActualAltitude=%5d %%| \r\n", desiredAltitude, actualAltitude);
//            UARTSend (statusStr);
//            usprintf (statusStr, "MainDuty=%4d %% TailDuty=%4d %% | \r\n", mainDuty, tailDuty);
//            UARTSend (statusStr);
//            usprintf (statusStr, "OperatingMode=%4d | \r\n", operatingMode);
//            UARTSend (statusStr);


        }

}

