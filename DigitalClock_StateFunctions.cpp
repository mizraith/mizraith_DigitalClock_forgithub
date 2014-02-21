/**
 * PART OF mizraith_DigitalClock package
 *
 * State transition and state machine related functionality.
 * This is where "all" the work is really done.
 */
 

//#### TODO:  Set a timeout for sub-states


#include "DigitalClock_StateFunctions.h"


namespace mizraith_DigitalClock {

    /* -------------------------------------------------------------------------------------
     * STATE MACHINE SPECIFIC GLOBALS
     */
    bool DEBUG_STATE_TRANSITIONS = false;

    static uint8_t CurrentState = STATE_NORMAL;
    static uint8_t CurrentDisplayOptionsState = STATE_DISPLAY_24HR;
    static uint8_t CurrentSetTimeState = STATE_SET_HOUR;
    
    
    //make this a global that we share -- plugs memory leaks and tightens up use.
    //I guess I could always call free(tempstate) inside the function, too
    struct state *statescratchpad = new struct state; //temporary for loading data out of PROGMEM
    struct state *inputeventstate = new struct state; //use this to pass state info forward
    // -------------------------------------------------------------------------------------

 
 
    /** ---------------------------------------------------------------------
     *   STATE TRANSITION FUNCTIONS
     * ----------------------------------------------------------------------
     */
    //These typically do the following:
    //Adjust display information or set value
    //set the currentstate to the nextstate
    //return 0 if okay.

    // ------------------------------
    // Main State Transitions
    // ------------------------------

    void startStateMachine(void) 
    {    
        if(DEBUG_STATE_TRANSITIONS) {
           Serial.println(F("Starting State Machine"));
        }
    
        CurrentState = STATE_NORMAL;
        CurrentDisplayOptionsState = STATE_DISPLAY_24HR;
        CurrentSetTimeState = STATE_SET_HOUR;
        inputeventstate->currentstate = CurrentState;
        inputeventstate->eventID = BUTTON_3_EVENT;
        
        SubMenuTimeOutCounter = millis();
        startupState(inputeventstate);   
        return;
    }


    //The main entry point -- intended to be called regularly so
    //that state machine can update displays
    void update( void ) 
    {
        if(DEBUG_STATE_TRANSITIONS) {
            delay(250);   //slow it down or we drown the serial port
        }
        handleEvent(UPDATE_EVENT, 0);
    }

    void handleSubStateTimeout(uint8_t event) {
        long currenttime = millis();
        if( (event != UPDATE_EVENT) || (CurrentState == STATE_NORMAL) ) {
            //got a real event from user, or in NORMAL state
            SubMenuTimeOutCounter = currenttime;   //reset counter
        }
        
        if ( (CurrentState != STATE_NORMAL) && 
             (currenttime - SubMenuTimeOutCounter > SUBSTATE_TIMEOUT) ) {
            //user is stuck, kick us back to normal.
            SubMenuTimeOutCounter = currenttime;
            transitionToNormal(inputeventstate);
        }
    }

    //One of the main entry points into our state machine
    void handleEvent(uint8_t event, int16_t eventdata) 
    {
        if(DEBUG_STATE_TRANSITIONS) {
            Serial.print(F("handleEvent: "));
            Serial.println(event, DEC);
        }
        
        if( eventOutOfRange(event) ) {
//             Serial.print(F("ERROR: event out of range:"));
//             Serial.println(event, DEC);
            return;
        }
        
        InputEventValue = eventdata;
        //Note, we could complicate things by testing against the event
        //and zeroing if we detect an appropriate transition...but this behaves
        //quite fine as is
        AccumulatedEventValue += eventdata;  
        
        
        inputeventstate->currentstate = CurrentState;
        inputeventstate->eventID = event;  
        inputeventstate->nextstate = STATE_NORMAL;          //just in case, but need to change out down the line
        inputeventstate->func_p = transitionToNormal;   //just in case

        handleSubStateTimeout(event);
        
        handleMainState( inputeventstate );
        
    
        if(DEBUG_STATE_TRANSITIONS) {
           Serial.print(F("CurrentState: "));
           Serial.print(CurrentState, DEC);
           Serial.print(F("\tCurrentDisplayOptionsState: "));
           Serial.print(CurrentDisplayOptionsState, DEC);
           Serial.print(F("\tCurrentSetTimeState: "));
           Serial.println(CurrentSetTimeState, DEC);
        }
        return;
    }






    void handleMainState(const struct state *input_s_p) {
        if(DEBUG_STATE_TRANSITIONS) {
            printStateData(input_s_p);
            Serial.println(F("  handleMainState"));
        }
        const struct state **statetransitionlist = main_state_transitions;                    
        handleStateWorker(input_s_p, statetransitionlist, CurrentState);

        return;
    }

    void handleDisplayOptionsState(const struct state *input_s_p) 
    {
        if(DEBUG_STATE_TRANSITIONS) {
            printStateData(input_s_p);
            Serial.println(F("  handleDisplayOptionsState"));
        }
    
        const struct state **statetransitionlist = display_options_state_transitions;   
        handleStateWorker(input_s_p, statetransitionlist, CurrentDisplayOptionsState);
        return;
    }


    void handleSetTimeState(const struct state *input_s_p) 
    {
         if(DEBUG_STATE_TRANSITIONS) {
             printStateData(input_s_p);
             Serial.println(F("  handleSetTimeState"));
        }
    
        const struct state **statetransitionlist = set_time_state_transitions;        
        handleStateWorker(input_s_p, statetransitionlist, CurrentSetTimeState);
        return;
    }



    //handles event transition passed in from main state machine.
    //matching current input conditions (state and evnet) with
    //an element in the input state table.
    void handleStateWorker(const struct state *input_s_p, 
                                    const struct state **statetransitionlist, 
                                    uint8_t currentsubstate)
    {
        if(DEBUG_STATE_TRANSITIONS) {
            printStateData(input_s_p);
            Serial.println(F("  handleStateWorker"));
        }
        
        boolean foundit = false;

        for(uint8_t i=0; !foundit; i++) {
            state * StateAddressInProgmem = (state *)pgm_read_word_near(  &(statetransitionlist[i])  );

            if ( ((uint16_t)StateAddressInProgmem) == ARRAY_END) {
                break;
            }
            
            loadStateVariableFromProgmemAddr(statescratchpad, StateAddressInProgmem);
              
            if( (statescratchpad->currentstate == currentsubstate) || 
                (statescratchpad->currentstate == ANY_STATE) ) {
            
                if( (statescratchpad->eventID == input_s_p->eventID) || 
                    (statescratchpad->eventID == ANY_EVENT)     ) {
            
                    foundit = true;

                    inputeventstate->currentstate = input_s_p->currentstate;  //keep as is don't overwrite match on ANY_STATE
                    inputeventstate->eventID =      input_s_p->eventID;      //keep as is don't overwrite match on ANY_EVENT
                    inputeventstate->nextstate =   statescratchpad->nextstate;
                    inputeventstate->func_p    =   statescratchpad->func_p;

                    //NOW CALL IT
                    inputeventstate->func_p(inputeventstate);    //keep pushing the input_s_p forward
                    }
            }
        }
        if (!foundit) {
            if(DEBUG_STATE_TRANSITIONS) {
                Serial.println(F("!!! handleStateWorker: No transition for event. This may be normal."));
            }
        }
    
        return;
    }






    void doNothingTransition(const struct state *s_p) 
    {
        if(DEBUG_STATE_TRANSITIONS) {
            printStateData(s_p);
            Serial.println(F("  doNothingTransition"));
        }
        return;
    }
 
    void startupState(const struct state *s_p) 
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  startupToNormalTransition "));
        }  
        //WE RELY ON MAIN .INO FILE FOR "STARTUP DELAY" so these will show
        strcpy_P(Display1Text, Startup_String);
        strcpy_P(Display2Text, Intro_String);
        
        if (DISPLAY_TO_SERIAL) { testPrintDisplays();}

        CurrentState =  STATE_STARTUP;
        //will remain in this state until an update() or other trigger occurs.
        return;
    }  

 
 
    void transitionToNormal(const struct state *s_p) 
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  transitionToNormal "));
        }  
        updateDisplay1WithCurrentTime();
        updateDisplay2GivenSettings();
        if (DISPLAY_TO_SERIAL) { testPrintDisplays();}
    
        CurrentState =  STATE_NORMAL;
        return;
    } 
 
    void transitionToDisplayOptions(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  transitionToDisplayOptions "));
        }  
        
        CurrentState =  STATE_DISPLAY_OPTIONS;;
        transitionToDisplay24Hr(s_p);     //I don't like hardcoding this, but it works 
        return;
    } 


    void transitionToSetTime(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  transitionToSetTime "));
        }  
    
        CurrentState =  STATE_SET_TIME;;
        transitionToSetHour(s_p);     //I don't like hardcoding this, but it works
        return;
    } 







 
 
 
    // ------------------------------
    // Display Options Transitions
    //  -- these need to end the cycle
    // ------------------------------
    void exitDisplayOptions(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  exitDisplayOptions "));
        }  
        AccumulatedEventValue = 0;
        CurrentDisplayOptionsState = STATE_DISPLAY_24HR;
        transitionToNormal(s_p);
        return;
    }

    void transitionToDisplayTemp(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  transitionToDisplayTemp "));
        }  
        copyToDisplay2(Show_Temp_String);
        if (SETTINGS->DISPLAY_TEMP) {
            copyToDisplay1(Yes_String);
        } else {
            copyToDisplay1(No_String);
        }
        BlinkingDisplay1();
        
        
        if (DISPLAY_TO_SERIAL) { testPrintDisplays();}
    
        CurrentDisplayOptionsState = STATE_DISPLAY_TEMP;
        return;
    }

    void transitionToDisplayDate(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  transitionToDisplayDate "));
        }  
        copyToDisplay2(Show_Date_String);
        if (SETTINGS->DISPLAY_DATE) {
            copyToDisplay1(Yes_String);
        } else {
            copyToDisplay1(No_String);
        }
        BlinkingDisplay1();
        
        if (DISPLAY_TO_SERIAL) { testPrintDisplays();}
        
        CurrentDisplayOptionsState = STATE_DISPLAY_DATE;
        
        return;
    }

    void transitionToDisplayDayOfWeek(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  transitionToDisplayDayOfWeek "));
        }  
        copyToDisplay2(Show_DayOfWeek_String);
        if (SETTINGS->DISPLAY_DAYOFWEEK) {
            copyToDisplay1(Yes_String);
        } else {
            copyToDisplay1(No_String);
        }
        BlinkingDisplay1();
        
        if (DISPLAY_TO_SERIAL) { testPrintDisplays();}
        
        CurrentDisplayOptionsState = STATE_DISPLAY_DAYOFWEEK;
        return;
    }

    void transitionToDisplayInfo(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  transitionToDisplayInfo "));
        }  
        copyToDisplay2(Show_Info_String);
        if (SETTINGS->DISPLAY_INFO) {
            copyToDisplay1(Yes_String);
        } else {
            copyToDisplay1(No_String);
        }
        BlinkingDisplay1();
        
        if (DISPLAY_TO_SERIAL) { testPrintDisplays();}
        
        CurrentDisplayOptionsState = STATE_DISPLAY_INFO;
        return;
    }

    void transitionToDisplayUserString(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  transitionToDisplayUserString "));
        }  
        copyToDisplay2(Show_User_String);
        if (SETTINGS->DISPLAY_USERSTRING) {
            copyToDisplay1(Yes_String);
        } else {
            copyToDisplay1(No_String);
        }
        BlinkingDisplay1();
        
        if (DISPLAY_TO_SERIAL) { testPrintDisplays();}
        
        CurrentDisplayOptionsState = STATE_DISPLAY_USERSTRING;
        return;
    }


    void transitionToDisplay24Hr(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  transitionToDisplay24Hr "));
        }  
        
        copyToDisplay2(Hour_Select_String);
        if (SETTINGS->USE_24_HOUR) {
            copyToDisplay1(TwentyFour_Hour_String);
        } else {
            copyToDisplay1(Twelve_Hour_String);
        }
        BlinkingDisplay1();
        
        if (DISPLAY_TO_SERIAL) { testPrintDisplays();}
        
        
        CurrentDisplayOptionsState = STATE_DISPLAY_24HR;
        return;
    }

    void transitionToDisplayC(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  transitionToDisplayC "));
        }  
        
        copyToDisplay2(Temp_Select_String);
        if (SETTINGS->USE_CELSIUS) {
            copyToDisplay1(Celsius_String);
        } else {
            copyToDisplay1(Fahrenheit_String);
        }
        BlinkingDisplay1();
        
        if (DISPLAY_TO_SERIAL) { testPrintDisplays();}
        
        
        CurrentDisplayOptionsState = STATE_DISPLAY_C;
        return;
    } 
 
 
    // ------------------------------
    // Display Options Adjustments
    //  -- these complete and then call their
    // transition state to update the display
    // ------------------------------
    void adjustDisplay24Hr(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  adjustDisplay24Hr "));
        }  else if( meetsBinaryAdjustmentCriteria(s_p) ) {
            SETTINGS->USE_24_HOUR =  !(SETTINGS->USE_24_HOUR);  
            saveSettingsToEEPROM();
        }

        CurrentDisplayOptionsState = s_p->nextstate;   //is this necessary?
        transitionToDisplay24Hr(s_p);
        return;
    }
    

    
    void adjustDisplayC(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  adjustDisplayC "));
        }  else if( meetsBinaryAdjustmentCriteria(s_p) ) {
                SETTINGS->USE_CELSIUS = !(SETTINGS->USE_CELSIUS);
                saveSettingsToEEPROM();
        }

        CurrentDisplayOptionsState = s_p->nextstate;   //is this necessary?
        transitionToDisplayC(s_p);
        return;
    }
    

    void adjustDisplayTemp(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  adjustDisplayTime "));
        }  else if( meetsBinaryAdjustmentCriteria(s_p) ) {
            SETTINGS->DISPLAY_TEMP = !(SETTINGS->DISPLAY_TEMP);
            saveSettingsToEEPROM();
        }

        CurrentDisplayOptionsState = s_p->nextstate;  //is this necessary?
        transitionToDisplayTemp(s_p); 
        return;
    }

    void adjustDisplayDate(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  adjustDisplayDate "));
        }  else if( meetsBinaryAdjustmentCriteria(s_p) ) {
            SETTINGS->DISPLAY_DATE = !(SETTINGS->DISPLAY_DATE);
            saveSettingsToEEPROM();
        }
        
        CurrentDisplayOptionsState = s_p->nextstate;    //is this necessary?
        transitionToDisplayDate(s_p);
        return;
    }

    void adjustDisplayDayOfWeek(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  adjustDisplayDayOfWeek "));
        }  else if( meetsBinaryAdjustmentCriteria(s_p) ) {
            SETTINGS->DISPLAY_DAYOFWEEK = !(SETTINGS->DISPLAY_DAYOFWEEK);
            saveSettingsToEEPROM();
        }        

        CurrentDisplayOptionsState = s_p->nextstate;
        transitionToDisplayDayOfWeek(s_p);       //is this necessary?
        return;
    }

    void adjustDisplayInfo(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  adjustDisplayInfo "));
        }  else if( meetsBinaryAdjustmentCriteria(s_p) ) {
            SETTINGS->DISPLAY_INFO = !(SETTINGS->DISPLAY_INFO);
            saveSettingsToEEPROM();
        } 
        
        CurrentDisplayOptionsState = s_p->nextstate;
        transitionToDisplayInfo(s_p);         //is this necessary?
        return;
    }

    void adjustDisplayUserString(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  adjustDisplayUserString "));
        }  else if( meetsBinaryAdjustmentCriteria(s_p) ) {
            SETTINGS->DISPLAY_USERSTRING = !(SETTINGS->DISPLAY_USERSTRING);
            saveSettingsToEEPROM();
        } 
        
        CurrentDisplayOptionsState = s_p->nextstate;
        transitionToDisplayUserString(s_p);         //is this necessary?
        return;
    } 
 
    // ------------------------------
    // Set Time Transitions
    //  -- these need to end the call cycle
    // ------------------------------ 
    void exitSetTime(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  exitSaveTime "));
        }  
        AccumulatedEventValue = 0;
        CurrentSetTimeState = STATE_SET_HOUR;
        transitionToNormal(s_p);
        return;
    }
       
    void transitionToSetMinute(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  transitionToSetMinute "));
        }  
        updateDisplay1WithCurrentTimeBlinkingSpan(3,4);
        copyToDisplay2( Set_Minute_String );
        
        if (DISPLAY_TO_SERIAL) { testPrintDisplays();}       
        
        CurrentSetTimeState = STATE_SET_MINUTE;
        return;
    }
    
    void transitionToSetMonth(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  transitionToSetMonth "));
        }  
        updateDisplay1WithCurrentMonthAndDateBlinkingSpan(3,4);
        copyToDisplay2( Set_Month_String );
        
        if (DISPLAY_TO_SERIAL) { testPrintDisplays();}
        
        CurrentSetTimeState = STATE_SET_MONTH;
        return;
    }   
    
    void transitionToSetDay(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  transitionToSetDay "));
        }  
        updateDisplay1WithCurrentMonthAndDateBlinkingSpan(6,7);
        copyToDisplay2( Set_Day_String );
        
        if (DISPLAY_TO_SERIAL) { testPrintDisplays();}
        
        CurrentSetTimeState = STATE_SET_DAY;
        return;
    }    
    
    void transitionToSetYear(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  transitionToSetYear "));
        }  
        updateDisplay1WithCurrentYearBlinkingSpan(4,7);
        copyToDisplay2( Set_Year_String );
        
        if (DISPLAY_TO_SERIAL) { testPrintDisplays();}     
        
        CurrentSetTimeState = STATE_SET_YEAR;
        return;
    }

    void transitionToSetDayOfWeek(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  transitionToSetDayOfWeek "));
        }  
        updateDisplay1WithCurrentDayOfWeek();
        BlinkingDisplay1();
        copyToDisplay2( Set_DayOfWeek_String );
        
        if (DISPLAY_TO_SERIAL) { testPrintDisplays();}     
        
        CurrentSetTimeState = STATE_SET_DAYOFWEEK;
        return;
    }


    void transitionToSetHour(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  transitionToSetHour "));
        }  
        updateDisplay1WithCurrentTimeBlinkingSpan(0,1);
        copyToDisplay2( Set_Hour_String );
        
        if (DISPLAY_TO_SERIAL) { testPrintDisplays();}    
        
        CurrentSetTimeState = STATE_SET_HOUR;
        
        if(DEBUG_STATE_TRANSITIONS) {
          Serial.print(F("FREE RAM AT DEPTH: "));
          Serial.println(freeRam(), DEC);
       }
        return;
    }


    
    
    // ------------------------------
    // Set Time Adjustments
    //  -- these call the transition method when done
    // in order to update the display 
    // ------------------------------ 
    void adjustSetHour(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  adjustSetHour "));
        }  
        
        int8_t adj = getAdjustment(s_p, EIGHTH_TURN_ADJUST_THRESHOLD);
        
        if(adj != 0) {
            int8_t hour = NOW.hour();
            hour = hour + adj;
            hour = (hour % 24);    //keep it looping
            DateTime x = DateTime(NOW.year(), NOW.month(), NOW.day(), hour, NOW.minute());
            NOW = x;
            RTC_Interface->setTime(x);
        }
       CurrentSetTimeState = s_p->nextstate;
       if(DEBUG_STATE_TRANSITIONS) {
         Serial.print(F("FREE RAM AT DEPTH: "));
         Serial.println(freeRam(), DEC);
       }
       
       transitionToSetHour(s_p);
       return;
    }

    void adjustSetMinute(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  adjustSetMinute "));
        }  
        
        int8_t adj = getAdjustment(s_p, SIXTEENTH_TURN_ADJUST_THRESHOLD);
        
        if(adj != 0) {
            int8_t minute = NOW.minute();
            minute = minute + adj;
            minute = (minute % 60);    //keep it looping
            DateTime x = DateTime(NOW.year(), NOW.month(), NOW.day(), NOW.hour(), minute);
            NOW = x;
            RTC_Interface->setTime(x);
        }
          
        transitionToSetMinute(s_p); 
        return;    
    }


    void adjustSetMonth(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  adjustSetMonth "));
        } 
        
        int8_t adj = getAdjustment(s_p, QUARTER_TURN_ADJUST_THRESHOLD);
        
        if(adj != 0) {
            int8_t month = NOW.month(); 
            month = month + adj;
            month = (month % 12);    //keep it looping
            if(month == 0) { month = 12; }
            DateTime x = DateTime(NOW.year(), month, NOW.day(), NOW.hour(), NOW.minute());
            NOW = x;
            RTC_Interface->setTime(x);
        }
        
        transitionToSetMonth(s_p); 
        return;
    }


    void adjustSetDay(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  adjustSetDay "));
        }  
        int8_t adj = getAdjustment(s_p, EIGHTH_TURN_ADJUST_THRESHOLD);
        
        if(adj != 0) {
            long x = NOW.unixtime();
            x = x + (adj * 86400);    //number of seconds in a day
            NOW = x;
            RTC_Interface->setTime(x);
        }
        
        transitionToSetDay(s_p);
        return ;
    }


    void adjustSetYear(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  adjustSetYear "));
        }  
        int8_t adj = getAdjustment(s_p, QUARTER_TURN_ADJUST_THRESHOLD);
        
        if(adj != 0) {
            uint16_t year = NOW.year();
            year = year + adj;
            DateTime x = DateTime(year, NOW.month(), NOW.day(), NOW.hour(), NOW.minute());
            NOW = x;
            RTC_Interface->setTime(x);
        }

        transitionToSetYear(s_p);
        return;
    }

    void adjustSetDayOfWeek(const struct state *s_p)
    {
        if(DEBUG_STATE_TRANSITIONS) {
             printStateData(s_p);
             Serial.println(F("  adjustSetDayOfWeek "));
        }  
        int8_t adj = getAdjustment(s_p, EIGHTH_TURN_ADJUST_THRESHOLD);
        
        if(adj != 0) {
            int8_t dow = DayOfTheWeek;
            dow = dow + adj;
            dow = (dow % 7);
            if(dow < 0 ) { 
                dow = 7 + dow;  //poor man's abs value calc of mod
            }     
            DayOfTheWeek = dow;
            RTC_Interface->setDayOfWeek(DayOfTheWeek);
        }

        transitionToSetDayOfWeek(s_p);
        return;
    }
 
 
 
     /** ---------------------------------------------------------------------
     *   HELPER METHODS
     * ----------------------------------------------------------------------
     */
    void loadStateVariableFromProgmemAddr( struct state *statescratch, state * StateAddressInProgmem) {
        statescratch->currentstate = (uint8_t) pgm_read_byte_near( ((uint16_t)StateAddressInProgmem) + OFFSET_CURRENTSTATE);
        statescratch->eventID   =    (uint8_t) pgm_read_byte_near( ((uint16_t)StateAddressInProgmem) + OFFSET_EVENTID);
        statescratch->nextstate =    (uint8_t) pgm_read_byte_near( ((uint16_t)StateAddressInProgmem) + OFFSET_NEXTSTATE);
        statescratch->func_p  =      (state_func  *) pgm_read_word_near( ((uint16_t)StateAddressInProgmem) + OFFSET_FUNCTION);
//             Serial.print("StateAddressInProgmem: ");
//             Serial.println((uint16_t)StateAddressInProgmem, DEC);
//             Serial.print("current:");
//             Serial.print(statescratch->currentstate);
//             Serial.print("  eventID:");
//             Serial.print(statescratch->eventID);
//             Serial.print("  nextstate:");
//             Serial.print(statescratch->nextstate);
//             Serial.print("  func_p:");
//             Serial.println((uint16_t)statescratch->func_p, DEC);
    }
     
     
    void printStateData(const struct state *s_p) 
    {
        Serial.print(F("(curr, eventID, next): "));
        Serial.print(s_p->currentstate, DEC);
        Serial.print(F("\t"));
        Serial.print(s_p->eventID, DEC);
        Serial.print(F("\t"));
        Serial.print(s_p->nextstate, DEC);
        Serial.print(F("\t"));
        Serial.print(F("    CurrentState = "));
        Serial.print(CurrentState, DEC);
        Serial.print(F("    CurrentDispOptState = "));
        Serial.print(CurrentDisplayOptionsState, DEC);
        Serial.print(F("    CurrentSetTimeState = "));
        Serial.print(CurrentSetTimeState, DEC);
    }
 

    bool eventOutOfRange(uint8_t event) {
        if ( (event>=INPUT_EVENT_OFFSET) && (event < MAX_NUMBER_OF_INPUT_EVENTS)) {
            return false;
        } else {
            return true;
        }
    }

    bool meetsBinaryAdjustmentCriteria(const struct state *s_p) {
        if (s_p->eventID == BUTTON_1_EVENT) {
            return true;  
        } 
        
        if ( (s_p->eventID == ADJUST_EVENT) && 
                    (binaryAdjustmentThresholdReachedThenClearAccumulated()) ) { 
            return true;
        }
       
        return false;
    }


    bool binaryAdjustmentThresholdReachedThenClearAccumulated( void ) {
        if (adjustmentThresholdReachedThenClearAccumulated(QUARTER_TURN_ADJUST_THRESHOLD)) {
            return true;
        } else {
            return false;
        }
    }


    bool adjustmentThresholdReachedThenClearAccumulated( uint8_t thresh )  {
        if ( (AccumulatedEventValue >= thresh) || 
             (AccumulatedEventValue <= -thresh) ) {
                 AccumulatedEventValue = 0;
                 return true;
        } else {
            return false;
        }
    
    }

    int8_t getAdjustment(const struct state *s_p, uint8_t thresh) {
        if (s_p->eventID == BUTTON_1_EVENT) {
            return 1;
        } else if (s_p->eventID == BUTTON_2_EVENT) {
            return -1;
        } else if ( (s_p->eventID == ADJUST_EVENT) &&
                    (adjustmentThresholdReachedThenClearAccumulated( thresh )) ) {
            if ( InputEventValue >= 1 ) {
                return 1;
            } else {
                return -1;
            }    
        }
        return 0;
    }
    

}
//end namespace section




 
