/*
* SharpDust.cpp
* Adds Sharp GP2Y1010AU0F Particle Sensor methods
* and Implements the virtual methods of the Sensors interface.
* 
* (C) Matías Barrera García, Ricardo Carrasco Cursach, 2014
*
* last modified: 02-04-14
* by: matybg and rccursach
* 
*/

#include "SharpDust.h"

SharpDust::SharpDust(int led, int analog) {
	ledPower = led;  // power on the LED
	dustPin = analog; // read the dust value
  	int delayTime=280;
  	int delayTime2=40;
  	float offTime=9680;
}

SharpDust::~SharpDust() {
}

void SharpDust::begin() {
	pinMode(ledPower,OUTPUT);
}

String SharpDust::read() {

	int dustVal=0;
	float voltage = 0;
	float dustdensity = 0;
	float ppmpercf = 0;

	digitalWrite(ledPower,LOW); // power on the LED
	delayMicroseconds(delayTime);
	dustVal=analogRead(dustPin); // read the dust value
	delayMicroseconds(delayTime2);
	digitalWrite(ledPower,HIGH); // turn the LED off
	delayMicroseconds(offTime);

	voltage = dustVal*0.0049;
	dustdensity = 0.17*voltage-0.1;
	ppmpercf = (voltage-0.0256)*120000;
	if (ppmpercf < 0)
		ppmpercf = 0;
	if (dustdensity < 0 )
		dustdensity = 0;
	if (dustdensity > 0.5)
		dustdensity = 0.5;

	String dataString = "";
	dataString += dtostrf(voltage, 9, 4, s);
	dataString += ",";
	dataString += dtostrf(dustdensity, 5, 2, s);
	dataString += ",";
	dataString += dtostrf(ppmpercf, 8, 0, s);

  return dataString;
}

int SharpDust::getRequiredPins() {
	return 0;
}

int SharpDust::getReqSerialSpeed() {
	return 0;
}