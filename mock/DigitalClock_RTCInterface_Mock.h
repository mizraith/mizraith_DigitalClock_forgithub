/**
 * PART OF mizraith_DigitalClock package
 
 Mock Class to allow more detail testing of Digital Clock Package.
 Matches the required minimum interface between our
 Digital Clock and our RTC.   This should allow for easier
 substitution of alternative RTC's.

 */
 
#ifndef _mizraith_DigitalCLock_RTCInterface_Mock_H_
#define _mizraith_DigitalCLock_RTCInterface_Mock_H_

#include "Arduino.h"
#include "Wire.h"
//#include "RTC_DS3231.h"
#include "RTClib.h"

#include "DigitalClock_Data.h"
#include "DigitalClock_Common.h"

class DigitalClock_RTCInterface {
    public:

        DigitalClock_RTCInterface( DateTime compiled_on );
        
        void setTime(DateTime t);
        DateTime getTime(void);
  
        void updateTemp(void);
        int16_t getTemp(void);
        
};



#endif