/**
 * PART OF mizraith_DigitalClock package
 *
 * base definition of a "state"
 *
 */
 
#ifndef _mizraith_DigitalClockStateTypedefs_H_
#define _mizraith_DigitalClockStateTypedefs_H_

#include <Arduino.h>

namespace mizraith_DigitalClock {

    /* Forward declaration of state structure */
    struct state;

    /* State function prototype */
    typedef void (state_func)(const struct state *);
 
    /* Indexes for gettting struct info from progmem */
    const uint8_t OFFSET_CURRENTSTATE = 0;
    const uint8_t OFFSET_EVENTID = 1;
    const uint8_t OFFSET_NEXTSTATE = 2;
    const uint8_t OFFSET_FUNCTION = 3;
    /* Structure containing state function and data */
    struct state  {
        uint8_t         currentstate;     //0
        uint8_t         eventID;          //1
        uint8_t         nextstate;        //2
        state_func  *func_p;              //3  * are uint16_t 3,4
    };

} 

#endif