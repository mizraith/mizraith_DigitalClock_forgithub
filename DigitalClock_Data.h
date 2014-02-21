/**
 * PART OF mizraith_DigitalClock package
 *
 * common data points and settings shared amongs pieces of the Digital Clock
 *
 */
 
#ifndef _mizraith_DigitalClockData_H_
#define _mizraith_DigitalClockData_H_

#include <Arduino.h>
#include <EEPROM.h>
#include "RTClib.h"         //for the DateTime class
#include "DigitalClock_Strings.h"
#include "DigitalClock_RTCInterface.h"
//#include "/mock/DigitalClock_RTCInterface_Mock.h"
#include "DigitalClock_EasterEggs.h"  
  
  
//forward declaration
class DigitalClock_RTCInterface;  

//our display strings will consume memory if we don't cap them.
// CAUTION:  overrun this and weird stuff results!!!
#define MAX_DISPLAY_STRING_LENGTH 100

#define MAX_USER_STRING_SIZE 34

//I was once told never to start at address 0....so, make it 64.
#define EEPROM_SETTINGS_ADDRESS 32 
#define EEPROM_USERSTRING_ADDRESS 100

//time that adjusting values blink on or off
#define BLINK_DELAY_TIME 500

#define SUBSTATE_TIMEOUT 10000

//with our encoder only at half-interrupt resolution,
//this corresponds to about 1/4 turn per binary value adjustment.
#define QUARTER_TURN_ADJUST_THRESHOLD 64
#define EIGHTH_TURN_ADJUST_THRESHOLD 32
#define SIXTEENTH_TURN_ADJUST_THRESHOLD 16

//Developer Note:  Yes, all these need to be given a value
//prior to user access.  The class and RTC setups do this
//for us long before we (or a user) queries their value
namespace mizraith_DigitalClock {
     
    //METHODS
    void initializeData( DateTime compiled_on );

    void saveSettingsToEEPROM( void );
    void saveUserStringToEEPROM( void );
    void loadSettingsFromEEPROM( void );  
    void verifyAndCorrectSettings( void );
    bool isNotValid(uint8_t setting);
    
    
    int8_t getTempHighByte( void );
    int8_t getTempLowByte( void );
    
    void updateTemperatureFromRTC( void );
    void updateDayOfWeekAtSetTimes( void );
    
    //Helper/debug methods
    void printOutUserSettings(void);
    void setAllSettingsTo(bool b);
    
    void setAndSaveUserString(char *);
    
    void updateEasterEggString(void);
    

    //VARIABLES 
    extern DateTime NOW;   
    extern uint8_t DayOfTheWeek;
    extern DigitalClock_RTCInterface * RTC_Interface;
    
    extern char Display1Text[MAX_DISPLAY_STRING_LENGTH];
    extern char Display2Text[MAX_DISPLAY_STRING_LENGTH];
    extern char stringbuffer[MAX_DISPLAY_STRING_LENGTH];
    extern char UserString[MAX_USER_STRING_SIZE];
    
    //16-bit, always stored in CELSIUS.   Top 8 bits are significand, bottom 8 bits
    //are decimal value.   We only convert to "F" on final display.
    extern int16_t TEMPERATUREasWORD;     

    extern int16_t InputEventValue;   //where we can store adjustment data from event until acted upon.
    extern int16_t AccumulatedEventValue;

    extern long SubMenuTimeOutCounter;
    extern bool BlinkingCharactersVisible;
    extern long BlinkCounter;

    extern const prog_char * EasterEggString;
    
    //---------User Settings-------------------------------------------  
    // for secondary display items, user can select what to show
    // each bool occupies 1-byte of space
    struct user_settings  {
        bool USE_24_HOUR;          //12h or 24h
        bool USE_CELSIUS;          //Display temp in celsius or fahrenheit
        bool DISPLAY_DAYOFWEEK;
        bool DISPLAY_DATE;
        bool DISPLAY_TEMP;
        bool DISPLAY_INFO;         //Project info
        bool DISPLAY_USERSTRING;
     };
    
    extern struct user_settings * SETTINGS;
    


    
    //--------------------------------------------------------------------------
    //EEPROM Anything
    // From Arduino Playground: http://playground.arduino.cc/Code/EEPROMWriteAnything
    //--------------------------------------------------------------------------
        template <class T> uint16_t EEPROM_writeAnything(int ee, const T& value)
    {
        const byte* p = (const byte*)(const void*)&value;
        uint16_t i;
        for (i = 0; i < sizeof(value); i++)  {
            EEPROM.write(ee++, *p++);
        }
        return i;
    }

    template <class T> uint16_t EEPROM_readAnything(int ee, T& value)
    {
        byte* p = (byte*)(void*)&value;
        uint16_t i;
        for (i = 0; i < sizeof(value); i++) {
            *p++ = EEPROM.read(ee++);
        }
        return i;
    }

    
       
}
#endif