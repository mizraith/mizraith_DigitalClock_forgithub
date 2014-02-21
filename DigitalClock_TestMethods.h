/**
 * PART OF mizraith_DigitalClock package
 *
 * defines test methods for excercising the state machine.
 */

#ifndef _mizraith_DigitalClockTestMethods_H_
#define _mizraith_DigitalClockTestMethods_H_

    #include <Arduino.h> 
    #include <avr/pgmspace.h>

    #include "DigitalClock_StatePackages.h"
    #include "DigitalClock_Data.h"
    #include "DigitalClock_Strings.h"
    #include "DigitalClock_StringProcessing.h"
    #include "DigitalClock_StateTypedefs.h"


    namespace mizraith_DigitalClock {
        //STATE MACHINE TEST METHODS
        int testAllTransitions(void);

        //EEPROM TEST METHODS
        void testSaveAndLoadSettingsFromEEPROM(void);
        void runSettingsTestStartingWith(bool b);
    
        //STRING & PROGMEM TEST METHODS
        void testStringProcessingMethods(void);
        
        void testFahrenheitToCelsiusConversion(int16_t tempasword);

    }
#endif