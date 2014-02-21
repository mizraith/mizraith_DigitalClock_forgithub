/**
 * see .h file for info
 */
 


#include "mizraith_DigitalClock.h"

DigitalClock::DigitalClock( DateTime compiled_on ) {
    //NOTE: This next debug call is 'stressful' on your comm port...lots of serial traffic!
    //only enable this is you've changed the state machine tables and need to test all your methods
    //----------------------------------------------
//     mizraith_DigitalClock::testAllTransitions();
    //----------------------------------------------
    mizraith_DigitalClock::NOW = compiled_on;       //way of passing in compiled on time forward
    mizraith_DigitalClock::initializeData( compiled_on );        //load settings from EEPROM
    mizraith_DigitalClock::serialPrintNOW();
    mizraith_DigitalClock::startStateMachine();
}


char * DigitalClock::getDisplay1( void ) {
    return mizraith_DigitalClock::Display1Text;
}


char * DigitalClock::getDisplay2( void ) {
    return mizraith_DigitalClock::Display2Text;
}


void DigitalClock::handleInputEvent(uint8_t inputEventID, int16_t eventData) {
    mizraith_DigitalClock::handleEvent( inputEventID, eventData );
}

void DigitalClock::update( void ) {
    mizraith_DigitalClock::update();
}


void DigitalClock::setUserString(char * userstring) {
    mizraith_DigitalClock::setAndSaveUserString( userstring );
}


void DigitalClock::updateTemperature( void ) {
    mizraith_DigitalClock::updateTemperatureFromRTC();
}


void DigitalClock::setTime(DateTime t) {
    mizraith_DigitalClock::RTC_Interface->setTime( t );
}
       
       
        
DateTime DigitalClock::getTime(void) {
    return mizraith_DigitalClock::RTC_Interface->getTime();
}
        

      
