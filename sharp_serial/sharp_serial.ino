/*
 Interface to Sharp GP2Y1010AU0F Particle Sensor
 Program by Christopher Nafis 
 Written April 2012
 
 http://www.sparkfun.com/datasheets/Sensors/gp2y1010au_e.pdf
 http://sensorapp.net/?p=479
 
 Sharp pin 1 (V-LED)   => 5V (connected to 150ohm resister)
 Sharp pin 2 (LED-GND) => Arduino GND pin
 Sharp pin 3 (LED)     => Arduino pin 2
 Sharp pin 4 (S-GND)   => Arduino GND pin
 Sharp pin 5 (Vo)      => Arduino A0 pin
 Sharp pin 6 (Vcc)     => 5V
 */
 
#include <SPI.h>
#include <stdlib.h>

long lastConnectionTime = 0;        // last time you connected to the server, in milliseconds
boolean lastConnected = false;      // state of the connection last time through the main loop
const int postingInterval = 10000;  //delay between updates to Pachube.com

int dustPin=0;
int ledPower=2;
int delayTime=280;
int delayTime2=40;
float offTime=9680;

int dustVal=0;
int i=0;
float ppm=0;
char s[32];
float voltage = 0;
float dustdensity = 0;
float ppmpercf = 0;

void setup(){
  Serial.begin(9600);
  pinMode(ledPower,OUTPUT);

  i=0;
  ppm =0;
}

void loop(){
  i=i+1;
  digitalWrite(ledPower,LOW); // power on the LED
  delayMicroseconds(delayTime);
  dustVal=analogRead(dustPin); // read the dust value
  ppm = ppm+dustVal;
  delayMicroseconds(delayTime2);
  digitalWrite(ledPower,HIGH); // turn the LED off
  delayMicroseconds(offTime);

  // if ten seconds have passed since
  // your last send, then send data again:
  if((millis() - lastConnectionTime > postingInterval)) {
    voltage = ppm/i*0.0049;
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
    i=0;
    ppm=0;
    Serial.println(dataString);
    lastConnectionTime = millis();
    delay(1000);
  }
}
