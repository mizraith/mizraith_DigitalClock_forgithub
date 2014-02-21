/**************************************************************************
 * Extensive clock using dual HDSP2111's and a LIghtwave Electronics
 * front laser panel interface.  Requires significant hardware layer.
 * However, digital clock object is isolated from display objects, so
 * it would be straightforward to use the DigitalClock and display
 * out on an LCD or some other display.
 *
 * Be sure to check out license.txt and README.txt files
 * 
 * See schematic PDF in package folder for example of hardware layer.
 *
 * REQUIRES >1kb of SRAM in current implementation, and nearly 25kb of prog flash.
 *      This means you need a ATMega321 (think Nano or ProMini)
 *
 * FUNCTIONALITY BASIcS:
 *   (1) Drives an HDSP2111 through an MCP23017 port expander
 *   (2) responding to button pressed via interrups on a second MCP23017
 *   (3) working with a DS3231 RTC and displaying on the HDSP2111
 *   (4) Reading an encoder.
 *   (5) full table-driven state machine backing the digital clock allows for
 *        easy expansion of functionality, or editing of what things do what.
 *   (6) Ability to set user 
 *
 *
 * http://github.com/mizraith
 * 8/28/2013  Red Byer - Author - original date
 *
 * 12/29/2013 
 * v0.5 2/18/2014
 * v1.0 2/21/2014 RELEASE
 ************************************************************************* */
 
 /******************************************************************************
 DATA CONNECTIONS:   (you're on your own for power/ground connections) 
    Arduino A5  -->  SCL    (i2C clock)
    Arduino A4  -->  SDA    (i2C data)
    MCP23017 port A (pins 21:28)  [Hook up a button anywhere to this port and ground]
    MCP23017 port B (pins 1:8)    [Hook up a button anywhere to this port and ground]
    
    MCP23017 INTA or INTB output pins to Arduino INT0 (D2) pin
    
    D13 - onboard LED (Arduino Nano).
    
    Be sure to set the address bits (A2A1A0) on your chip to match addressing below
    
 *******************************************************************************/
#include <Arduino.h>
#include <Wire.h>
#include <avr/pgmspace.h>
#include <EEPROM.h>

#include "Adafruit_MCP23017.h"
#include "mizraith_HDSP2111.h"
#include "mizraith_TimerHelper.h"
#include "RTClib.h"
#include "mizraith_DigitalClock.h"

 
//ARDUINO PINS
#define LED_PIN 13
#define INT_PIN 2
#define RTC_SQW_IN 5     // input square wave from RTC into T1 pin (D5)
                         //WE USE TIMER1 so that it does not interfere with Arduino delay() command

//---------MCP23017 #2 PINS------------------------- 
// For HDSP2111 connection, see the mizraith_HDSP2111.h file
//
//On the MCP23017 there is PORT A and PORT B
//The library numbers these pins (for convenience) 0:15
//the following are for other inputs for MCP23017

// for the buttons
#define BUTTON_SELECT   1     //map to BUTTON 1 EVENT
#define BUTTON_DISPLAY  2     //map to BUTTON 2 EVENT
#define BUTTON_STANDBY  4     //map to BUTTON 3 EVENT
#define BUTTON_SET      8     //map to BUTTON 4 EVENT

#define SELECT_EVENT mizraith_DigitalClock::BUTTON_1_EVENT
#define DISPLAY_EVENT mizraith_DigitalClock::BUTTON_2_EVENT
#define STANDBY_EVENT mizraith_DigitalClock::BUTTON_3_EVENT
#define SET_EVENT mizraith_DigitalClock::BUTTON_4_EVENT
#define ENCODER_EVENT mizraith_DigitalClock::ADJUST_EVENT


const DateTime COMPILED_ON = DateTime(__DATE__, __TIME__);  


//--------------GLOBALS ------------------------------
//
//----------------------------------------------------
mizraith_HDSP2111 mcp_HDSP2111s;
Adafruit_MCP23017 mcp_buttons;
static DigitalClock *DIGITAL_CLOCK;

const uint8_t mcp_display_addr = 0b00000000;   //i2C chip address for the display
const uint8_t mcp_buttons_addr = 0b00000001;   //i2C chip address for the buttons

int incomingByte = 0;

boolean StartingUp = true;
volatile unsigned long start_time;    
#define START_DELAY 11000  //amount of time to display startup strings  



//-------------INTERRUPT AND OTHER GLOBALS--------------
#define PWM_COUNT 1020   //determines how often the LED flips
volatile long TOGGLE_COUNT = 0;  //used as interrupt counter with PWM_COUNT

//track interrupt time for "debouncing" purposes
volatile unsigned long int_time = 0;        

//flag telling main loop to report on interrupt
volatile boolean serviceint = false;        

//In my case (bad button, no RC debounce), I find I cannot push button faster than 100ms anyway.
#define DEBOUNCE_DELAY 200

// storage for recording interrupt conditions
uint16_t intflagAB=0, 
          intcapAB=0, 
          valAB=0, 
          btncnt=0;   

uint8_t INTCAPA = 0xFF;  


//time (ms) between valid button presses 
unsigned long last_int_time = 0;
unsigned long this_loop_time = 0;

//loop timers
unsigned long last_display1update_time = 0;
unsigned long last_display2update_time = 0;


/***************************************************
 *   ARDUINO INTERRUPT LOGIC
 ***************************************************/

/**
 * It's important to keep this method TIGHT.  
 * We set a flag and move on, as doing anything real
 * (like Serial.print) can cause problems..many of
 * those commands use interrupts themselves.
 */
ISR(INT0_vect) {
    if(serviceint) {
      return;                             //the last int has not been serviced yet, could be a bounce
    } else {
      int_time = millis();
      serviceint = true;                  //set flag to service
    }
}


ISR(TIMER1_COMPA_vect) {
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  TOGGLE_COUNT++;
}


//We're using PCINT2 vector, but we could also move one of these
//pins to D3 and use the INT1 vector.  Instead we chose
//to save INT1 for future needs.  
#define encoder0PinA 6
#define encoder0PinB 7
volatile int16_t encoder0Pos = 0;
/* If pinA and pinB are both high or both low, it is spinning
   * forward. If they're different, it's going backward.
   */
ISR( PCINT2_vect) {
 //ASSUMES WE ARE ONLY INTERRUPTING ON OUR D6 PIN AND NOTHING ELSE
 //This is 'half" resolution...but that's fine for our use here.
 //one of the pins D0 to D7 has changed) 
  digitalWrite(LED_PIN, !digitalRead(LED_PIN) );   
  if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) {
    encoder0Pos--;
  } else {
    encoder0Pos++;
  }
}


uint8_t input_event, input_event_data;
bool send_event;

//--------------------------------------------------------
//  SERIAL INPUT FUNCTION
// ACTS IMMEDIATELY ONE CHAR AT A TIME
//--------------------------------------------------------
char c;
const char NONE = -1;
const char MAX_USER_STRINGLENGTH = 32;  //be careful not to overrun DigitalClock_Data.h MAX_USER_STRING_SIZE
bool accumulatestring = false;
char userstringbuff[MAX_USER_STRINGLENGTH];

void serialEvent() {
    c = Serial.read();  
    if (  (c == '\r') || 
          (c == '\n') )  {
       c = NONE;
       if(accumulatestring) {
           accumulatestring = false;
           Serial.print(F("USER STRING SET->"));
           Serial.println(userstringbuff);
           DIGITAL_CLOCK->setUserString(userstringbuff);
       }
    }
   
    if( c == '>') {
        accumulatestring = true;
        strcpy(userstringbuff, "");  
    }
    
    if(accumulatestring) {
      if ( c == '>') {
        return;  //don't accumulate these
      }  else {
        if (strlen(userstringbuff) > (MAX_USER_STRINGLENGTH - 2) ) {
          return;     
        } else {
          char cbuff[2] = " ";  //char plus null0
          cbuff[0] = c;
          strcat(userstringbuff,  cbuff);
        }
      }  
    } else if (c != NONE) {
      //SEE IF USER ENTERED A MODE NUMBER
      int x = c - 48;     //simple c to i conversion minus 1 for array indexing :  ord for 0 is 48. 
      if ( (x >= mizraith_DigitalClock::INPUT_EVENT_OFFSET ) && (x < mizraith_DigitalClock::MAX_NUMBER_OF_INPUT_EVENTS ) ) {
        input_event = x;
        input_event_data = 0;
        Serial.print(F("Input Event Via Serial: "));
        Serial.println(x);
        DIGITAL_CLOCK->handleInputEvent(input_event, input_event_data);
      } else {
          //Serial.println(F("Invalid state given"));
      }
        c = NONE;       //reset
    }
}
  
  
  
  
  
  
  
  
  
  
  


/***************************************************
 *   SETUP
 ***************************************************/

void setup() {
//  uint8_t temp;
  uint16_t tempwide;
  
  Serial.begin(57600);
  
  serialPrintHeaderString();
  
  checkRAMandExitIfLow(0);
  
  //---------------------------------------------------------
  // SETUP ARDUINO PIN MODES
  //--------------------------------------------------------- 
  pinMode(INT_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(RTC_SQW_IN, INPUT);
  //  digitalWrite(INT_PIN, HIGH);  //set pullup -- not necessary in this case. Hi-Z is better
  
  mcp_HDSP2111s.setup(mcp_display_addr);
  mcp_HDSP2111s.resetDisplays();     
  
  //DEBUG_PrintHDSP1111Strings();
    
  setupButtonMCP();
  setupArduinoINT0();
  setupArduinoPCINT2();
  
  mizraith_TimerHelper mth;
  mth.setupTimer1ForCounting((int)PWM_COUNT); 
//  mth.printTimer1Info();      
   
  DIGITAL_CLOCK =  new DigitalClock(COMPILED_ON); 
  mizraith_DigitalClock::printOutUserSettings();
 
  
  //The folloiwng has the effect of setting the HDSP2111 text pointers
  //to the memory space of the state machine strings.  This gives
  //state machine control over those strings, removing need to constantly
  //call "setnewstring"
  mcp_HDSP2111s.setDisplayStringAsNew(DIGITAL_CLOCK->getDisplay1(), 1);
  mcp_HDSP2111s.setDisplayStringAsNew(DIGITAL_CLOCK->getDisplay2(), 2);
  
  //mcp_HDSP2111s.DEBUG_PrintDisplayData();

  checkRAMandExitIfLow(1);
  

  last_int_time = millis();
  start_time = last_int_time;
  tempwide = mcp_buttons.readGPIOAB();    //This will clear interrupts on MCP prior to entering main loop.
  
 //Test and display all currently loaded easter eggs
 //mizraith_DigitalClock::test_EasterEggMethods();
 
   printSerialInputInstructions();
  
}




/***************************************************
 *   SETUP HELPERS
 ***************************************************/

void serialPrintHeaderString() {
  Serial.println();
  Serial.println();
  Serial.println(F("#######################################"));
  Serial.println(F("Lightwave Electronics Front Panel Clock"));
  Serial.println(F("---------------------------------------"));
  Serial.println(F("Red Byer    www.redstoyland.com"));
  Serial.println(F("http://github.com/mizraith"));
  Serial.println(F("VERSION DATE: 2-21-2014"));
  Serial.print(F("COMPILED ON: "));
  Serial.print(COMPILED_ON.month());
  Serial.print(F("/"));
  Serial.print(COMPILED_ON.day());
  Serial.print(F("/"));
  Serial.print(COMPILED_ON.year());
  Serial.print(F(" @ "));
  Serial.print(COMPILED_ON.hour(), DEC);
  Serial.println(COMPILED_ON.minute(), DEC); 
  Serial.println(F("#######################################"));
  Serial.println();
}

void printSerialInputInstructions( void ) {
    Serial.println();
    Serial.println(F("--------------------------------------------------")); 
    Serial.println(F("Valid serial inputs: 0:7  >userstringmax30char"));
    Serial.println(F("--------------------------------------------------"));  
}


void setupButtonMCP() {  
  uint16_t tempwide = 0;
  
  //---------------------------------------------------------
  // SETUP MCP23017 FOR BUTTONS
  //---------START MCP BUTTON COMMUNICATIONS ---------------
//  Serial.println(F("-----MCP Button Setup Follows-----"));
//  Serial.print(F("Setting up buttons MCP23017 at address: "));
//  Serial.print( MCP23017_ADDRESS | mcp_buttons_addr , BIN);
//  Serial.println();
  
  mcp_buttons.begin(mcp_buttons_addr);   // use i2C address for display
  mcp_buttons.setGPIOABMode(0xFFFF);     // 1=input 0=output  set all as inputs
  
  //--------ENABLE PULLUPS FOR BUTTONS  (default is off)-------------------
  mcp_buttons.setGPIOABPullUp(0xFFFF);    //all enabled
  tempwide = mcp_buttons.get2Registers( MCP23017_GPPUA );
//  Serial.print(F("Buttons  GPPUA/B: "));
//  Serial.print(tempwide, BIN);      //0 = pull up disabled, 1=pull up enabled
//  Serial.println();
  
  //--------VERIFY PORT DIRECTION SETUP-------------------
//  tempwide = mcp_display.get2Registers( MCP23017_IODIRA );
//  Serial.print("Display IODIRA/B: ");
//  Serial.print(tempwide, BIN);
//  Serial.println();
  tempwide = mcp_buttons.get2Registers( MCP23017_IODIRA );
//  Serial.print(F("Buttons IODIRA/B: "));
//  Serial.print(tempwide, BIN);
//  Serial.println();
//  Serial.println();
  
  //--------ENABLE MCP INTERRUPTS  (default is off)-------------------
  mcp_buttons.setInterruptMirror( 1 );       //enable interrupt mirroring, so either port works
  mcp_buttons.setInterruptPolarity ( 1 );    // 1= active high
//  mcp.setInterruptControlAB( 0x0000);   //all pins interrupt on change only. Would catch button press & depress
  mcp_buttons.setInterruptControlAB( 0xFFFF ); //all pins compared against a default value. interrupt on mismatch
  mcp_buttons.setDefaultValueAB ( 0xFFFF);    //set default val for all pins to 1, since pullups enabled.
  //finally
  mcp_buttons.setInterruptAB( 0xFFFF );      //enable interrupt on both ports for all pins
  
//  tempwide = mcp_buttons.get2Registers(MCP23017_IOCONA);
//  Serial.println(F("-----MCP Interrupt Setup Follows-----"));
//  Serial.print(F("IOCONAB:  "));
//  Serial.print(tempwide, BIN);
//  Serial.println();
//  Serial.print(F("Interrupt Control AB:    "));
//  tempwide = mcp_buttons.get2Registers(MCP23017_INTCONA);
//  Serial.print(tempwide, BIN);
//  Serial.println();
//  Serial.print(F("Default Values AB:       "));
//  tempwide = mcp_buttons.get2Registers(MCP23017_DEFVALA);
//  Serial.print(tempwide, BIN);
//  Serial.println();
//  Serial.print(F("Interrupt Enabled Pins:  "));
//  tempwide = mcp_buttons.get2Registers(MCP23017_GPINTENA);
//  Serial.print(tempwide, BIN);
//  Serial.println();
//  Serial.println();
}

void setupArduinoINT0() {
  //-------ENABLE ARDUINO INT0 interrupt routines------------------
  //-------do this before we start expecting to see interrupts-----
  //--------INT 0---------
  EICRA = 0;               //clear it
  EICRA |= (1 << ISC01);
  EICRA |= (1 << ISC00);   //ISC0[1:0] = 0b11  rising edge INT0 creates interrupt
  EIMSK |= (1 << INT0);    //enable INT0 interrupt
  // Note:  instead of above, could use Arduino version of same
  // attachInterrupt(0,  functionname , RISING);
}

void setupArduinoPCINT2() {
  PCICR = 0;
  PCICR |= (1 << PCIE2);    //turn on Pin Change Interrupt Enable 2
  PCMSK2 = 0;
  PCMSK2 |= (1 << PCINT22);  //enable interrupt on Port Pin D6 (PCINT22) for encoder
//we won't use the next line because the PCINT2 cannot tell us which pin interrupted.
//easier to just know it was D6 from above line
//  PCMSK2 |= (1 << PCINT23);  //enable interrupt on Port Pin D7 (PCINT23) for encoder

//  uint8_t val = PCICR;
//  Serial.print("PCICR: ");
//  Serial.println(val, BIN);
//  val = PCMSK2; 
//  Serial.print("PCMSK2: ");
//  Serial.println(val, BIN);
//  val = PCMSK1; 
//  Serial.print("PCMSK1: ");
//  Serial.println(val, BIN);
//  val = PCMSK0; 
//  Serial.print("PCMSK0: ");
//  Serial.println(val, BIN);
  
}













//##############################################################################
// #############################################################################
//    MAIN LOOP
//##############################################################################
//##############################################################################
void loop() {
    this_loop_time = millis();
  

    mcp_HDSP2111s.GoDogGo();   //continuous scrolling and updating of info
    
    checkInterrupts();
    handleEncoderInput();   
    
    if(StartingUp) {
        if (this_loop_time - start_time > START_DELAY) {
          StartingUp = false;
        } 
        //simply prevent from entering other updates.
        last_display1update_time = this_loop_time;
        last_display2update_time = this_loop_time;  
    }
   
    if (this_loop_time - last_display1update_time >  125) {    // 300) {
        DIGITAL_CLOCK->update();
        //mcp_HDSP2111s.DEBUG_PrintDisplayData();
        last_display1update_time = this_loop_time;    
    }
    
    
    if (this_loop_time - last_display2update_time > 3000) {
        DIGITAL_CLOCK->updateTemperature();
        last_display2update_time = this_loop_time;        
    }
    
}


void handleEncoderInput( void ) {
      if (encoder0Pos != 0) {
//          Serial.print("New encoder position: ");
//          Serial.print(encoder0Pos, DEC);
//          Serial.print("  >>1: ");
//          Serial.print(encoder0Pos>>1, DEC);
//          Serial.print("  >>3: ");
//          Serial.print(encoder0Pos>>2, DEC);
//          Serial.print("   original: ");
//          Serial.println(encoder0Pos, DEC);
          DIGITAL_CLOCK->handleInputEvent(ENCODER_EVENT, encoder0Pos);
          encoder0Pos = 0;
      }
}      


/**
 * Because we cannot issue I2C commands inside an interrupt vector we
 * need to have an easy functional method that we can call frequently
 * within our code (poll).  This method looks for flags (set by interrupts)
 * and takes the I2C actions.
 */
void checkInterrupts() {
    if(serviceint) {     
      handleINT();
    } 
    else {
      if(digitalRead(INT_PIN)) {
        //somehow our pin is high, but serviceint is false. Clear the int
        valAB = mcp_buttons.readGPIOAB();
      } 
    }
}



/**
 * Do all the 'heavy' lifting resulting from our interrupt.  This function
 * should be ready to be called frequently to aid in responsiveness.  
 * One reason for this method is that i2C calls
 * use interrupts themselves and should really not be called inside
 * the interrupt vector function.
 */
void handleINT() {
    unsigned long temp = int_time - last_int_time;
//    Serial.print("Button press time: ");
//    Serial.print(temp, DEC);
//    Serial.print("     ");

    if(temp > DEBOUNCE_DELAY) {
      btncnt++;
      last_int_time = int_time;
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));      //TOGGLE:  ^ 1);
      
      //i2c calls are impossible inside an interrupt vector, so we save those for here
      //If you call them inside an INT vector, the processor will hang!
      intflagAB = mcp_buttons.getInterruptFlagAB();     
      
      //NOTE: These next 2 will clear the interrupt.  The problem is that if the user
      //has _not_ released the button yet, it will simply re-interrupt again.  We
      //were seeing this every 30-60ms (about the time it takes for the user to
      //depress the button.  The solution is to re-set the delay here.  This way,
      //if the user holds down the button it can continue to interrupt (as they
      //may intend) but at a controlled rate.
      //I found a quick depress to be 30ms and a slow depress to be about 70ms.   
      
//      intcapAB = mcp_buttons.getInterruptCaptureAB();    //this will clear the MCP interrupt
//      valAB = mcp_buttons.readGPIOAB();                  //this will clear the MCP interrupt
      //since we don't need both A and B for our use
      INTCAPA = mcp_buttons.getInterruptCaptureA();
      //mask off high nibble
      INTCAPA = INTCAPA | 0XF0;
      //invert 
      INTCAPA = ~INTCAPA;
                                          
                              
      switch(INTCAPA) {
          case BUTTON_SELECT:
              //mcp_HDSP2111s.setDisplayStringAsNew(Select_String, 2);
              DIGITAL_CLOCK->handleInputEvent(SELECT_EVENT, 0);            
              break;
          case BUTTON_DISPLAY:
              //mcp_HDSP2111s.setDisplayStringAsNew(Display_String, 2);
              DIGITAL_CLOCK->handleInputEvent(DISPLAY_EVENT, 0);
              break;
          case BUTTON_STANDBY:
              //mcp_HDSP2111s.setDisplayStringAsNew(Standby_String, 2);
              DIGITAL_CLOCK->handleInputEvent(STANDBY_EVENT, 0);
              break;
          case BUTTON_SET:
              //mcp_HDSP2111s.setDisplayStringAsNew(Set_String, 2);
              DIGITAL_CLOCK->handleInputEvent(SET_EVENT, 0);
              break;                    
      }        
                                                                               
//      Serial.print("GOT BUTTON PRESS:  ");
//      Serial.print(INTCAPA, DEC);
//      Serial.println();
//      Serial.println();
      
      last_display2update_time = this_loop_time;   //give message a chance to scroll
      int_time = millis();                //reset int time, giving user time to release the button
                                          //before it interrupts again
    } else {
      valAB = mcp_buttons.readGPIOAB();  //this will clear the MCP interrupt
    }
    
    delay(50);       // was DEBOUNCE_DELAY  hack: Allow the user time to depress the button, or the MCP will interrupt again
    serviceint = false;
}


void DEBUG_PrintHDSP1111Strings( void ) {
    Serial.println(F("__HDSP2111 DISPLAY INFO__"));
    char *text = mcp_HDSP2111s.getDisplayString(1);
    Serial.print((uint16_t) &text,  DEC);
    Serial.print(F(":"));
    Serial.println(text);
    
    text = mcp_HDSP2111s.getDisplayString(2);
    Serial.print((uint16_t) &text,  DEC);
    Serial.print(F(":"));
    Serial.println(text);
    Serial.println();
}



void checkRAMandExitIfLow( uint8_t checkpointnum ) {
  int x = freeRam();
  if (x < 128) {
    Serial.print(F("!!! WARNING LOW SRAM !! FREE RAM at checkpoint "));
    Serial.print(checkpointnum, DEC);  
    Serial.print(F(" is: "));
    Serial.println(x, DEC);
    Serial.println();
    gotoEndLoop();
  } else {
    Serial.print(F("FREE RAM, at checkpoint "));
    Serial.print(checkpointnum, DEC);  
    Serial.print(F(" is: "));
    Serial.println(x, DEC);
  }
}


//Endpoint for program (really for debugging)
void gotoEndLoop( void ) {
  Serial.println(F("--->END<---"));
  while (1) {
    delay(100);   
  }
}


/**
 * Takes one byte and loads up bytestr[8] with the value
 *  eg  "8" loads bytestr with "00000111"
 */
void getBinaryString(uint8_t byteval, char bytestr[]) 
{
	uint8_t bitv;
	int i = 0;
	
    for (i = 0; i < 8; i++) {                    
           bitv = (byteval >> i) & 1;
           if (bitv == 0) {
               bytestr[7-i] = '0';
           } else {
               bytestr[7-i] = '1';
           }
    }
}





