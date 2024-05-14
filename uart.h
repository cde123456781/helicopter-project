/**
 * @file    uart.h
 * @authors  Bryson Chen & Dylan Carlson
 * @date    9 May 2024
 *
 * This header file contains all declarations, variables and macros for UART
 *
*/

#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdbool.h>


#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#include "utils/ustdlib.h"

//********************************************************
// Macros
//********************************************************

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
// Global variables
//********************************************************

volatile uint8_t sendUARTFlag;

//********************************************************
// Functions
//********************************************************


// This function initialises UART
void initialiseUSB_UART (void);


// This function displays the specified character buffer on the terminal
void UARTSend (char *pucBuffer);

// This function displays the desired yaw, the actual yaw, the desired altitude, the actual altitude, both duty cycles, and 
// the current mode of the helicopter on the terminal
void
displayUART (int32_t desiredYaw, int32_t actualYaw,
             int16_t desiredAltitude, int16_t actualAltitude,
             uint8_t mainDuty, uint8_t tailDuty, uint8_t isLanding,
             uint8_t isHovering, uint8_t isTakingOff);

void
printMode (uint8_t isLanding, uint8_t isHovering, uint8_t isTakingOff);
#endif // UART_H
