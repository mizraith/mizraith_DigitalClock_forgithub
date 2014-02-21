/**
 * PART OF mizraith_DigitalClock package
 *
 * shared common functions
 */
 
#ifndef _mizraith_DigitalClockEasterEggs_H_
#define _mizraith_DigitalClockEasterEggs_H_

#include <Arduino.h>
#include <avr/pgmspace.h>
#include "RTClib.h"
#include "DigitalClock_Data.h"

#define ANY_TIME 0
#define month_index 0
#define day_index 1
#define century_index 2
#define year_index 3

//Developer Note:  Yes, all these need to be given a value
//prior to user access.  The class and RTC setups do this
//for us long before we (or a user) queries their value
namespace mizraith_DigitalClock {
    struct egg_array {
        uint8_t month;
        uint8_t day;
        uint8_t century;
        uint8_t year;
    };
      
    const prog_char * getEasterEggStringOrBlank( void );
    void printEasterEggsToSerial( ) ;
    char * getEasterEggStringOrNull( void );

    void test_EasterEggMethods( void );


    extern const uint8_t *egg_table[];
    extern const prog_char *egg_text_table[];
    
}

#endif