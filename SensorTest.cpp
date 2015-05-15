/*
* SensorTest.cpp
* Implements the virtual methods of the Sensors interface
* and add one more wich is specific to this realization of Sensors.
* 
* (C) Ricardo Carrasco Cursach 2014
*
* last modified: 16-03-14
* by: rccursach
* 
*/

#include "SensorTest.h"

SensorTest::SensorTest() {
  speed = 1000;
}

SensorTest::~SensorTest() {
}

void SensorTest::setSpeed(int speed = 1000) {
}

void SensorTest::begin() {
	delay(1000);
}

String SensorTest::read() {
  delay(speed);
  return "Some value";
}

int SensorTest::getRequiredPins() {
	return 0;
}

int SensorTest::getReqSerialSpeed() {
  return 0;
}
