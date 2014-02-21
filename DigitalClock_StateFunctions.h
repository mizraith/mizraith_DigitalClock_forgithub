/**
 * PART OF mizraith_DigitalClock package
 *
 * State transition and state machine related functionality.
 * This is where "all" the work is really done.
 */
#include "DigitalClock_StatePackages.h"


#ifndef _mizraith_DigitalClockStateFunctions_H_
#define _mizraith_DigitalClockStateFunctions_H_

#include <Arduino.h>
#include "DigitalClock_Data.h"
#include "DigitalClock_Strings.h"
#include "DigitalClock_Common.h"
#include "DigitalClock_StateTypedefs.h"


namespace mizraith_DigitalClock {

    extern bool DEBUG_STATE_TRANSITIONS;   // Display 'state traces'  over the serial port.  Consumes ~2k of PROGMEM
    const bool DISPLAY_TO_SERIAL = false;  //print out 'display' as we go -- can generate a LOT of chatter, so slow down calls to update()!!!
    
    void startStateMachine(void);
    void handleEvent(uint8_t, int16_t);           //the main entry point
    
    void handleSubStateTimeout(uint8_t);
    
    //expect to be regularly called as a way to let state machine update the displays
    void update( void );       

    /* State transition and function declarations */
    //-----main state transitions
    void handleMainState(const struct state *);
    void handleDisplayOptionsState(const struct state *);
    void handleSetTimeState(const struct state *);
    void handleStateWorker(const struct state *, const struct state **, uint8_t );    

    void doNothingTransition(const struct state *);  
    void startupState(const struct state *);
    void transitionToNormal(const struct state *);
    void transitionToDisplayOptions(const struct state *);
    void transitionToSetTime(const struct state *);

    
    //-----display option state transitions
    void exitDisplayOptions(const struct state *);

    void transitionToDisplay24Hr(const struct state *);    
    void transitionToDisplayC(const struct state *);
    void transitionToDisplayTemp(const struct state *);
    void transitionToDisplayDate(const struct state *);
    void transitionToDisplayDayOfWeek(const struct state *);
    void transitionToDisplayInfo(const struct state *);
    void transitionToDisplayUserString(const struct state *);

    void adjustDisplay24Hr(const struct state *);
    void adjustDisplayC(const struct state *);
    void adjustDisplayTemp(const struct state *);
    void adjustDisplayDate(const struct state *);
    void adjustDisplayDayOfWeek(const struct state *);
    void adjustDisplayInfo(const struct state *);
    void adjustDisplayUserString(const struct state *);
        
 
    //-----set time state transitions 
    void exitSetTime(const struct state *);
    
    void transitionToSetMinute(const struct state *);
    void transitionToSetMonth(const struct state *);
    void transitionToSetDay(const struct state *);        
    void transitionToSetYear(const struct state *);
    void transitionToSetHour(const struct state *);
    void transitionToSetDayOfWeek(const struct state *);
                
    void adjustSetMinute(const struct state *);
    void adjustSetHour(const struct state *);
    void adjustSetMonth(const struct state *);
    void adjustSetDay(const struct state *);
    void adjustSetYear(const struct state *);
    void adjustSetDayOfWeek(const struct state *);


    //--------HELPER functions for this test class
    void loadStateVariableFromProgmemAddr( struct state *statescratch, state * StateAddressInProgmem);
    
    void printStateData(const struct state *); 
    bool eventOutOfRange(uint8_t event);
    bool meetsBinaryAdjustmentCriteria(const struct state *s_p);
    bool binaryAdjustmentThresholdReachedThenClearAccumulated( void );
    bool adjustmentThresholdReachedThenClearAccumulated( uint8_t thresh );
    int8_t getAdjustment(const struct state *s_p, uint8_t thresh) ;
}




#endif