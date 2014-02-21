/**
 * PART OF mizraith_DigitalClock package
 *
 * common strings used by pieces of the Digital Clock. Note
 * we save RAM by using PROGMEM
 *
 */
 
#ifndef _mizraith_DigitalClock_CtoF_H_
#define _mizraith_DigitalClock_CtoF_H_

#include <Arduino.h>
//#include <avr/pgmspace.h>

//Developer Note:  Yes, all these need to be given a value
//prior to user access.  The class and RTC setups do this
//for us long before we (or a user) queries their value
//
// While the syntax can be tricky, much of this was verified based on testing ("PROMEM_Testing" Sketch)
namespace mizraith_DigitalClock {

    //where 0C is table[0] = 32
    const int8_t MAX_TEMP_TABLE_LENGTH = 53;
    const int8_t MAX_TEMP_TABLE_LENGTH_NEG = 30;
    
    const int8_t CtoF_Table[MAX_TEMP_TABLE_LENGTH] =
    {    
       //0   1  2   3   4   5   6   7   8   9  
        32, 33, 35, 37, 39, 41, 42, 44, 46, 48,
       //10  11  12  13  14  15  16  17  18  19
         50, 51, 53, 55, 57, 59, 60, 62, 64, 66,
       //20  21  22  23  24  25  26  27  28  29
         68, 69, 71, 73, 75, 77, 78, 80, 82, 84,
       //30  31  32  33  34  35  36  37  38  39
         86, 87, 89, 91, 93, 95, 96, 98, 100, 102,
       //40   41   42   43   44   45   46   47   48   49   50    51    52
         104, 105, 107, 109, 111, 113, 114, 116, 118, 120,  122,  123, 125
    };
        
    //the fractional component rotates in this pattern  0, 0.8, 0.6, 0.4, 0, ...
    const int8_t CtoF_fractional[5] = { 0, 80, 60, 40, 20};
    
    const int8_t CtoF_Table_Negative[30] = 
    {
       //0, -1, -2, -3, -4, -5, -6, -7, -8, -9
        32, 30, 28, 26, 24, 23, 21, 19, 17, 15, 
       //-10, -11, -12, -13, -14, -15, -16, -17, -18, -19,
          14,  12,  10,  8,   6,    5 ,  3 ,  1,  0,  -2,  
       //-20, -21, -22, -23, -24, -25, -26, -27, -28, -29
          -4,  -5,  -7,  -9, -11, -13, -14, -16, -18, -20
    };
    
    const int8_t CtoF_fractional_neg[5] = { 0, 20, 40, 60, 80};
        
    
}


#endif