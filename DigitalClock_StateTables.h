/**
 * PART OF mizraith_DigitalClock package
 *
 * state transition table definition for our clock's state machine.
 *
 */


#ifndef _mizraith_DigitalClockStateTables_H_
#define _mizraith_DigitalClockStateTables_H_

#include "DigitalClock_StatePackages.h"
#include <Arduino.h>
#include <avr/pgmspace.h>
#include "DigitalClock_StateTypedefs.h"

namespace mizraith_DigitalClock {

    /* -------------------------------------------------------------------------------------
     * State table(s)
     * -------------------------------------------------------------------------------------
     */
    const uint8_t ANY_STATE = 125;       //arbitrary value to test against
    const uint8_t ANY_EVENT = 125;       //arbitrary value to test against
//     const uint8_t TABLE_END = 126;       //iterator flag for our state tables
    const uint16_t ARRAY_END = 0;        //null flag to quickly determine array end

    extern const struct state * main_state_transitions[];
    extern const struct state * display_options_state_transitions[];
    extern const struct state * set_time_state_transitions[];

}



#endif