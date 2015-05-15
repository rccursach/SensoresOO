/*
* SharpDust.h
* Definition of a class to manage Sharp GP2Y1010AU0F Particle Sensor,
* based on Christopher Nafis program.
* Also implements the common Sensors interface.
* 
* http://www.sparkfun.com/datasheets/Sensors/gp2y1010au_e.pdf
* http://sensorapp.net/?p=479
* 
* Sharp pin 1 (V-LED)   => 5V (connected to 150ohm resistor)
* Sharp pin 2 (LED-GND) => Arduino GND pin
* Sharp pin 3 (LED)     => Arduino pin 2
* Sharp pin 4 (S-GND)   => Arduino GND pin
* Sharp pin 5 (Vo)      => Arduino A0 pin
* Sharp pin 6 (Vcc)     => 5V
*
* (C) Matías Barrera García, Ricardo Carrasco Cursach, 2014
*
* last modified: 02-04-14
* by: matybg and rccursach
* 
*/

#ifndef SHARP_H
#define SHARP_H

//Include Sensors Interface
#include "sensors.h"

#include <stdlib.h>

class SharpDust : public Sensors {
public:
    //by contract with Sensors interface
  virtual void begin();
  virtual String read();
  virtual int getRequiredPins();
  virtual int getReqSerialSpeed();
    //own methods
  SharpDust(int, int);
  ~SharpDust();
private:

  int dustPin;
  int ledPower;
  int delayTime;
  int delayTime2;
  float offTime;
  char s[32];

};

#endif