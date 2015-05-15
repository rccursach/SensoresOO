/*
* SensorOO.ino
* Object Oriented data adquiring implementation
* for usage with arduino sensors.
* 
* (C) Ricardo Carrasco Cursach 2014
*
* last modified: 15-05-15
* by: rccursach
* 
*/

#include "SensorTest.h" //Test class implementing the Sensors interface.
#include "SharpDust.h"


SensorTest st;  //Test Sensors derived object
SharpDust sh(9, 1);



void setup(){
  st.begin();  //Sensor test
  sh.begin();  //Sensor Sharp 
  Serial1.begin(9600);
}

void loop(){
  Serial1.println( "{ test : "+ st.read() + ", " + "sharp : " + sh.read() + "}");
  delay(1000);
}
