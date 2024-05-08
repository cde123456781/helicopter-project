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

enum switchNames {DOWN = 0, UP}

#define SLIDING_SWITCH_PERIPH       SYSCTL_PERIPH_GPIOA
#define SLIDING_SWITCH_PORT_BASE    GPIO_PORTA_BASE
#define SLIDING_SWITCH_PIN          GPIO_PIN_7
//assume the same as buttons.h
void
initSwitch(void);

void
checkSwitch(void);

void
updateSwitch(void);

