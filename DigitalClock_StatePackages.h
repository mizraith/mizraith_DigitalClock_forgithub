/**
 * PART OF mizraith_DigitalClock package
 *
 * MAIN INCLUDE SEQUENCER FOR THE STATE MACHINE
 * UNDERLYING THE DIGITAL CLOCK PACKAGE
 *
 * Basic C table-driven state machine testing for an arduino.   Uses Digital Clock Library
*
* Red Byer    1/11/2014
* See http://github.com/mizraith
*
* Released to the public domain under the "beer license".  Meaning, buy me a beer should we meet!
*
* See: http://kjarvel.wordpress.com/2011/10/26/table-driven-state-machine-using-function-pointers-in-c/
*
*  We are taking that and blending it with a O(1) lookup state machine.  Sure, it's "slower"
* but state changes don't occur that fast on a digital clock, so speed is not that critical. The
* plus side is that we don't have to have a complete FSM modeled...and can add states as needed.
*  Ignored states wil "do nothing"
 */
#ifndef _mizraith_DigitalClockStatePackages_H_
#define _mizraith_DigitalClockStatePackages_H_

    #include "DigitalClock_StateTypedefs.h"
    #include "DigitalClock_InputEvents.h"
    #include "DigitalClock_StateEnums.h"
    #include "DigitalClock_StateFunctions.h"
    #include "DigitalClock_StateTables.h"
    #include "DigitalClock_TestMethods.h"     //optional, of course


#endif