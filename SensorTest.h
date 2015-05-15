/*
* SensorTest.cpp
* Definition of a test class wich implements the Sensors interface
* and also suplies its own method ( setSpeed(int) ).
* 
* (C) Ricardo Carrasco Cursach 2014
*
* last modified: 16-03-14
* by: rccursach
* 
*/

#include "sensors.h"

class SensorTest : public Sensors {
  public:
  	//by contract with Sensors interface
    virtual void begin();
    virtual String read();
    virtual int getRequiredPins();
    virtual int getReqSerialSpeed();
  	//own methods
    SensorTest();
    ~SensorTest();
    void setSpeed(int);
  private:
    int speed;
};
