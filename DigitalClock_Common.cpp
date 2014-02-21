/**
 * PART OF mizraith_DigitalClock package
 *
 * shared common functions
 */
 
#include "DigitalClock_Common.h"

//Developer Note:  Yes, all these need to be given a value
//prior to user access.  The class and RTC setups do this
//for us long before we (or a user) queries their value
namespace mizraith_DigitalClock {
    
    //blinks Arduino D13  (typically the LED pint).  Used for debugging.
    void blinkXTimes(uint8_t numberOnOffs) {
         for(uint8_t i=0; i++; i < numberOnOffs) {
              digitalWrite( 13, !digitalRead( 13 ));
              delay(100);
              digitalWrite( 13, !digitalRead( 13 ));
              delay(100);
         }
    }
    
    
    //Serial print out both Display1 and Display2 as they are
    //currently loaded -- with a little bit of formatting.
    void testPrintDisplays(void) {
        Serial.println(F("|||--------|||"));
        Serial.print(F("D1:"));
        Serial.println(Display1Text);
        Serial.print(F("D2:"));
        Serial.println(Display2Text);
    }
    
    
    
    void serialPrintNOW( void ) {
          Serial.println();
          Serial.print(F("NOW.hour()/min()/sec(): "));
          Serial.print(NOW.hour(), DEC);
          Serial.print(F(" "));
          Serial.print(NOW.minute(), DEC);
          Serial.print(F(" "));
          Serial.println(NOW.second(), DEC);
          Serial.println();
    }
    
    
    
}
//end namespace section




//Outside namespace definition.  The freeRam function doesn't seem to
//compile if it is within another namespace.
    /**
     * Extremely useful method for checking AVR RAM status
     * see: http://playground.arduino.cc/Code/AvailableMemory
     * and source:  http://www.controllerprojects.com/2011/05/23/determining-sram-usage-on-arduino/
     */
    int freeRam () {
      extern int __heap_start, *__brkval; 
      int v; 
      return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
    }