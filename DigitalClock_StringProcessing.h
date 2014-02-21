/**
 * PART OF mizraith_DigitalClock package
 *
 * methods that handle the heavy string lifting
 *
 */

#ifndef _mizraith_DigitalClockStringProcessing_H_
#define _mizraith_DigitalClockStringProcessing_H_

#include <Arduino.h> 
#include <avr/pgmspace.h>
#include "DigitalClock_Data.h"
#include "DigitalClock_Strings.h"
#include "DigitalClock_CtoF.h"
#include "DigitalClock_EasterEggs.h"

namespace mizraith_DigitalClock {

    //updates an 8-character time string target with the given time
    //Relies on  NOW being up to date
    void BlinkingDisplay1( void );
    void BlinkingDisplay1Span(uint8_t spanstart, uint8_t spanend);
    
    void updateDisplay1WithCurrentTimeBlinkingSpan(uint8_t spanstart, uint8_t spanend);
    void updateDisplay1WithCurrentTime(void);
    void updateDisplay1WithCurrentMonthAndDateBlinkingSpan(uint8_t spanstart, uint8_t spanend);
    void updateDisplay1WithCurrentMonthAndDate(void);
    void updateDisplay1WithCurrentYearBlinkingSpan( uint8_t spanstart, uint8_t spanend);
    void updateDisplay1WithCurrentYear(void);
    void updateDisplay1WithCurrentDayOfWeek( void );
    
    //RELIES ON USER PREFERENCE STRUCT
    //Reference NOW, and TEMP_HBYTE and TEMP_LBYTE and other options
    void updateDisplay2GivenSettings(void);
    
    //given progmem location, return the pointer to the string
    char * getStringFromProgmem( const prog_char *str );  
    
    void copyToDisplay1( const prog_char *str);
    void copyToDisplay2( const prog_char *str);
    
    
    void copyToBufferAndPrint(const prog_char *str);
    void copyStringFromProgmemToBuffer( const prog_char *str );
    void printBufferString( void );
    
    //void printProgmemStringCharByChar(const prog_uchar *str);

     //Helper methods
     void addDayOfWeekToStringBuffer( void );
     void addDateToStringBuffer( void );
     void addTempToStringBuffer( void );
     void addInfoToStringBuffer( void );
     void addUserStringToStringBuffer( void );
     void addCelsiusTempToStringBuffer( void );
     void addFahrenheitTempToStringBuffer( void );
     uint8_t getMilliFahrenheitOffset( uint8_t  lowCbyte );
     
     void blankDisplay1Span(uint8_t spanstart, uint8_t spanend);

}

#endif