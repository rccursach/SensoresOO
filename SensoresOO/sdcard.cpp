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

Sdcard::Sdcard(int sdPin) {
	sdPin = 8;
}

Sdcard::~Sdcard() {
}

void begin(){
sd.begin(sdPin);
}

boolean startSDCard() {
  boolean result = false; 
  begin();
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  //pinMode(10, OUTPUT);

  // see if the card is present and can be initialized:
  if (!sd.begin(sdPin)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    result = false;
  } 
  else {
    Serial.println("card initialized.");
    File dataFile = sd.open(fileName, FILE_WRITE);
    // open the file:
  }  
  return result;
}

void Sdcard::writeLine() {
	;
}

String Sdcard::getPackage(String) {
	;
}

void Sdcard::dismissPackage() {
	;
}