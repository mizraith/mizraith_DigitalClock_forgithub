/**
 * PART OF mizraith_DigitalClock package
 
 Class for exposing the required minimum interface between our
 Digital Clock and our RTC.   This should allow for easier
 substitution of alternative RTC's.

 */
 


#include "DigitalClock_RTCInterface.h"
#include "Arduino.h"


DigitalClock_RTCInterface::DigitalClock_RTCInterface( DateTime compiled_on ) {
        //This is where you can change your actual device 
        Serial.println(F("Setting up RTCInterface"));
        
        RTC_DS3231 *RTC_COMM = new RTC_DS3231();      
        setupRTC(compiled_on);
}
    


void DigitalClock_RTCInterface::setTime(DateTime t) {
    //no error checking
    RTC_COMM->adjust(t);
}


//Get a fresh time from the RTC
DateTime DigitalClock_RTCInterface::getTime( void ) {
    mizraith_DigitalClock::NOW = RTC_COMM->now();
    return mizraith_DigitalClock::NOW;
}

//returns current DayOfWeek (1-7)
uint8_t DigitalClock_RTCInterface::getDayOfWeek( void ) {
    uint8_t day;
    day = RTC_COMM->getDayOfWeek();
}

void DigitalClock_RTCInterface::setDayOfWeek(uint8_t day) {
    RTC_COMM->setDayOfWeek(day);
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


/**
 * Initialize the RTC.
 */
void DigitalClock_RTCInterface::setupRTC(DateTime compiled_on) {
    Serial.print(F("   setupRTC COMPILED_ON: " ));
    Serial.print(compiled_on.hour(), DEC);
    Serial.println(compiled_on.minute(), DEC);
          
//     if(RTC_COMM->begin()) {
//         Serial.println(F("   setupRTC begin() normal"));
//     }
    if (! RTC_COMM->isrunning()) {
             Serial.println(F("   setupRTC NOT RUNNING!!!"));      
    }
  
    mizraith_DigitalClock::NOW = RTC_COMM->now();       
  
    if (mizraith_DigitalClock::NOW.unixtime() < compiled_on.unixtime()) {
        Serial.println(F("   setupRTC updating RTC time"));  
        RTC_COMM->adjust(compiled_on);
      
        //provide signal that we're resetting time from scratch
        //this could indicate a failed coin cell backing the clock.   
//        Serial.println(F("Set time older than compile time! Updating"));
        mizraith_DigitalClock::blinkXTimes(5);
    }
    
    RTC_COMM->enable32kHz(true);
    RTC_COMM->SQWEnable(true);         //used for "seconds" ticker ...eventually
    RTC_COMM->BBSQWEnable(false);      //while handy, enabling the BBSQW drains coin cell significantly faster!
    RTC_COMM->SQWFrequency( DS3231_SQW_FREQ_1024 );
   
   
    
    
//    char datastr[100];
//    RTC.getControlRegisterData( datastr[0]  );
//    if(DEBUG) {
//        Serial.print(  datastr );      //do we want to do this?
//    }
}


void DigitalClock_RTCInterface::updateTemp(void) {
    mizraith_DigitalClock::TEMPERATUREasWORD = RTC_COMM->getTempAsWord();
}

int16_t DigitalClock_RTCInterface::getTemp(void) {
    mizraith_DigitalClock::TEMPERATUREasWORD = RTC_COMM->getTempAsWord();
    return mizraith_DigitalClock::TEMPERATUREasWORD;
}














