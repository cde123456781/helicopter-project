#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "switch.h"
#include "protocols.h"
#include "yaw.h"
//#include "buttons4.h"

#include "uart.h"

bool switchChange;

uint8_t switchPosition;
uint8_t switchCount;
int32_t switchState;

// This function initialises the switch on the specified port and pin
void initSwitch(void)
{
    SysCtlPeripheralEnable(SLIDING_SWITCH_PERIPH);
    GPIOPinTypeGPIOInput (SLIDING_SWITCH_PORT_BASE, SLIDING_SWITCH_PIN);
    GPIOPadConfigSet (SLIDING_SWITCH_PORT_BASE, SLIDING_SWITCH_PIN, GPIO_STRENGTH_2MA,
          GPIO_PIN_TYPE_STD_WPD);
    switchCount = 0;
    switchPosition = GPIOPinRead(SLIDING_SWITCH_PORT_BASE, SLIDING_SWITCH_PIN) == SLIDING_SWITCH_PIN;
    switchState = 0;
}

// This function detects if an action has caused the switch state to change, with added switch debouncing 
void
updateSwitch(void)
{
    switchState = GPIOPinRead(SLIDING_SWITCH_PORT_BASE, SLIDING_SWITCH_PIN) == SLIDING_SWITCH_PIN;
//            displayUART (0, 0,
//                         switchState, 0,
//                         0, 0, 1);

    //switchState = GPIOPinRead (UP_BUT_PORT_BASE, UP_BUT_PIN) == UP_BUT_PIN;
    if (switchState != switchPosition) {
        switchCount ++;

        if (switchCount >= NUM_SWITCH_POLLS){

            switchPosition = switchState;
            switchChange = true;
            switchCount = 0;
        }

    } else {
        switchCount = 0;
    }

}

// This function returns the action that has been done to the switch
uint8_t
checkSwitch(void)
{
    if (switchChange == true)
    {

        if (switchPosition == 1) {
            switchChange = false;
            return SWITCHUP;
        }
        else {
            switchChange = false;
            return SWITCHDOWN;
        }

    } else {
        return SWITCH_NO_CHANGE;
    }


}


// This function responds to any action which has changed the switch's state
void
pollSwitch(void)
{
    static uint8_t switchAction;
    switchAction = checkSwitch();
    if (switchAction == SWITCHUP)
     {

            if (isLanding == 0)
            {
                isTakingOff = 1;
            }

     }

    if (switchAction == SWITCHDOWN)
    {
        if (isRefFound) {
            isLanding = 1;
            isTakingOff = 0;
            isHovering = 0;
        }


    }
}













