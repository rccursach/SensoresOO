/*
* SensorOO.ino
* Object Oriented data adquiring implementation
* for usage with arduino sensors.
* 
* (C) Ricardo Carrasco Cursach 2014
*
* last modified: 16-03-14
* by: rccursach
* 
*/

/* TODO:
 *
 * * Add pins usage control (On instance creation check if a pin is used by another Sensor derived object).
 * * Add pins requirements indication/exception (E: needs PWM capable pin) on Sensors interface.
 */

#include "SensorTest.h" //Test class implementing the Sensors interface.
#include "SharpDust.h"
#include "GpsSIM5218.h"
#include <SoftwareSerial.h>

//xBee using SoftSerial
SoftwareSerial mySerial(2, 3); // rx, tx

//SensorTest st;  //Test Sensors derived object
SharpDust sh(9, 1);
GpsSIM5218 gps(5, 115200);

bool gps_ok;

void setup(){
  //st.begin();
  sh.begin();
  mySerial.begin(9600);
  mySerial.println("Starting gps");
  gps_ok = gps.begin();
  if(gps_ok){ mySerial.println("gps ok"); }
  else{ mySerial.println("gps error"); }
}

void loop(){
  //mySerial.println( st.read() );
  mySerial.println( sh.read() );

  if (gps_ok){
  	char* s = gps.read();
  	if(strcmp("", s)!=0 )
  		mySerial.println(s);
	}
  delay(3000);
}
