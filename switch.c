#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "inc/tm4c123gh6pm.h"  // Board specific defines (for PF0)
#include "switch.h"

static bool switchState;

void initSwitch(void)
{
    SysCtlPeripheralEnable(SLIDING_SWITCH_PERIPH);
    GPIOPinTypeGPIOInput (SLIDING_SWITCH_PORT_BASE, SLIDING_SWITCH_PIN);
    GPIOPadConfigSet (SLIDING_SWITCH_PORT_BASE, SLIDING_SWITCH_PIN, GPIO_STRENGTH_2MA,
          GPIO_PIN_TYPE_STD_WPD);

}

void checkSwitch(void)
{
     switchState = GPIOPinRead(SLIDING_SWITCH_PORT_BASE, SLIDING_SWITCH_PIN );

}

bool getSwitchState (void) {
    return switchState;
}












