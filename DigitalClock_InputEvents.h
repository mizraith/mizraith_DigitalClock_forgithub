/**
 * PART OF mizraith_DigitalClock package
 *
 * input event definitions
 */
 
#ifndef _mizraith_DigitalClockInputEvents_H_
#define _mizraith_DigitalClockInputEvents_H_

#include <Arduino.h>

namespace mizraith_DigitalClock {

    const uint8_t INPUT_EVENT_OFFSET = 0;

    enum InputEvents { UPDATE_EVENT = INPUT_EVENT_OFFSET,
                       BUTTON_1_EVENT,   //e.g. select
                       BUTTON_2_EVENT,   //e.g.  display
                       BUTTON_3_EVENT,   //e.g. standby
                       BUTTON_4_EVENT,   //e.g. set
                       BUTTON_5_EVENT,   //unused at this time
                       BUTTON_6_EVENT,   //unused at this time
                       ADJUST_EVENT,     //e.g. rotary knob with +/- adjusts
                       MAX_NUMBER_OF_INPUT_EVENTS } ;
}

#endif