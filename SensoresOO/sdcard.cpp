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

#include "sdcard.h"
#include <SdFat.h>


Sdcard::Sdcard(int sdPin, char* fileName) {
	this->sdPin = 8;
  this->dataFileName = fileName;
  this->sd = SdFat();
  this->dataFile = SdFile();
}

Sdcard::~Sdcard() {
  delete[] dataFileName;
}

void Sdcard::begin() {
  //sd.begin(sdPin);
  Serial.println("Use: sd.begin(sdPin), as in startSDCard()");
}

boolean Sdcard::startSDCard() {
  boolean result = true; 
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(this->sdPin, OUTPUT);

  // see if the card is present and can be initialized:
  if (!sd.begin(this->sdPin, SPI_HALF_SPEED)) {
    this->sd.initErrorHalt();
    Serial.println("Card failed, or not present.");
    result = false;
  } 
  else {
    if(!this->dataFile.open(this->dataFileName, O_RDWR | O_CREAT | O_AT_END)){
      sd.initErrorHalt();
      Serial.println("File cannot be opened.");
      result = false;
    }
    Serial.println("card initialized.");
  }  
  return result;
}

void Sdcard::writeLine() {
	//;
}

String Sdcard::getPackage(String) {
	return "";
}

void Sdcard::dismissPackage() {
	//;
}
