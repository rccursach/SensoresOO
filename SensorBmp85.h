/*
* SensorBmp85.h
* Definition of a test class wich implements the Sensors interface
* and also suplies its own method ( setSpeed(int) ).
* 
* (C) Ricardo Carrasco Cursach 2014
*
* last modified: 16-03-14
* by: rccursach
* 
*/


#ifndef BMP085_H
#define BMP085_H

//Include Sensors Interface
#include "sensors.h"

#define BMP085_ADDRESS 0x77  // I2C address of BMP085

class SensorBmp85 : public Sensors {
  public:
    //by contract with Sensors interface
    virtual void begin();
    virtual String read();
    virtual int getRequiredPins();
    virtual int getReqSerialSpeed();
    //own methods
    SensorBmp85();
    ~SensorBmp85();
  private:
    void bmp085Calibration();
    char bmp085Read(unsigned char address);
    int bmp085ReadInt(unsigned char address);
    unsigned int bmp085ReadUT();
    unsigned long bmp085ReadUP();
    short bmp085GetTemperature(unsigned int ut);
    long bmp085GetPressure(unsigned long up);

    unsigned char OSS;  // Oversampling Setting

    // Calibration values
    int ac1;
    int ac2; 
    int ac3; 
    unsigned int ac4;
    unsigned int ac5;
    unsigned int ac6;
    int b1; 
    int b2;
    int mb;
    int mc;
    int md;
    // b5 is calculated in bmp085GetTemperature(...), this variable is also used in bmp085GetPressure(...)
    // so ...Temperature(...) must be called before ...Pressure(...).
    long b5; 
    short temperature;
    long pressure;
    // Use these for altitude conversions
    float p0;     // Pressure at sea level (Pa)
    float altitude;
};

#endif
