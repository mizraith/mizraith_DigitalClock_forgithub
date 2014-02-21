/**
 * PART OF mizraith_DigitalClock package
 *
 * common data points and settings shared amongs pieces of the Digital Clock
 *
 */
 
#include "DigitalClock_Data.h"

//Developer Note:  Yes, all these need to be given a value
//prior to user access.  The class and RTC setups do this
//for us long before we (or a user) queries their value
namespace mizraith_DigitalClock {
    DigitalClock_RTCInterface * RTC_Interface ;
    //DigitalClock_RTCInterface * RTC_Interface =  new DigitalClock_RTCInterface(NOW);     // initialize the RTC_Interface while we're at it.
    struct user_settings * SETTINGS;
    
    char Display1Text[MAX_DISPLAY_STRING_LENGTH];
    char Display2Text[MAX_DISPLAY_STRING_LENGTH];
    char stringbuffer[MAX_DISPLAY_STRING_LENGTH];
    char UserString[MAX_USER_STRING_SIZE];
    
    DateTime NOW;  
    uint8_t DayOfTheWeek;
    int16_t TEMPERATUREasWORD;
    int16_t InputEventValue;
    int16_t AccumulatedEventValue;
    
    bool BlinkingCharactersVisible;
    long BlinkCounter;
    long SubMenuTimeOutCounter;
    
    const prog_char * EasterEggString = Empty_String;
        
    void initializeData( DateTime compiled_on ) {
        Serial.println(F("Init DigitalClock data"));
        //load with defaults in case EEPROM write fails
        
        strcpy_P(Display1Text, Empty_String);
        strcpy_P(Display2Text, Empty_String);
        strcpy_P(stringbuffer, Empty_String);
        strcpy_P(UserString, Empty_String);
        
        TEMPERATUREasWORD = (int16_t) (25 << 8); 
        InputEventValue = 0;
        AccumulatedEventValue = 0;
        
        SETTINGS = new struct user_settings; 
        //setAllSettingsTo(true);
        loadSettingsFromEEPROM();    
        
        RTC_Interface =  new DigitalClock_RTCInterface(compiled_on);   //assumes NOW was already set 
        
        DayOfTheWeek = RTC_Interface->getDayOfWeek();
        
        EasterEggString = getEasterEggStringOrBlank(); 
        
        
        SubMenuTimeOutCounter = millis();
        BlinkingCharactersVisible = true;
        BlinkCounter = 0;
        
        
        
    }
     
    void saveSettingsToEEPROM( ) {
        //have to dereference SETTINGS because that's what the macro expects
        EEPROM_writeAnything(EEPROM_SETTINGS_ADDRESS, *SETTINGS);
    }
    
    void saveUserStringToEEPROM( ) {
        EEPROM_writeAnything(EEPROM_USERSTRING_ADDRESS, UserString);
    }
    
    void loadSettingsFromEEPROM( void ) {
        //future, add validity checking?
        //but since it's all booleans, not ints, we'll survive!
        EEPROM_readAnything(EEPROM_SETTINGS_ADDRESS, *SETTINGS);
        //printOutUserSettings();
        
        EEPROM_readAnything(EEPROM_USERSTRING_ADDRESS, UserString);
        verifyAndCorrectSettings();
    }
    
    void verifyAndCorrectSettings( void ) {
        bool needToWriteOut = false;
        //printOutUserSettings();
        //A setting should either equal 1 or 0.  Otherwise, set to 1;
        if ( isNotValid(SETTINGS->USE_24_HOUR) )    { 
            SETTINGS->USE_24_HOUR = 0x01; 
            needToWriteOut = true;
        }
        if ( isNotValid(SETTINGS->USE_CELSIUS) )    { 
            SETTINGS->USE_CELSIUS = 0x01; 
            needToWriteOut = true;
        }
        if ( isNotValid(SETTINGS->DISPLAY_DAYOFWEEK) ) { 
            SETTINGS->DISPLAY_DAYOFWEEK = 0x01; 
            needToWriteOut = true;
        }
        if ( isNotValid(SETTINGS->DISPLAY_DATE) )   { 
            SETTINGS->DISPLAY_DATE = 0x01; 
            needToWriteOut = true;
        }
        if ( isNotValid(SETTINGS->DISPLAY_TEMP) )   { 
            SETTINGS->DISPLAY_TEMP = 0x01; 
            needToWriteOut = true;
        }
        if( isNotValid(SETTINGS->DISPLAY_INFO) )   { 
            SETTINGS->DISPLAY_INFO = 0x01; 
            needToWriteOut = true;
        }
        if( isNotValid(SETTINGS->DISPLAY_USERSTRING) )   { 
            SETTINGS->DISPLAY_USERSTRING = 0x01; 
            needToWriteOut = true;
        }
    
        if(needToWriteOut) {
            saveSettingsToEEPROM();
            Serial.println(F("Corrected User Settings"));
            printOutUserSettings();
        }
        
    }
    
    bool isNotValid(uint8_t setting) {
        if( (setting == 0) || (setting == 1) ) {
            //valid
            return false;
        } else {
            //invalid
//             Serial.print(F("Will fix isNotValid(setting): "));
//             Serial.println(setting, BIN);
            return true;
        }
    }
    
    
    
    int8_t getTempHighByte(  ) {
        int16_t x;
        x = TEMPERATUREasWORD;
        int8_t y = (int8_t)(x >> 8);
//         Serial.print("&&&&gethigh&&&&:  ");
//         Serial.print(TEMPERATUREasWORD, BIN);
//         Serial.print("  @:  ");
//         Serial.println((int) &TEMPERATUREasWORD, DEC);
        return y;
    }
    
    int8_t getTempLowByte( void ) {
        int16_t x;
        x = TEMPERATUREasWORD;
        int8_t y;
        y = x &= 0x00FF;
//         Serial.print("&&&&getlow&&&&:  ");
//         Serial.print(TEMPERATUREasWORD, BIN);
//         Serial.print("  @:  ");
//         Serial.println((int)&TEMPERATUREasWORD, DEC);
        return y;
    }
    
      
    void updateTemperatureFromRTC( void ) {
        RTC_Interface->updateTemp();
    }
    
    
    
    void setAndSaveUserString(char * userstring) {
        uint8_t len = strlen(userstring);
        if(len > (MAX_USER_STRING_SIZE - 2)) {
            Serial.println(F("Data.cpp: UserString too long"));
        } else {
            strcpy(UserString, userstring);
            saveUserStringToEEPROM();
        }
    }
    
    
    void updateDayOfWeekAtSetTimes(void) {
        if( (NOW.hour() == 0) ||  (NOW.hour() == 12) ) {
            if(NOW.minute() == 00) {
                if(NOW.second() < 2) {          //overkill, but make sure we catch it
                    //Serial.println("UPDATING EASTER EGG");
                    DayOfTheWeek = RTC_Interface->getDayOfWeek();
                }
            } 
        }
    }
    
    //check NOW to see if it's an appropriate time to update, so we
    //don't do it that often.
    void updateEasterEggString( void ) {
        if( (NOW.hour() == 0) ||  (NOW.hour() == 12) ) {
            if(NOW.minute() == 00) {
                if(NOW.second() < 2) {          //overkill, but make sure we catch it
                    //Serial.println("UPDATING EASTER EGG");
                    EasterEggString = getEasterEggStringOrBlank();
                }
            } 
        }
    }
    
    
    void printOutUserSettings(void) {
        Serial.println(F("---User Settings---"));
        Serial.print(F("24 Hour ? "));Serial.println(SETTINGS->USE_24_HOUR, BIN);
        Serial.print(F("Celsius ? "));Serial.println(SETTINGS->USE_CELSIUS, BIN);
        Serial.print(F("D of W  ? "));Serial.println(SETTINGS->DISPLAY_DAYOFWEEK, BIN);
        Serial.print(F("Date    ? "));Serial.println(SETTINGS->DISPLAY_DATE, BIN);
        Serial.print(F("Temp    ? "));Serial.println(SETTINGS->DISPLAY_TEMP, BIN);
        Serial.print(F("Info    ? "));Serial.println(SETTINGS->DISPLAY_INFO, BIN);
        Serial.print(F("UserStr ? "));Serial.println(SETTINGS->DISPLAY_USERSTRING, BIN);
        Serial.print(F("UserString->"));Serial.println(UserString);
        Serial.println();
    }
    
    
    void setAllSettingsTo(bool b) {
        SETTINGS->USE_24_HOUR = b;
        SETTINGS->USE_CELSIUS = b;
        SETTINGS->DISPLAY_DAYOFWEEK = b;
        SETTINGS->DISPLAY_DATE = b;
        SETTINGS->DISPLAY_TEMP = b;
        SETTINGS->DISPLAY_INFO = b;
        SETTINGS->DISPLAY_USERSTRING = b;
    }
    
}

