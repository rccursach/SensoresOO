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

SensorTest st;  //Test Sensors derived object
SharpDust sh(9, 1);

void setup(){
  st.begin();
  sh.begin();
  Serial.begin(9600);
}

void loop(){
  Serial.println( st.read() );
  Serial.println( sh.read() );
  delay(3000);
}
