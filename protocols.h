#ifndef PROTOCOLS_H
#define PROTOCOLS_H

#include "switch.h"

#include "yaw.h"
#include "altitude.h"
#include "control.h"
#include "pwm.h"

uint8_t isLanding;
uint8_t isTakingOff;
uint8_t isHovering;


// Initialises values used in protocols
void initProc(void);

// Turns motors on and makes helicopter hover 
// and find reference point (or move to reference if found)
void takeOffProc(void);


// This function gradually decreases the altitude of the helicopter, directs it towards
// the reference position and sets it on the ground with the motor turned off at the end.
void landingProc(void);


// Determines the protocol that needs to be done
void determineProc(void);



#endif
