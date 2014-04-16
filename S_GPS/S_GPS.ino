/*
 *  Description: This example shows how to use the stand-alone GPS
 *  This example shows the AT commands (and the answers of the module) used
 *  to work with the stand-alone GPS. For more information about the AT commands, 
 *  refer to the AT command manual.
 *
 *  Copyright (C) 2013 Libelium Comunicaciones Distribuidas S.L.
 *  http://www.libelium.com
 *
 *  This program is free software: you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation, either version 3 of the License, or 
 *  (at your option) any later version. 
 *  
 *  This program is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details. 
 *  
 *  You should have received a copy of the GNU General Public License 
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 *
 *  Version 0.2
 *  Author: Alejandro Gallego 
 */

#include <LiquidCrystal.h>

int8_t answer;
int onModulePin= 2;
char gps_data[100];
int counter;

LiquidCrystal lcd(12, 11, 6, 5, 4, 3);

void setup(){

  lcd.begin(16, 2);

  pinMode(onModulePin, OUTPUT);
  Serial.begin(57600);

  Serial.println("Starting...");
  lcd.clear();
  lcd.print("Starting...");
  power_on();

  delay(5000);

  // starts GPS session in stand alone mode
  answer = sendATcommand("AT+CGPS=1,1","OK",1000);    
  if (answer == 0)
  {
    Serial.println("Error starting the GPS");
    Serial.println("The code stucks here!!");
    while(1);
  }
}
void loop(){

  answer = sendATcommand("AT+CGPSINFO","+CGPSINFO:",1000);    // request info from GPS
  if (answer == 1)
  {

    counter = 0;
    do{
      while(Serial.available() == 0);
      gps_data[counter] = Serial.read();
      counter++;
    }
    while(gps_data[counter - 1] != '\r');
    gps_data[counter] = '\0';
    if(gps_data[0] == ',')
    {
      Serial.println("No GPS data available");
      lcd.clear();
      lcd.print("No gps data");
      lcd.setCursor(0,1);
      lcd.print("Available");
    }
    else
    {
      Serial.print("GPS data:");
      Serial.println(gps_data);  
      Serial.println("");
      lcd.clear();
      lcd.print("GPS Data:");
      lcd.setCursor(0,1);
      lcd.print(gps_data);
    }       

  }
  else
  {
    Serial.println("Error"); 
  }

  delay(5000);
  lcd.clear();
}

void power_on(){

  uint8_t answer=0;

  // checks if the module is started
  answer = sendATcommand("AT", "OK", 2000);
  if (answer == 0)
  {
    // power on pulse
    digitalWrite(onModulePin,HIGH);
    delay(3000);
    digitalWrite(onModulePin,LOW);

    // waits for an answer from the module
    while(answer == 0){    
      // Send AT every two seconds and wait for the answer
      answer = sendATcommand("AT", "OK", 2000);    
    }
  }

}


int8_t sendATcommand(char* ATcommand, char* expected_answer1, unsigned int timeout)
{

  uint8_t x=0,  answer=0;
  char response[100];
  unsigned long previous;

  memset(response, '\0', 100);    // Initialize the string

  delay(100);

  while( Serial.available() > 0) Serial.read();    // Clean the input buffer

  Serial.println(ATcommand);    // Send the AT command 


    x = 0;
  previous = millis();

  // this loop waits for the answer
  do{

    if(Serial.available() != 0){    
      response[x] = Serial.read();
      x++;
      // check if the desired answer is in the response of the module
      if (strstr(response, expected_answer1) != NULL)    
      {
        answer = 1;
      }
    }
    // Waits for the asnwer with time out
  }
  while((answer == 0) && ((millis() - previous) < timeout));    

  return answer;
}


