/**
 * PART OF DigitalClock package
 *
 */
 
#ifndef _mizraith_DigitalClockStateEnums_H_
#define _mizraith_DigitalClockStateEnums_H_

#include <Arduino.h>

namespace mizraith_DigitalClock {

    const uint8_t MAIN_STATE_OFFSET = 0;
    const uint8_t DISPLAY_OPTIONS_OFFSET = 10;
    const uint8_t SET_TIME_OFFSET = 20;

    enum MainStates { STATE_STARTUP = MAIN_STATE_OFFSET,
                      STATE_NORMAL,      
                      STATE_DISPLAY_OPTIONS,     
                      STATE_SET_TIME,   
                      MAX_NUMBER_OF_STATES } ; 
                
    enum DisplayOptionsSubStates {  STATE_DISPLAY_24HR = DISPLAY_OPTIONS_OFFSET,
                                    STATE_DISPLAY_TEMP, 
                                    STATE_DISPLAY_C,
                                    STATE_DISPLAY_DATE, 
                                    STATE_DISPLAY_DAYOFWEEK,
                                    STATE_DISPLAY_INFO,
                                    STATE_DISPLAY_USERSTRING,
                                    MAX_NUMBER_OF_DISPLAY_OPTIONS_STATES };
                  
    enum SetTimeSubStates { STATE_SET_HOUR = SET_TIME_OFFSET,
                            STATE_SET_MINUTE,
                            STATE_SET_MONTH,
                            STATE_SET_DAY,
                            STATE_SET_YEAR,
                            STATE_SET_DAYOFWEEK,
                            MAX_NUMBER_OF_SET_TIME_STATES } ;   


}


#endif