/**
 * PART OF mizraith_DigitalClock package
 *
 * defines test methods for excercising the state machine.
 *
 */
 
#include "DigitalClock_TestMethods.h"

namespace mizraith_DigitalClock {

     /** ---------------------------------------------------------------------
     *   PRIMARY STATE MACHINE TEST FUNCTION
     * ----------------------------------------------------------------------
     */
    int testAllTransitions(void)
    {
       Serial.println(F(""));
       Serial.println(F("--------------------------------------------------"));
       Serial.println(F("STARTING STATE TRANSITION TESTS"));
       Serial.println(F("--------------------------------------------------"));
       Serial.println(F(""));
   
       DEBUG_STATE_TRANSITIONS = true;
   
       struct state *statescratch = new struct state;
  
       Serial.println(F("----- Main States -----"));
       Serial.print(F("FREE RAM: "));
       Serial.println(freeRam(), DEC);
       //first the main states
       boolean foundit = false;
       for(uint8_t i=0; !foundit; i++) {
            state * StateAddressInProgmem = (state *)pgm_read_word_near(  &(main_state_transitions[i])  );

            if ((uint16_t)StateAddressInProgmem == ARRAY_END) {
                break;
            }  

            loadStateVariableFromProgmemAddr(statescratch, StateAddressInProgmem);

            statescratch->func_p(statescratch);         
            delay(25);
            Serial.println();
       } 
       Serial.println(F("--ok--"));
     
     
       

       delay(500);
       Serial.println(F("----- Display Option States -----"));
       Serial.print(F("FREE RAM: "));
       Serial.println(freeRam(), DEC);
       //second the display option states
       foundit=false;
       for(uint8_t i=0; !foundit; i++) {
            state * StateAddressInProgmem = (state *)pgm_read_word_near(  &(display_options_state_transitions[i])  );

            if ((uint16_t)StateAddressInProgmem == ARRAY_END) {
                break;
            }  

            loadStateVariableFromProgmemAddr(statescratch, StateAddressInProgmem);

            statescratch->func_p(statescratch);         
            delay(25);
            Serial.println();
       }
       Serial.println(F("--ok--"));
    
    
    
    
       delay(500); 
       Serial.println(F("----- Set Time States -----")); 
       Serial.print(F("FREE RAM: "));
       Serial.println(freeRam(), DEC);
       //third the set time options states 
       foundit=false;
       for(uint8_t i=0; !foundit; i++) {
            state * StateAddressInProgmem = (state *)pgm_read_word_near(  &(set_time_state_transitions[i])  );

            if ((uint16_t)StateAddressInProgmem == ARRAY_END) {
                break;
            }  

            loadStateVariableFromProgmemAddr(statescratch, StateAddressInProgmem);

            statescratch->func_p(statescratch);         
            delay(25);
            Serial.println();
       }
       Serial.println(F("--ok--"));
       
    
    
    
       
       Serial.println(F(""));
       Serial.println(F("--------------------------------------------------"));
       Serial.println(F("STATE TRANSITION TESTS COMPLETE  "));
       Serial.println(F("--------------------------------------------------"));
       Serial.println(F(""));
     
  
//------Useful little memory checker for when I was considering using PROGMEM for state tables    
//       const struct state *stateptr;
//        //uint16_t stateptr = (uint16_t)test_transitions;
//        stateptr = test_transitions;
//        Serial.print("TEST TRANSITIONS ADDRESS:   ");
//        Serial.println( (int)stateptr, DEC );
//        
//        uint8_t t = 0;
//        uint8_t i = 0;
//        while (i < 16) {   
//            t = pgm_read_byte_near( stateptr + i );
//            Serial.print( (int)stateptr + i , DEC );
//            Serial.print(":  ");
//            Serial.println( t, DEC );
//            i++;
//        }
//        Serial.println();


        DEBUG_STATE_TRANSITIONS = false;
        return 0;
    }


    //EEPROM TEST METHODS
    
    void testSaveAndLoadSettingsFromEEPROM(void) {
        Serial.println();
        Serial.println(F("------------------------------------------------------"));
        Serial.println(F("EEPROM save/load test. Will destroy existing settings."));
        Serial.println(F("------------------------------------------------------"));
        
        //run two tests back to back to protect against reading in old data and passing.
        runSettingsTestStartingWith(false);
        runSettingsTestStartingWith(true);
        
        Serial.println(F("---------------EEPROM testing complete----------------"));
        Serial.println(F("------------------------------------------------------"));
        Serial.println();
    }
    
    void runSettingsTestStartingWith(bool b) {
        setAllSettingsTo(b);
        Serial.println(F("Saving settings..."));
        printOutUserSettings();
        saveSettingsToEEPROM();
        
        setAllSettingsTo(!b);    //flip settings prior to loading them again to verify
        Serial.println(F("Loading settings..."));
        loadSettingsFromEEPROM();
        printOutUserSettings();        
    }



        //PROGMEM & STRING PROCESSING TEST METHODS
    void testStringProcessingMethods() {
        Serial.println();
        Serial.println();
        Serial.println(F("-------------------------------------"));
        Serial.println(F("Testing String Processing Methods"));
        Serial.println(F("-------------------------------------"));
        Serial.println(F("Printing a couple of PROGMEM strings...."));
        Serial.print(F(" 1 ->"));
        copyToBufferAndPrint(Intro_String);
        Serial.println();
        Serial.print(F(" 2 ->"));
        copyToBufferAndPrint(Hour_Select_String);
        
        Serial.println();
        Serial.println(F("Test Update Display1 with Current Time and Display2:"));
        updateDisplay1WithCurrentTime();
        updateDisplay2GivenSettings();
        testPrintDisplays();
        
        
  
        //updateShortTime.....method test
        //updateDisplayGivenSettings.....methodtest
        
        
        Serial.println(F("-------------------------------------"));
        Serial.println(F("String Processing Testing Complete"));
        Serial.println(F("-------------------------------------"));
        Serial.println();
        Serial.println();
    }

    void testFahrenheitToCelsiusConversion(int16_t tempasword) {
        TEMPERATUREasWORD = tempasword;
        strcpy(stringbuffer, "");    //clear it
        Serial.println(F("--------------------------"));
        Serial.println(F("Test Fahrenheit Conversion"));
        Serial.print(F("Given word: "));
        Serial.print(TEMPERATUREasWORD, BIN);
        Serial.print(F(" "));
        Serial.print(getTempHighByte(), DEC);
        Serial.print(F(" "));
        Serial.println(getTempLowByte(), DEC);
        addCelsiusTempToStringBuffer();
        strcat(stringbuffer, "   ");
        addFahrenheitTempToStringBuffer();
        Serial.print(stringbuffer);
        Serial.println();
        Serial.println(F("--------------------------"));
    }


}
