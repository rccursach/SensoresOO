/*
* sdcard.h
* This is a class definition for the sdcard.
* The list of virtual methods are used to get sensors data packages, 
* write them and dismiss sent packages managed in the SD card.
* 
* (C) Matías Barrera García, Ricardo Carrasco Cursach, 2014
*
* last modified: 23-06-14
* by: matybg and rccursach
* 
*/

#ifndef   SDCARD_H
#define   SDCARD_H

#include <Arduino.h>
#include <SdFat.h>

class Sdcard{
public:
  Sdcard(int sdPin, char* fileName);
	void begin();              //included by compatibility ()
	boolean startSDCard();
  void writeLine();           //writes a string describing a package to file
  String getPackage(String);  //get the first non-dismissed package string on file
  void dismissPackage();      //mark first non-dismissed package as dismissed
  ~Sdcard();
private:
	SdFat sd;
  SdFile dataFile;
	int sdPin;                  //control pin used by the shield
  char* dataFileName;         //filename on sd card
};

#endif
