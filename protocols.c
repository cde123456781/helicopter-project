#include "switch.h"
#include "yaw.h"
#include "altitude.h"
#include "control.h"
#include "protocols.h"
#include "pwm.h"



// Initialises values used in protocols
void initProc(void)
{
    isLanding = 0;
    isTakingOff = 0;
    isHovering = 0;
}


// Makes helicopter hover and find reference point (or move to reference if found)
void takeOffProc(void)
{
    //Switched to take off
    //Hover and then Look for reference
    if (isLanding == 0 && isTakingOff == 1)
    {

        enableMainPWMOutput();
        enableTailPWMOutput();

        // take off procedure goes here
        //start hovering 2% alt

        mainSetPoint = 2;
        //find reference point, set yaw to zero at reference

        if (isRefFound == 0)
        {
            discoverReference();
        } else if (isRefFound == 1)
        {
            goToReference();
            isTakingOff = 0;
            isHovering = 1;
        }
    }

}

// This function gradually decreases the altitude of the helicopter, directs it towards
// the reference position and sets it on the ground with the motor turned off at the end.
void landingProc(void)
{

    if (isLanding == 1) 
    {
        if (mainSensorValue >= 5) {
            if (landingTimeFlag == true)
            {
                landingTimeFlag = false;
                if ((mainSetPoint < 10)) {
                    mainSetPoint -= 1;
                    if (mainSetPoint < 5) {
                        mainSetPoint = 2;
                    }
                } else {
                    mainSetPoint -= 5;
                }


            }

        } else if (tailSensorValue != 0) {
            goToReference();

        } else {
            mainSetPoint = 0;
            isLanding = 0;

            disableMainPWMOutput();
            disableTailPWMOutput();

        }

    }
}


// Determines the protocol that needs to be done
void determineProc (void)
{
    if ((isLanding == 1) && (isRefFound == 1)) 
    {
        landingProc();

    } else if (isTakingOff == 1) 
    {
        takeOffProc();
    }
}
