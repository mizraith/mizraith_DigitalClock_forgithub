/**
 * PART OF mizraith_DigitalClock package
 *
 * methods that handle the heavy string lifting
 *
 */
 
#include "DigitalClock_StringProcessing.h"

namespace mizraith_DigitalClock {


    void BlinkingDisplay1( void ) {
        BlinkingDisplay1Span(0, 7);
    }

    //blink contiguous inclusive span on display 1. (overwrites
    //text wtih blanks at regular interval
    void BlinkingDisplay1Span(uint8_t spanstart, uint8_t spanend) {
        long currenttime = millis();
        
        if (currenttime - BlinkCounter > BLINK_DELAY_TIME) {
            BlinkingCharactersVisible = !BlinkingCharactersVisible;
            BlinkCounter = currenttime;    
        }  
        
        if (BlinkingCharactersVisible) {
            //leave it as is
        } else {
            blankDisplay1Span(spanstart, spanend);
        }
        
    }


    //handles the blinking when adjusting.  
    //NOTE: Span is INCLUSIVE and contiguous, index position starts at 0
    void updateDisplay1WithCurrentTimeBlinkingSpan(uint8_t spanstart, uint8_t spanend) {
        updateDisplay1WithCurrentTime(); 
        BlinkingDisplay1Span(spanstart, spanend);
    }



    //updates an 8-character time string target with the given time
    //Relies on  NOW being up to date
    void updateDisplay1WithCurrentTime(void) {
          char buff[3];   //2-digit vals here
          uint8_t temp;

          NOW = RTC_Interface->getTime();
          
          updateEasterEggString();    //this only updates at certain times
          
          strcpy_P(stringbuffer, Empty_String);   
  
          temp = NOW.hour();
          if( !(SETTINGS->USE_24_HOUR) ) {
              temp = temp % 12;
              if(temp == 0) { temp = 12; }
          }
          if (temp < 10) {
            strcat_P(stringbuffer, Zero_String);
          }
          itoa(temp, buff, 10);
          strcat(stringbuffer, buff );
          strcat_P(stringbuffer, Colon_String);
  
          temp = NOW.minute();
          if (temp < 10) {
            strcat_P(stringbuffer, Zero_String);
          }
          itoa(temp, buff, 10); 
          strcat(stringbuffer, buff );
          strcat_P(stringbuffer, Colon_String);
  
          temp = NOW.second();
          if (temp < 10) {
            strcat_P(stringbuffer, Zero_String);
          }
          itoa(temp, buff, 10);
          strcat(stringbuffer, buff );
          
          strcpy( Display1Text, stringbuffer );
    }   
    
    
    void updateDisplay1WithCurrentMonthAndDateBlinkingSpan(uint8_t spanstart, uint8_t spanend) {
        updateDisplay1WithCurrentMonthAndDate(); 
        BlinkingDisplay1Span(spanstart, spanend);
    }
    
    
    // "12345678"
    // "   MM/DD"
    void updateDisplay1WithCurrentMonthAndDate( void ) {
        char buff[3];   //2-digit vals here
        uint8_t temp;
        
        strcpy_P(stringbuffer, Blank_3String);
        
        temp = NOW.month();
        if (temp < 10) {
            strcat_P(stringbuffer, Zero_String);
        }   
        itoa(temp, buff, 10); 
        strcat(stringbuffer, buff );
        
        strcat_P(stringbuffer, Slash_String);
        
        temp = NOW.day();
        if (temp < 10) {
            strcat_P(stringbuffer, Zero_String);
        }   
        itoa(temp, buff, 10); 
        strcat(stringbuffer, buff );
       
        strcpy( Display1Text, stringbuffer);
    }
    
    
    
    void updateDisplay1WithCurrentYearBlinkingSpan( uint8_t spanstart, uint8_t spanend) {
        updateDisplay1WithCurrentYear(); 
        BlinkingDisplay1Span(spanstart, spanend);
    }
    
    
    // "12345678"
    //  "   YYYY"
    void updateDisplay1WithCurrentYear(void) {
        char buff[5];
        uint16_t temp;
        strcpy_P(stringbuffer, Blank_4String);
         
        temp = NOW.year();
        itoa(temp, buff, 10); 
        strcat( stringbuffer, buff );
       
        strcpy( Display1Text, stringbuffer);
    }
    
    void updateDisplay1WithCurrentDayOfWeek( void ) {
        prog_char * stringaddress;
        stringaddress =  (prog_char *)pgm_read_word_near(  &(days_of_the_week[DayOfTheWeek])  );
        
        strcpy_P(stringbuffer, Blank_5String);
        strcat_P(stringbuffer, stringaddress);
    
        strcpy( Display1Text, stringbuffer);
    
    }
    
    
    
    //RELIES ON USER PREFERENCE STRUCT
    //Reference NOW, and TEMP_HBYTE and TEMP_LBYTE and other options
    void updateDisplay2GivenSettings(void) {
        strcpy_P(stringbuffer, Blank_5String);   //start blank lead in 

        if (SETTINGS->DISPLAY_DAYOFWEEK) {
           strcat_P(stringbuffer, Blank_2String);
           addDayOfWeekToStringBuffer();
        }
        if (SETTINGS->DISPLAY_DATE) {
            strcat_P(stringbuffer, Blank_2String);
            addDateToStringBuffer();
        }
        if (SETTINGS->DISPLAY_TEMP) {
            strcat_P(stringbuffer, Blank_2String);
            addTempToStringBuffer();
        }

        //WE ALWAYS ADD IN THE EASTER EGG STRING
        strcat_P(stringbuffer, Blank_2String);
        strcat_P(stringbuffer, EasterEggString);  
        
        if (SETTINGS->DISPLAY_USERSTRING) {
            strcat_P(stringbuffer, Blank_2String);
            addUserStringToStringBuffer();
        }
    
    
        if (SETTINGS->DISPLAY_INFO) {
            strcat_P(stringbuffer, Blank_2String);
            addInfoToStringBuffer();
        }
        
        strcpy( Display2Text, stringbuffer );
    }
    
    
    
    void addDayOfWeekToStringBuffer( ) {
        prog_char * stringaddress;
        stringaddress =  (prog_char *)pgm_read_word_near(  &(days_of_the_week[DayOfTheWeek])  );
        strcat_P(stringbuffer, stringaddress);
    }
        
    
    void addDateToStringBuffer() {  
        char ybuffer[5]; //4-digit, like YYYY
        char buff[3];   //2-digit vals here
        uint8_t temp;
        //using US standard here:  MM/DD/YYYY
        temp = NOW.month();
        if (temp < 10) { strcat_P(stringbuffer, Zero_String); }
        itoa(temp, buff, 10);
        strcat(stringbuffer, buff );
        strcat_P(stringbuffer, Slash_String);
        
        temp = NOW.day();
        if (temp < 10) { strcat_P(stringbuffer, Zero_String); }
        itoa(temp, buff, 10);
        strcat(stringbuffer, buff );
        strcat_P(stringbuffer, Slash_String);
        //note that NOW.year() is a 16bit
        itoa(NOW.year(), ybuffer,10);
        strcat(stringbuffer,  ybuffer);
    }
    
    
    void addTempToStringBuffer() {
        if(SETTINGS->USE_CELSIUS) { 
            addCelsiusTempToStringBuffer();  
        } else {  //FAHRENHEIT CONVERSION
            addFahrenheitTempToStringBuffer();
        }    
    }
    
    
    void addCelsiusTempToStringBuffer() {
        char ybuffer[6]; //4-digit, like YYYY
        int8_t significand, fraction;
        significand = getTempHighByte();
        fraction = getTempLowByte();
        
//         Serial.println(F("@@ USING CELSIUS @@"));
//         Serial.print(F("High Byte: "));
//         Serial.print(significand, DEC);
//         Serial.print(F("     Low Byte: "));
//         Serial.println(fraction, DEC);
        
        strcpy_P(ybuffer, Empty_String);
        itoa(significand, ybuffer, 10);
        strcat(stringbuffer, ybuffer);
        strcat_P(stringbuffer, Dot_String);
        itoa(fraction, ybuffer, 10);
        strcat(stringbuffer, ybuffer);
        strcat(stringbuffer, "C");     
    }
    
    
    void addFahrenheitTempToStringBuffer() {
        int8_t fsig, ffrac, csig, cfrac;
        int16_t tempfrac;
        uint8_t x = 0;
        char ybuffer[5]; //4-digit, like YYYY
        csig = getTempHighByte();
        cfrac = getTempLowByte();
        
        
        //POSITIVE VALUES
        if( (csig >= 0) && (csig <= MAX_TEMP_TABLE_LENGTH) ) {
            fsig = CtoF_Table[csig];
            
            x = (uint8_t) csig % 5;   //length of pattern
            ffrac = CtoF_fractional[x];
            x = getMilliFahrenheitOffset(cfrac);
            tempfrac = ffrac + x;
//             Serial.print(F(" $$$millis: "));
//             Serial.print(x, DEC);
//             Serial.print(F(" $$$tempfrac total: "));
//             Serial.println(tempfrac, DEC);
            if (tempfrac >= 100) {
                fsig = fsig + 1;    //carry forward
                ffrac = tempfrac - 100;
//                 Serial.print(F(" $$$fsig now: "));
//                 Serial.print(fsig, DEC);
//                 Serial.print(F(" $$$ffrac now: "));
//                 Serial.println(ffrac, DEC);
            } else {
                ffrac = tempfrac;
            }  
        } 
        //NEGATIVE VALUES
        else if (  (csig < 0) &&  (csig > -MAX_TEMP_TABLE_LENGTH_NEG) ){
            fsig = CtoF_Table_Negative[-csig];
            
            x = (uint8_t) (-csig % 5);      //we know csig is negative here
            ffrac = CtoF_fractional_neg[x];
            x = getMilliFahrenheitOffset(cfrac);   //cfrac should be positive here
            tempfrac = ffrac + x;
//              Serial.print(F(" $$$ffrac: "));
//              Serial.print(ffrac, DEC);
//              Serial.print(F(" $$$millis: "));
//              Serial.print(x, DEC);
//              Serial.print(F(" $$$tempfrac total: "));
//              Serial.println(tempfrac, DEC);
            if (tempfrac >= 100) {
                fsig = fsig - 1;   //remember, we're negative
                ffrac = tempfrac -  100;
            } else {
                ffrac = tempfrac;
            }
            
        } else {
            //inappropriate number out of range, append NaN
            strcat(stringbuffer, "xx.x");
        }

        itoa(fsig, ybuffer, 10);
        strcat(stringbuffer, ybuffer);
        strcat_P(stringbuffer, Dot_String);
        itoa(ffrac, ybuffer, 10);
        strcat(stringbuffer, ybuffer); 
        strcat(stringbuffer, "F");
        }       
    
    //given the low byte of our temp word (either 0, 25, 50, 75)
    //representing 0.0, 0.25, 0.50, 0.75C)
    //figure out the fractional fahrenheit offset
    //NOTE:  This function attempts to do some rounding BUT NEEDS WORK
    //TO CLEANUP THAT ROUNDING AND RANGE 
    uint8_t getMilliFahrenheitOffset( uint8_t  lowCbyte ) {
        uint8_t mF = 0;
        if ( (lowCbyte >= 14 ) && (lowCbyte <= 42) ) {
            mF = 45;  // for 0.45
        } else if ( (lowCbyte >= 43 ) && (lowCbyte <= 71) ) {
            mF = 90;  // for 0.9
        } else if ( (lowCbyte >= 72 ) && (lowCbyte <= 99) )  {
            mF = 135;
        } //otherwise, do nothing and live with errors
        return mF;
    }
    
    
    
    void addInfoToStringBuffer() {
        //this method could overrun our stringbuffer, so do some checking
        uint16_t currentlength, infolength, total;
        currentlength = strlen(stringbuffer);
        infolength = strlen(Info_String);
        total = currentlength + infolength;
        if( total > (MAX_DISPLAY_STRING_LENGTH - 2) ) {
            //----for debugging---------------
//             Serial.print(F("STRING 2 IS TOO LONG! curr info total= "));
//             Serial.print(currentlength, DEC);
//             Serial.print(" ");
//             Serial.print(infolength, DEC);
//             Serial.print(" ");
//             Serial.println(total, DEC);
//             delay(1000);  
            //---------------------------------
            return;   //don't add the info string
        } else {           
            strcat_P(stringbuffer, Info_String);
        }
    }
    
    void addUserStringToStringBuffer( void ) {
        //this method could overrun our stringbuffer, so do some checking
        uint16_t currentlength, userlength, total;
        currentlength = strlen(stringbuffer);
        userlength = strlen(UserString);
        total = currentlength + userlength;
        if( total > (MAX_DISPLAY_STRING_LENGTH - 2) ) {
            return;   //don't add the info string
        } else {           
            strcat(stringbuffer, UserString);
        }
    
    }
    
    
    void copyToDisplay1( const prog_char *str) {
        strcpy_P(Display1Text, str);
    }
    
    void copyToDisplay2( const prog_char *str) {
        strcpy_P(Display2Text, str);
    }
    
    
    
    //given progmem location, return the pointer to the string
    char * getStringFromProgmem( const prog_char *str ) {
        strcpy_P(stringbuffer, str);
        return stringbuffer;
    } 
    
    
    void copyToBufferAndPrint(const prog_char *str) {
        copyStringFromProgmemToBuffer(str);
        printBufferString();
    }
    
    
    void copyStringFromProgmemToBuffer( const prog_char *str ) {
        strcpy_P(stringbuffer,  str);
    }
    


    void printBufferString() {
        Serial.print(stringbuffer);
    }


    void blankDisplay1Span(uint8_t spanstart, uint8_t spanend) {
        for (uint8_t i=spanstart ; i<=spanend; i++) {
                Display1Text[i] = ' ';
        }
    }







//----------unused method-------------------
//     //The more manual method of doing this
//     void printProgmemStringCharByChar(const prog_uchar *str) {
//         char c;
//         while( (c = pgm_read_byte(str++)) ) {
//             Serial.write(c); 
//         }
//     }
    
    
    

}



