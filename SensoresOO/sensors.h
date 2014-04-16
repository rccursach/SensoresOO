/*
* sensors.h
* This is a class definition for the Sensors interface.
* The list of virtual methods are common to all sensors
* to perform basic tasks.
* 
* (C) Ricardo Carrasco Cursach 2014
*
* last modified: 16-03-14
* by: rccursach
* 
*/

#ifndef   SENSORS_H
#define   SENSORS_H

#include <Arduino.h>

class Sensors{
  public:
    virtual void begin() = 0;
    virtual String read() = 0;
    virtual int getRequiredPins() = 0;
    virtual int getReqSerialSpeed() = 0;
    virtual ~Sensors() = 0;
};

#endif