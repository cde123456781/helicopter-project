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
#include "protocols.h"
#include "yaw.h"

#define NUM_SWITCH_POLLS 3

enum switchActions {SWITCHUP = 0, SWITCHDOWN , SWITCH_NO_CHANGE};


bool switchChange;

uint8_t switchPosition;
uint8_t switchCount;
int32_t switchState;



#define SLIDING_SWITCH_PERIPH       SYSCTL_PERIPH_GPIOA
#define SLIDING_SWITCH_PORT_BASE    GPIO_PORTA_BASE
#define SLIDING_SWITCH_PIN          GPIO_PIN_7



// This function initialises the switch on the specified port and pin
void
initSwitch(void);


// This function detects if an action has caused the switch state to change, with added switch debouncing 
void
updateSwitch(void);


// This function returns the action that has been done to the switch
uint8_t
checkSwitch(void);


// This function responds to any action which has changed the switch's state
void
pollSwitch(void);



#endif
