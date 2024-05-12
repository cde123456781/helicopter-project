#include "switch.h"
#include "yaw.h"
#include "altitude.h"
#include "control.h"

uint8_t isLanding;
uint8_t isTakingOff;

void takeOffProc(void)
{
    if (isLanding == 0)
    {
        // take off procedure goes here
    }
}

void landingProc(void)
{
    if (isLanding == 1) 
    {
        //landing procedure goes here
    }
}