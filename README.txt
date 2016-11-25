mizraith_DigitalClock package

THIS VERSION IS COMPATIBLE WITH ARDUINO 1.06   
Arduino version after 1.56 change the gcc compiler.  This version will not compile under 1.56+.  A new version is in the works.


The Digital Clock package is a lot more complicated than I originally imagined it would be.  In the end, a table-driven state machine (stored in PROGMEM to save precious RAM) handles a variety of user inputs, including those from an encoder.  

The advantages of the table driven state machine are ease of expansion and adjustment to the event handling.  Some DEBUG and TEST functions are provided to verify underlying code functionality (as well as stress-test the state machine itself).  

An interface layer is also provided for RTCs, so it should be fairly simple to use the RTC device of your chosing, provided it can handle the RTC_Interface calls defined.

Other areas of note:
	A table-driven Celsius to Fahrenheit conversion, so as to avoid costly or expensive floating point arithmetic.
	Easter Egg functionality — let your Digital Clock load an easter egg string every now and then.  Fairly simple to add new eggs.
	Extensive use of PROGMEM should provide you with rich examples of how to store complicated data types and get them in and out.  Saves a TON of SRAM.
        

Apologies to more dedicated C++ professionals.  As this was my first more expansive package, I struggled a bit with namespaces and classes.  The breakdown is not as clean as it should be, but enough is enough already!

USE OF FILES 
	- this directory should go into your Arduino libraries folder and then you need to add the Library from within the Arduino application.
	- Suppporting Files DigitalClock_UseExample.ino (move to your Arduino directory), study and adjust to suit your needs, sinice it is unlikely that you have the full hardware layer.
	- 

Enjoy.
_Red