/**
 *  The mizraith_DigitalClock library.  
 *  Full featured RTC digital clock for the Arduino with user inputs
 * and time setting capability.  Alarm, etc, to be added soon!!! 
 *
* Red Byer    1/11/2014
* See http://github.com/mizraith
*
This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.


 */
 



#ifndef _mizraith_DigitalCLock_H_
#define _mizraith_DigitalClock_H_

    #include "DigitalClock_Data.h"
    #include "DigitalClock_Common.h"
    #include "DigitalClock_Strings.h"

    #include "DigitalClock_StatePackages.h"

    #include "DigitalClock_StringProcessing.h"

    #include "DigitalClock_TestMethods.h"

    //#include "RTC_DS3231.h"
    #include "RTClib.h"         //for the DateTime class
    //#include <avr/pgmspace.h>



    class DigitalClock {
        public:
            DigitalClock(DateTime compiled_on );
        
            char * getDisplay1(void);
            char * getDisplay2(void);
        
            void handleInputEvent(uint8_t inputEventID, int16_t eventData);
            void setUserString(char *userstring);
        
            void update(void);   //intended to be called every few-hundred milliseconds
            void updateTemperature(void); //intended to be called every few seconds (not as often)
                    
            void setTime(DateTime t);
        
            DateTime getTime(void);
     
    };



#endif