/**
 * PART OF mizraith_DigitalClock package
 *
 * common strings used by pieces of the Digital Clock. Note
 * we save RAM by using PROGMEM
 *
 */
 
#ifndef _mizraith_DigitalClockStrings_H_
#define _mizraith_DigitalClockStrings_H_

#include <Arduino.h>
#include <avr/pgmspace.h>

//Developer Note:  Yes, all these need to be given a value
//prior to user access.  The class and RTC setups do this
//for us long before we (or a user) queries their value
//
// While the syntax can be tricky, much of this was verified based on testing ("PROMEM_Testing" Sketch)
namespace mizraith_DigitalClock {

        const prog_char  Startup_String[]            PROGMEM =  {" STARTUP"};
        const prog_char Intro_String[]               PROGMEM =  {"       LIGHTWAVE CLOCK V1.0 - RED BYER - 57600bps - http://github.com/mizraith"};
        const prog_char Info_String[]                PROGMEM =  {"LIGHTWAVE CLOCK V1.0 - RED BYER"};

        const prog_char Static_Time_String[]        PROGMEM =  {"HH:MM:SS"};
        const prog_char Static_Date_String[]        PROGMEM =  {"       01/27/2014   25.0C"};

        const prog_char Blank_8String[]              PROGMEM =  {"        "};
        const prog_char Blank_7String[]              PROGMEM =  {"       "};  //for lead in on scrolling text
        const prog_char Blank_5String[]              PROGMEM =  {"     "};
        const prog_char Blank_4String[]              PROGMEM =  {"    "};
        const prog_char Blank_3String[]              PROGMEM =  {"   "};
        const prog_char Blank_2String[]              PROGMEM =  {"  "};
        const prog_char Blank_1String[]              PROGMEM =  {" "};
        const prog_char  Empty_String[]              PROGMEM =  {""};
        
        const prog_char  Zero_String[]               PROGMEM =  {"0"};
        const prog_char  Dot_String[]                PROGMEM =  {"."};
        const prog_char  Colon_String[]              PROGMEM =  {":"};
        const prog_char  Slash_String[]              PROGMEM =  {"/"};
        
        
    
        const prog_char Hour_Select_String[]        PROGMEM =  {" 12H/24H"};
        const prog_char Twelve_Hour_String[]        PROGMEM =  {"   12H  "};
        const prog_char TwentyFour_Hour_String[]    PROGMEM =  {"   24H  "};
    
        const prog_char Temp_Select_String[]        PROGMEM =  {"  F / C "};
        const prog_char Fahrenheit_String[]         PROGMEM =  {"    F   "};
        const prog_char Celsius_String[]            PROGMEM =  {"    C   "};
    
        const prog_char Show_Date_String[]          PROGMEM =  {"       SHOW DATE?"};
        const prog_char Show_Temp_String[]          PROGMEM =  {"       SHOW TEMP?"};
        const prog_char Show_DayOfWeek_String[]     PROGMEM =  {"       SHOW DAY OF WEEK?"};
        const prog_char Show_Info_String[]          PROGMEM =  {"       SHOW INFO?"};
        const prog_char Show_User_String[]          PROGMEM =  {"       SHOW USER STRING?"};
        
        const prog_char Yes_String[]                PROGMEM =  {"   YES  "};
        const prog_char No_String[]                 PROGMEM =  {"   NO   "};
    
        const prog_char Set_Hour_String[]           PROGMEM =  {"  SET H "};
        const prog_char Set_Minute_String[]         PROGMEM =  {"  SET M "};
        const prog_char Set_Month_String[]          PROGMEM =  {"  SET MM"};
        const prog_char Set_Day_String[]            PROGMEM =  {"  SET D "};
        const prog_char Set_Year_String[]           PROGMEM =  {"  SET Y "};
        const prog_char Set_DayOfWeek_String[]      PROGMEM =  {" SET DAY"};
    
        const prog_char Sunday_String[]             PROGMEM =  {"SUN"};
        const prog_char Monday_String[]             PROGMEM =  {"MON"};
        const prog_char Tuesday_String[]            PROGMEM =  {"TUE"};
        const prog_char Wednesday_String[]          PROGMEM =  {"WED"};
        const prog_char Thursday_String[]           PROGMEM =  {"THU"};
        const prog_char Friday_String[]             PROGMEM =  {"FRI"};
        const prog_char Saturday_String[]           PROGMEM =  {"SAT"};
        
        extern const prog_char *days_of_the_week[];
        
}


#endif