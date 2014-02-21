/**
 * PART OF mizraith_DigitalClock package
 *
 * shared common functions
 */
 
#ifndef _mizraith_DigitalClockCommon_H_
#define _mizraith_DigitalClockCommon_H_

#include <Arduino.h>
#include "DigitalClock_Strings.h"
#include "DigitalClock_Data.h"


//Developer Note:  Yes, all these need to be given a value
//prior to user access.  The class and RTC setups do this
//for us long before we (or a user) queries their value
namespace mizraith_DigitalClock {

        void blinkXTimes(uint8_t numberOnOffs);
        
        
        void testPrintDisplays(void);
        void serialPrintNOW(void);
    
}

//outside of namespace
    int freeRam (void);

#endif