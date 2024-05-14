/**
 * @file    pwm.h
 * @authors  Bryson Chen & Dylan Carlson
 * @date    7 May 2024
 *
 * This header file contains all declarations for functions, variables and macros for PWM of the ENCE361 Helicopter project
 *
*/

#ifndef PWM_H
#define PWM_H


#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h" //Needed for pin configure
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"


#include "stdlib.h"


//********************************************************
// Global variables
//********************************************************

uint8_t tailDutyCycle;
uint8_t mainDutyCycle;



//*****************************************************************************
// Constants
//*****************************************************************************



// PWM configuration
#define PWM_RATE_HZ  200
#define PWM_INITIAL_DUTY     0
#define PWM_DIVIDER_CODE   SYSCTL_PWMDIV_4
#define PWM_DIVIDER        4

//  PWM Hardware Details M0PWM7 (gen 3)
//  ---Main Rotor PWM: PC5, J4-05
#define PWM_MAIN_BASE        PWM0_BASE
#define PWM_MAIN_GEN         PWM_GEN_3
#define PWM_MAIN_OUTNUM      PWM_OUT_7
#define PWM_MAIN_OUTBIT      PWM_OUT_7_BIT
#define PWM_MAIN_PERIPH_PWM  SYSCTL_PERIPH_PWM0
#define PWM_MAIN_PERIPH_GPIO SYSCTL_PERIPH_GPIOC
#define PWM_MAIN_GPIO_BASE   GPIO_PORTC_BASE
#define PWM_MAIN_GPIO_CONFIG GPIO_PC5_M0PWM7
#define PWM_MAIN_GPIO_PIN    GPIO_PIN_5



//  ---Tail Rotor PWM: PF1, J3-10
#define PWM_TAIL_BASE        PWM1_BASE
#define PWM_TAIL_GEN         PWM_GEN_2
#define PWM_TAIL_OUTNUM      PWM_OUT_5
#define PWM_TAIL_OUTBIT      PWM_OUT_5_BIT
#define PWM_TAIL_PERIPH_PWM  SYSCTL_PERIPH_PWM1
#define PWM_TAIL_PERIPH_GPIO SYSCTL_PERIPH_GPIOF
#define PWM_TAIL_GPIO_BASE   GPIO_PORTF_BASE
#define PWM_TAIL_GPIO_CONFIG GPIO_PF1_M1PWM5
#define PWM_TAIL_GPIO_PIN    GPIO_PIN_1

//*****************************************************************************
// Functions
//*****************************************************************************


/*
 * Set Main motor PWM
 */
void
setMainPWM (uint8_t desiredDuty);


/*
 * Set Tail motor PWM
 */
void
setTailPWM (uint8_t desiredDuty);




/*
 * Initialise main motor PWM
 */
void
initialiseMainPWM (void);


/*
 * Initialise tail motor PWM
 */
void initialiseTailPWM (void);


/*
 * Enable output for the Tail PWM
 */
void enableTailPWMOutput (void);

/*
 * Enable output for the Main PWM
 */
void enableMainPWMOutput (void);


/*
 * Disable output for the Main PWM
 */
void disableMainPWMOutput (void);

/*
 * Disable output for the Tail PWM
 */
void disableTailPWMOutput (void);



#endif //PWM_H


