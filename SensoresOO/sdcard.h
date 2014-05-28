/*
* sdcard.h
* This is a class definition for the sdcard.
* The list of virtual methods are used to get sensors data packages, 
* write them and dismiss sent packages managed in the SD card.
* 
* (C) Matías Barrera García, Ricardo Carrasco Cursach, 2014
*
* last modified: 16-04-14
* by: matybg and rccursach
* 
*/

#ifndef   SDCARD_H
#define   SDCARD_H

#include <Arduino.h>
#include <SdFat.h>

class Sdcard{
public:
	void begin();
	boolean startSDCard()
  void writeLine();
  String getPackage(String);
  void dismissPackage();
  ~Sdcard();
private:
	SdFat sd;
  SdFile dataFile;
	int sdPin;
  char* dataFileName;
};

#endif