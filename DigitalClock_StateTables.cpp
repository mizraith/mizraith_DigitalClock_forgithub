/**
 * PART OF mizraith_DigitalClock package
 *
 * state transition table definition for our clock's state machine.
 *
 */

#include "DigitalClock_StateTables.h"

namespace mizraith_DigitalClock {

    /* -------------------------------------------------------------------------------------
     * State table(s)
     * -------------------------------------------------------------------------------------
     */
                                      //currentstate               InputEvent           nextstate             statefunction*
                                      //-------------------------+-----------------+-----------------------+-------------------
    const struct state mst1 PROGMEM = { STATE_STARTUP,         ANY_EVENT,        STATE_NORMAL,            transitionToNormal             };
    
    const struct state mst2 PROGMEM = { STATE_NORMAL,          UPDATE_EVENT,     STATE_NORMAL,            transitionToNormal             };
    const struct state mst3 PROGMEM = { STATE_NORMAL,          BUTTON_1_EVENT,   STATE_NORMAL,            doNothingTransition            };
    const struct state mst4 PROGMEM = { STATE_NORMAL,          BUTTON_2_EVENT,   STATE_DISPLAY_OPTIONS,   transitionToDisplayOptions     };
    const struct state mst5 PROGMEM = { STATE_NORMAL,          BUTTON_3_EVENT,   STATE_NORMAL,            doNothingTransition            };
    const struct state mst6 PROGMEM = { STATE_NORMAL,          BUTTON_4_EVENT,   STATE_SET_TIME,          transitionToSetTime            };
    
    const struct state mst7 PROGMEM = { STATE_DISPLAY_OPTIONS, ANY_EVENT,        STATE_DISPLAY_OPTIONS,   handleDisplayOptionsState      };
    
    const struct state mst8 PROGMEM = { STATE_SET_TIME,        ANY_EVENT,        STATE_SET_TIME,          handleSetTimeState             };
    

    PROGMEM const struct state * main_state_transitions[] = {
        &mst1, &mst2, &mst3, &mst4, &mst5, &mst6, &mst7, &mst8, ARRAY_END
    };

                                      //currentstate               InputEvent           nextstate             statefunction*
                                      //-------------------------+-----------------+-----------------------+-------------------
    const struct state dost1 PROGMEM =   { ANY_STATE,               BUTTON_3_EVENT,   STATE_NORMAL,            exitDisplayOptions                    };
    
    const struct state dost2 PROGMEM =   { STATE_DISPLAY_24HR,      UPDATE_EVENT,     STATE_DISPLAY_24HR,      transitionToDisplay24Hr               };
    const struct state dost3 PROGMEM =   { STATE_DISPLAY_24HR,      BUTTON_1_EVENT,   STATE_DISPLAY_24HR,      adjustDisplay24Hr                     };
    const struct state dost4 PROGMEM =   { STATE_DISPLAY_24HR,      BUTTON_2_EVENT,   STATE_DISPLAY_C,         transitionToDisplayC                  };
    const struct state dost5 PROGMEM =   { STATE_DISPLAY_24HR,      BUTTON_4_EVENT,   STATE_DISPLAY_24HR,      transitionToDisplay24Hr               };
    const struct state dost6 PROGMEM =   { STATE_DISPLAY_24HR,      ADJUST_EVENT,     STATE_DISPLAY_24HR,      adjustDisplay24Hr                     };
        
    const struct state dost7 PROGMEM =   { STATE_DISPLAY_C,         UPDATE_EVENT,     STATE_DISPLAY_C,         transitionToDisplayC                  };
    const struct state dost8 PROGMEM =   { STATE_DISPLAY_C,         BUTTON_1_EVENT,   STATE_DISPLAY_C,         adjustDisplayC                        };
    const struct state dost9 PROGMEM =   { STATE_DISPLAY_C,         BUTTON_2_EVENT,   STATE_DISPLAY_TEMP,      transitionToDisplayTemp               };
    const struct state dost10 PROGMEM =  { STATE_DISPLAY_C,         BUTTON_4_EVENT,   STATE_DISPLAY_C,         transitionToDisplayC                  };
    const struct state dost11 PROGMEM =  { STATE_DISPLAY_C,         ADJUST_EVENT,     STATE_DISPLAY_C,         adjustDisplayC                        };
    
    const struct state dost12 PROGMEM =  { STATE_DISPLAY_TEMP,      UPDATE_EVENT,     STATE_DISPLAY_TEMP,      transitionToDisplayTemp               };
    const struct state dost13 PROGMEM =  { STATE_DISPLAY_TEMP,      BUTTON_1_EVENT,   STATE_DISPLAY_TEMP,      adjustDisplayTemp                     };
    const struct state dost14 PROGMEM =  { STATE_DISPLAY_TEMP,      BUTTON_2_EVENT,   STATE_DISPLAY_DATE,      transitionToDisplayDate               };
    const struct state dost15 PROGMEM =  { STATE_DISPLAY_TEMP,      BUTTON_4_EVENT,   STATE_DISPLAY_TEMP,      transitionToDisplayTemp               };
    const struct state dost16 PROGMEM =  { STATE_DISPLAY_TEMP,      ADJUST_EVENT,     STATE_DISPLAY_TEMP,      adjustDisplayTemp                     };
    
    const struct state dost17 PROGMEM =  { STATE_DISPLAY_DATE,      UPDATE_EVENT,     STATE_DISPLAY_DATE,       transitionToDisplayDate              };    
    const struct state dost18 PROGMEM =  { STATE_DISPLAY_DATE,      BUTTON_1_EVENT,   STATE_DISPLAY_DATE,       adjustDisplayDate                    };
    const struct state dost19 PROGMEM =  { STATE_DISPLAY_DATE,      BUTTON_2_EVENT,   STATE_DISPLAY_DAYOFWEEK,  transitionToDisplayDayOfWeek         };
    const struct state dost20 PROGMEM =  { STATE_DISPLAY_DATE,      BUTTON_4_EVENT,   STATE_DISPLAY_DATE,       transitionToDisplayDate              };
    const struct state dost21 PROGMEM =  { STATE_DISPLAY_DATE,      ADJUST_EVENT,     STATE_DISPLAY_DATE,       adjustDisplayDate                    };
    
    const struct state dost22 PROGMEM =  { STATE_DISPLAY_DAYOFWEEK, UPDATE_EVENT,     STATE_DISPLAY_DAYOFWEEK,   transitionToDisplayDayOfWeek        };    
    const struct state dost23 PROGMEM =  { STATE_DISPLAY_DAYOFWEEK, BUTTON_1_EVENT,   STATE_DISPLAY_DAYOFWEEK,   adjustDisplayDayOfWeek              };
    const struct state dost24 PROGMEM =  { STATE_DISPLAY_DAYOFWEEK, BUTTON_2_EVENT,   STATE_DISPLAY_INFO,        transitionToDisplayInfo             };
    const struct state dost25 PROGMEM =  { STATE_DISPLAY_DAYOFWEEK, BUTTON_4_EVENT,   STATE_DISPLAY_DAYOFWEEK,   transitionToDisplayDayOfWeek        };
    const struct state dost26 PROGMEM =  { STATE_DISPLAY_DAYOFWEEK, ADJUST_EVENT,     STATE_DISPLAY_DAYOFWEEK,   adjustDisplayDayOfWeek              };
    
    const struct state dost27 PROGMEM =  { STATE_DISPLAY_INFO,      UPDATE_EVENT,     STATE_DISPLAY_INFO,        transitionToDisplayInfo             };    
    const struct state dost28 PROGMEM =  { STATE_DISPLAY_INFO,      BUTTON_1_EVENT,   STATE_DISPLAY_INFO,        adjustDisplayInfo                   };
    const struct state dost29 PROGMEM =  { STATE_DISPLAY_INFO,      BUTTON_2_EVENT,   STATE_DISPLAY_USERSTRING,  transitionToDisplayUserString       };
    const struct state dost30 PROGMEM =  { STATE_DISPLAY_INFO,      BUTTON_4_EVENT,   STATE_DISPLAY_INFO,        transitionToDisplayInfo             };
    const struct state dost31 PROGMEM =  { STATE_DISPLAY_INFO,      ADJUST_EVENT,     STATE_DISPLAY_INFO,        adjustDisplayInfo                   };
        
    const struct state dost32 PROGMEM =  { STATE_DISPLAY_USERSTRING, UPDATE_EVENT,     STATE_DISPLAY_USERSTRING, transitionToDisplayUserString       };    
    const struct state dost33 PROGMEM =  { STATE_DISPLAY_USERSTRING, BUTTON_1_EVENT,   STATE_DISPLAY_USERSTRING, adjustDisplayUserString             };
    const struct state dost34 PROGMEM =  { STATE_DISPLAY_USERSTRING, BUTTON_2_EVENT,   STATE_DISPLAY_24HR,       transitionToDisplay24Hr             };
    const struct state dost35 PROGMEM =  { STATE_DISPLAY_USERSTRING, BUTTON_4_EVENT,   STATE_DISPLAY_USERSTRING, transitionToDisplayUserString       };
    const struct state dost36 PROGMEM =  { STATE_DISPLAY_USERSTRING, ADJUST_EVENT,     STATE_DISPLAY_USERSTRING, adjustDisplayUserString             };
  
    PROGMEM const struct state * display_options_state_transitions[] = {
        &dost1,   &dost2,  &dost3,  &dost4,  &dost5,  &dost6,  &dost7,  &dost8,  &dost9, &dost10,
        &dost11, &dost12, &dost13, &dost14, &dost15, &dost16, &dost17, &dost18, &dost19, &dost20,
        &dost21, &dost22, &dost23, &dost24, &dost25, &dost26, &dost27, &dost28, &dost29, &dost30,
        &dost31, &dost32, &dost33, &dost34, &dost35, &dost36, ARRAY_END
    };

                                      //currentstate               InputEvent           nextstate             statefunction*
                                      //-------------------------+-----------------+-----------------------+-------------------
    const struct state stst1  PROGMEM =  { ANY_STATE,           BUTTON_3_EVENT,   STATE_NORMAL,            exitSetTime               };
    
    const struct state stst2  PROGMEM =  { STATE_SET_HOUR,      UPDATE_EVENT,     STATE_SET_HOUR,         transitionToSetHour        };        
    const struct state stst3  PROGMEM =  { STATE_SET_HOUR,      BUTTON_1_EVENT,   STATE_SET_HOUR,         adjustSetHour              };
    const struct state stst4  PROGMEM =  { STATE_SET_HOUR,      BUTTON_2_EVENT,   STATE_SET_MINUTE,       adjustSetHour              };
    const struct state stst5  PROGMEM =  { STATE_SET_HOUR,      BUTTON_4_EVENT,   STATE_SET_HOUR,         transitionToSetMinute      };
    const struct state stst6  PROGMEM =  { STATE_SET_HOUR,      ADJUST_EVENT,     STATE_SET_HOUR,         adjustSetHour              };
    
    const struct state stst7  PROGMEM =  { STATE_SET_MINUTE,    UPDATE_EVENT,     STATE_SET_MINUTE,       transitionToSetMinute      };    
    const struct state stst8  PROGMEM =  { STATE_SET_MINUTE,    BUTTON_1_EVENT,   STATE_SET_MINUTE,       adjustSetMinute            };
    const struct state stst9  PROGMEM =  { STATE_SET_MINUTE,    BUTTON_2_EVENT,   STATE_SET_MONTH,        adjustSetMinute            };
    const struct state stst10 PROGMEM =  { STATE_SET_MINUTE,    BUTTON_4_EVENT,   STATE_SET_MINUTE,       transitionToSetMonth       };
    const struct state stst11 PROGMEM =  { STATE_SET_MINUTE,    ADJUST_EVENT,     STATE_SET_MINUTE,       adjustSetMinute            };
    
    const struct state stst12 PROGMEM =  { STATE_SET_MONTH,     UPDATE_EVENT,     STATE_SET_MONTH,        transitionToSetMonth       };
    const struct state stst13 PROGMEM =  { STATE_SET_MONTH,     BUTTON_1_EVENT,   STATE_SET_MONTH,        adjustSetMonth             };
    const struct state stst14 PROGMEM =  { STATE_SET_MONTH,     BUTTON_2_EVENT,   STATE_SET_DAY,          adjustSetMonth             };
    const struct state stst15 PROGMEM =  { STATE_SET_MONTH,     BUTTON_4_EVENT,   STATE_SET_MONTH,        transitionToSetDay         };
    const struct state stst16 PROGMEM =  { STATE_SET_MONTH,     ADJUST_EVENT,     STATE_SET_MONTH,        adjustSetMonth             };

    const struct state stst17 PROGMEM =  { STATE_SET_DAY,       UPDATE_EVENT,     STATE_SET_DAY,          transitionToSetDay         };    
    const struct state stst18 PROGMEM =  { STATE_SET_DAY,       BUTTON_1_EVENT,   STATE_SET_DAY,          adjustSetDay               };
    const struct state stst19 PROGMEM =  { STATE_SET_DAY,       BUTTON_2_EVENT,   STATE_SET_YEAR,         adjustSetDay               };
    const struct state stst20 PROGMEM =  { STATE_SET_DAY,       BUTTON_4_EVENT,   STATE_SET_DAY,          transitionToSetYear        };
    const struct state stst21 PROGMEM =  { STATE_SET_DAY,       ADJUST_EVENT,     STATE_SET_DAY,          adjustSetDay               };
    
    const struct state stst22 PROGMEM =  { STATE_SET_YEAR,      UPDATE_EVENT,     STATE_SET_YEAR,         transitionToSetYear        };
    const struct state stst23 PROGMEM =  { STATE_SET_YEAR,      BUTTON_1_EVENT,   STATE_SET_YEAR,         adjustSetYear              };
    const struct state stst24 PROGMEM =  { STATE_SET_YEAR,      BUTTON_2_EVENT,   STATE_SET_DAYOFWEEK,    adjustSetYear              };
    const struct state stst25 PROGMEM =  { STATE_SET_YEAR,      BUTTON_4_EVENT,   STATE_SET_YEAR,         transitionToSetDayOfWeek   };
    const struct state stst26 PROGMEM =  { STATE_SET_YEAR,      ADJUST_EVENT,     STATE_SET_YEAR,         adjustSetYear              };
        
    const struct state stst27 PROGMEM =  { STATE_SET_DAYOFWEEK, UPDATE_EVENT,     STATE_SET_DAYOFWEEK,    transitionToSetDayOfWeek   };
    const struct state stst28 PROGMEM =  { STATE_SET_DAYOFWEEK, BUTTON_1_EVENT,   STATE_SET_DAYOFWEEK,    adjustSetDayOfWeek         };
    const struct state stst29 PROGMEM =  { STATE_SET_DAYOFWEEK, BUTTON_2_EVENT,   STATE_SET_HOUR,         adjustSetDayOfWeek         };
    const struct state stst30 PROGMEM =  { STATE_SET_DAYOFWEEK, BUTTON_4_EVENT,   STATE_SET_DAYOFWEEK,    transitionToSetHour        };
    const struct state stst31 PROGMEM =  { STATE_SET_DAYOFWEEK, ADJUST_EVENT,     STATE_SET_DAYOFWEEK,    adjustSetDayOfWeek         };
    

    PROGMEM const struct state * set_time_state_transitions[] =
    {
        &stst1,   &stst2,  &stst3,  &stst4,  &stst5,  &stst6,  &stst7,  &stst8,  &stst9, &stst10,
        &stst11, &stst12, &stst13, &stst14, &stst15, &stst16, &stst17, &stst18, &stst19, &stst20,
        &stst21, &stst22, &stst23, &stst24, &stst25, &stst26, &stst27, &stst28, &stst29, &stst30,
        &stst31, ARRAY_END 
    };


}

