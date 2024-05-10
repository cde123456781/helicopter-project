#ifndef SWITCH_H
#define SWITCH_H

// *******************************************************
// switch.h
//
// programming functionality of sliding switch 1 on tiva
// board.
//
// Dylan Carlson and Bryson Chen
// 8/05/2024
// *******************************************************

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "inc/tm4c123gh6pm.h"

//enum switchNames {DOWN = 0, UP};


#define SLIDING_SWITCH_PERIPH       SYSCTL_PERIPH_GPIOA
#define SLIDING_SWITCH_PORT_BASE    GPIO_PORTA_BASE
#define SLIDING_SWITCH_PIN          GPIO_PIN_7
//assume the same as buttons.h
void
initSwitch(void);

void
checkSwitch(void);

bool getSwitchState (void);

#endif
