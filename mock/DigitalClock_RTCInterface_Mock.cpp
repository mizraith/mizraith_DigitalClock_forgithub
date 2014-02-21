/**
 * PART OF mizraith_DigitalClock package
 
 Class for exposing the required minimum interface between our
 Digital Clock and our RTC.   This should allow for easier
 substitution of alternative RTC's.

 */
 

#include "Arduino.h"
#include "DigitalClock_RTCInterface_Mock.h"

//We use the following for automatically adjusting the time
//when the firmware is loaded up.  


DigitalClock_RTCInterface::DigitalClock_RTCInterface( DateTime compiled_on ) {
        Serial.print(F("### USING MOCK RTC INTERFACE ###"));
        Serial.print(F("   COMPILED_ON: " ));
        Serial.print(compiled_on.hour(), DEC);
        Serial.println(compiled_on.minute(), DEC);
        mizraith_DigitalClock::NOW = compiled_on;       
}
    


void DigitalClock_RTCInterface::setTime(DateTime t) {
    mizraith_DigitalClock::NOW = t;
}


//Get a fresh time from the RTC
DateTime DigitalClock_RTCInterface::getTime( void ) {
    mizraith_DigitalClock::NOW += 1;
    Serial.print(F("#tick-tock#"));
    return mizraith_DigitalClock::NOW;
}

/**
 * Return temperature as a 2 bytes in degrees C
 *    MSB is the significant  00011001 = 25
 *    LSB is the decimals     00011001 = .25
 *  Display by writing them out to the display   MSB . LSB
 *
 * Data from the 3231 10bits provided in 2 bytes (11h and and 2bits in 12h)
 * Resolution of 0.25C
 * e.g.  00011001 01     =    25.25C
 *       00011001 = 25
 *             01 = 0.25 * 1 = .25
 *
 * Note, you can use macros:  TEMP_HBYTE and TEMP_LBYTE
 *
 */



void DigitalClock_RTCInterface::updateTemp(void) {
    int16_t x = getTemp();
}

int16_t DigitalClock_RTCInterface::getTemp(void) {
    uint8_t significand = 25;
    uint8_t decimals = 50;
    uint16_t temp = (uint16_t)significand << 8;
    temp = temp + decimals;
    mizraith_DigitalClock::TEMPERATUREasWORD = temp;
    return mizraith_DigitalClock::TEMPERATUREasWORD;
}










