/**
 * PART OF mizraith_DigitalClock package
 *
 * shared common functions
 */
 
#include "DigitalClock_EasterEggs.h"

//Developer Note:  Yes, all these need to be given a value
//prior to user access.  The class and RTC setups do this
//for us long before we (or a user) queries their value
namespace mizraith_DigitalClock {
                                                          //m    d   century   year
    const prog_uchar EGG_NEWYEAR[]             PROGMEM =  {  1,  1, ANY_TIME, ANY_TIME};
    const prog_uchar EGG_VDAY[]                PROGMEM =  {  2, 14, ANY_TIME, ANY_TIME};
    const prog_uchar EGG_FORCE[]               PROGMEM =  {  5,  4, ANY_TIME, ANY_TIME};
    const prog_uchar EGG_TOWELDAY[]            PROGMEM =  {  5, 25, ANY_TIME, ANY_TIME};
    const prog_uchar EGG_PIRATEDAY[]           PROGMEM =  {  9, 19, ANY_TIME, ANY_TIME};
    const prog_uchar EGG_HALLOWEEN[]           PROGMEM =  { 10, 31, ANY_TIME, ANY_TIME};    
    const prog_uchar EGG_XMAS[]                PROGMEM =  { 12, 25, ANY_TIME, ANY_TIME};
    const prog_uchar EGG_TEST[]                PROGMEM =  { 2, 18, 20, 14 };
    
    //---------NOTE:  KEEP YOUR STRINGS < 30 CHARACTERS OR RISK HAVING TOO LONG A SECOND STRING//
                                                        //  1........10........20........30   
    const prog_char  New_Year_String[]         PROGMEM =  {" HAPPY NEW YEAR!"};
    const prog_char  Valentine_String[]        PROGMEM =  {" HAPPY V-DAY!"};
    const prog_char  ForceDay_String[]         PROGMEM =  {" MAY THE FORCE BE WITH YOU!"};
    const prog_char  TowelDay_String[]         PROGMEM =  {" HAPPY TOWEL DAY!"};
    const prog_char  PirateDay_String[]        PROGMEM =  {" RRRRR! AVAST BELAY YO HO!"};
    const prog_char  Halloween_String[]        PROGMEM =  {" TRICK OR TREAT!"};
    const prog_char  Christmas_String[]        PROGMEM =  {" MERRY CHRISTMAS!"};
    const prog_char  TestEgg_String[]          PROGMEM =  {" NOTHING TO SEE HERE."};
    
    
    //NOTE:  Need to update the length to match and keep the 2 arrays in sync!
    const uint8_t EGG_TABLE_LENGTH = 8;
    
    PROGMEM const prog_uchar *egg_table[] = 
    {
        EGG_NEWYEAR,
        EGG_VDAY,
        EGG_FORCE,
        EGG_TOWELDAY,
        EGG_PIRATEDAY,
        EGG_HALLOWEEN,
        EGG_XMAS,
        EGG_TEST
    };
    
    
    PROGMEM const prog_char *egg_text_table[] = 
    {
        New_Year_String,
        Valentine_String,
        ForceDay_String,
        TowelDay_String,
        PirateDay_String,
        Halloween_String,
        Christmas_String,
        TestEgg_String
    };
    
        
    
    //returns either the current easter egg string
    //or a pointer to the blank string
   const prog_char * getEasterEggStringOrBlank( void ) {
        char * s = getEasterEggStringOrNull();
        if( s ) {
            return (prog_char *) s;
        } else {
            return Empty_String;
        }
    }
    
    
    
     /**** METHODS ****/   
    void printEasterEggsToSerial( void ) {
        char buff[30];
        Serial.println(F("____Easter_Eggs____"));

//         Serial.print("address Egg_newyear[] ");
//         Serial.println( (uint16_t)EGG_NEWYEAR, DEC);
        
        for (uint8_t i=0 ; i<EGG_TABLE_LENGTH ; i++ ) {
            prog_uchar * eggaddr;
            uint8_t m, d, c, y;
            uint16_t EggYear;
            eggaddr =  (prog_uchar *)pgm_read_word_near(  &(egg_table[i])  );
            
            Serial.print(F("eggaddr: "));
            Serial.print((uint16_t)eggaddr, DEC);
            Serial.print(F("\t"));
            m = pgm_read_byte_near( eggaddr + month_index);
            d = pgm_read_byte_near( eggaddr + day_index );
            c = pgm_read_byte_near( eggaddr + century_index );
            y = pgm_read_byte_near( eggaddr + year_index );
            EggYear = c * 100;
            EggYear = EggYear + y;
            
            strcpy_P(buff, (char *)pgm_read_word( &(egg_text_table[i])));
            
            Serial.print(m);
            Serial.print(F("/"));
            Serial.print(d);
            Serial.print(F("/"));
            Serial.print(EggYear);
            Serial.print(F("\t--->"));
            Serial.print(buff);
            Serial.println();
        }
    }
    
    
    
    
    //returns the PROGMEM pointer to the easter string if today is an easter
    //egg day [being a positive number, this is as good as boolean true
    // You'll have to use a strcpy_P to get the string from PROGMEM to
    //a local string for use
    //  OTHERWISE, if not a day, will return 0  (boolean false).
    char * getEasterEggStringOrNull( void ) {
        prog_uchar * eggaddr;
        uint8_t m, d, c, y;
        uint8_t Nm, Nd;
        uint16_t EggYear, Ny;
        for (uint8_t i=0 ; i<EGG_TABLE_LENGTH ; i++ ) {        
            eggaddr =  (prog_uchar *)pgm_read_word_near(  &(egg_table[i])  );
            m = pgm_read_byte_near( eggaddr + month_index);
            d = pgm_read_byte_near( eggaddr + day_index );
            c = pgm_read_byte_near( eggaddr + century_index );
            y = pgm_read_byte_near( eggaddr + year_index );
            EggYear = c * 100;
            EggYear = EggYear + y;
            
            Nm = NOW.month();
            Nd = NOW.day();
            Ny = NOW.year();
            
            if( (m == ANY_TIME) || (m == Nm) ) {
                if ( (d == ANY_TIME) || (d == Nd) ) {
                    if ( (EggYear == ANY_TIME) || (EggYear = Ny) ) {
                        return (char *)pgm_read_word( &(egg_text_table[i]));
                    }
                }
            }
        }
        return (char *)0;            
    }
    
    
    void test_EasterEggMethods( void ) {
        printEasterEggsToSerial( );
        Serial.println(F("Today's Easter Egg String->"));
        const prog_char * s = getEasterEggStringOrBlank();
        char buff[30];
        strcpy_P(buff, s);
        Serial.print( (uint16_t)s, DEC);
        Serial.println(buff);

        // char * s = getEasterEggStringOrNull();
        // if( s ) {
        //     Serial.print(F("Today is an easter egg day:  "));
        //     Serial.print((uint16_t)s, DEC);
        //     char buff[30];
        //     strcpy_P(buff, s);
        //     Serial.println(buff);
        // } else {
        //     Serial.println(F("Today is NOT an easter egg day"));
        // }  
    }
    
    
    
    
    
}
//end namespace section

