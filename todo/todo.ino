// This is the top of the program
#include <Wire.h>
#include <dht.h>
#include <SdFat.h>
#include <SPI.h>
#include <stdlib.h>

#define BMP085_ADDRESS 0x77  // I2C address of BMP085
#define DHT22_PIN 6  // Pin del sensor DHT-22

dht DHT; // Objeto del sensor de humedad DHT-22 ~ RHT03

//Objetos para manejar la sd
SdFat sd;
SdFile logFile;
const int sdChipSelect = 8;
const char* logFileName = "mylog.txt";

//GPS
int8_t answer;
int onModulePin= 2;
char gps_data[100];
int counter;

//Sharp_Serial
long lastConnectionTime = 0;        // last time you connected to the server, in milliseconds
boolean lastConnected = false;      // state of the connection last time through the main loop
int dustPin=0;
int ledPower=2;
int delayTime=280;
int delayTime2=40;
float offTime=9680;

const int postingInterval = 10000;  //delay between updates to Pachube.com
int dustVal=0;
int i=0;
float ppm=0;
char s[32];
float voltage = 0;
float dustdensity = 0;
float ppmpercf = 0;

const unsigned char OSS = 0;  // Oversampling Setting

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
const float p0 = 101325;     // Pressure at sea level (Pa)
float altitude;

void setup()
{
  Serial.begin(115200);
  //GSP
  pinMode(onModulePin, OUTPUT);
  Serial.println("Starting...");
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
  //Pressure-sensor
  Wire.begin();
  bmp085Calibration();
  //Sharp-Sensor
  pinMode(ledPower,OUTPUT);
  i=0;
  ppm =0;
  //inicializar SD
  if (!sd.begin(sdChipSelect, SPI_HALF_SPEED)) sd.initErrorHalt();
  if (!logFile.open(logFileName, O_RDWR | O_CREAT | O_AT_END)) {
    sd.errorHalt("opening test.txt for write failed");
  }
  else{
    logFile.close();
  }
}

// Stores all of the bmp085's calibration values into global variables
// Calibration values are required to calculate temp and pressure
// This function should be called at the beginning of the program
void bmp085Calibration()
{
  ac1 = bmp085ReadInt(0xAA);
  ac2 = bmp085ReadInt(0xAC);
  ac3 = bmp085ReadInt(0xAE);
  ac4 = bmp085ReadInt(0xB0);
  ac5 = bmp085ReadInt(0xB2);
  ac6 = bmp085ReadInt(0xB4);
  b1 = bmp085ReadInt(0xB6);
  b2 = bmp085ReadInt(0xB8);
  mb = bmp085ReadInt(0xBA);
  mc = bmp085ReadInt(0xBC);
  md = bmp085ReadInt(0xBE);
}


#define BMP085_ADDRESS 0x77  // I2C address of BMP085

// Read 1 byte from the BMP085 at 'address'
char bmp085Read(unsigned char address)
{
  unsigned char data;
  
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  
  Wire.requestFrom(BMP085_ADDRESS, 1);
  while(!Wire.available())
    ;
    
  return Wire.read();
}

// Read 2 bytes from the BMP085
// First byte will be from 'address'
// Second byte will be from 'address'+1
int bmp085ReadInt(unsigned char address)
{
  unsigned char msb, lsb;
  
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  
  Wire.requestFrom(BMP085_ADDRESS, 2);
  while(Wire.available()<2)
    ;
  msb = Wire.read();
  lsb = Wire.read();
  
  return (int) msb<<8 | lsb;
}

// Read the uncompensated temperature value
unsigned int bmp085ReadUT()
{
  unsigned int ut;
  
  // Write 0x2E into Register 0xF4
  // This requests a temperature reading
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x2E);
  Wire.endTransmission();
  
  // Wait at least 4.5ms
  delay(5);
  
  // Read two bytes from registers 0xF6 and 0xF7
  ut = bmp085ReadInt(0xF6);
  return ut;
}

// Read the uncompensated pressure value
unsigned long bmp085ReadUP()
{
  unsigned char msb, lsb, xlsb;
  unsigned long up = 0;
  
  // Write 0x34+(OSS<<6) into register 0xF4
  // Request a pressure reading w/ oversampling setting
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x34 + (OSS<<6));
  Wire.endTransmission();
  
  // Wait for conversion, delay time dependent on OSS
  delay(2 + (3<<OSS));
  
  // Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF6);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDRESS, 3);
  
  // Wait for data to become available
  while(Wire.available() < 3)
    ;
  msb = Wire.read();
  lsb = Wire.read();
  xlsb = Wire.read();
  
  up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS);
  
  return up;
}

// Calculate temperature given ut.
// Value returned will be in units of 0.1 deg C
short bmp085GetTemperature(unsigned int ut)
{
  long x1, x2;
  
  x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
  x2 = ((long)mc << 11)/(x1 + md);
  b5 = x1 + x2;

  return ((b5 + 8)>>4);  
}

// Calculate pressure given up
// calibration values must be known
// b5 is also required so bmp085GetTemperature(...) must be called first.
// Value returned will be pressure in units of Pa.
long bmp085GetPressure(unsigned long up)
{
  long x1, x2, x3, b3, b6, p;
  unsigned long b4, b7;
  
  b6 = b5 - 4000;
  // Calculate B3
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;
  
  // Calculate B4
  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;
  
  b7 = ((unsigned long)(up - b3) * (50000>>OSS));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
    p = (b7/b4)<<1;
    
  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p += (x1 + x2 + 3791)>>4;
  
  return p;
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


void loop()
{
  //GPS
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
    }
    else
    {
      Serial.print("GPS data:");
      Serial.println(gps_data);  
      Serial.println("");
    }       

  }
  else
  {
    Serial.println("Error"); 
  }

  delay(5000);

  //Sharp-Sensor
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

  //Pressure Sensor
  temperature = bmp085GetTemperature(bmp085ReadUT());
  pressure = bmp085GetPressure(bmp085ReadUP());
  
  int temp = (int)temperature * 0.1;
  //float pres = pressure / 100;
  float pres = (float)pressure / 100.0;
  
  int chk = DHT.read22(DHT22_PIN);
   
  Serial.print("Temperatura;");
  Serial.print(temp, DEC);
  Serial.println(";C");
  Serial.print("Presion;");
  Serial.print(pres, 3);
  Serial.println(";hPa");
  Serial.print("Humedad;");
  Serial.print(DHT.humidity, 1);
  Serial.println(";%");
  Serial.print("Temperatura 2;");
  Serial.print(DHT.temperature, 1);
  Serial.println(";C");  
  
  if(logFile.open(logFileName, O_RDWR | O_CREAT | O_AT_END)){
    Serial.println("File opened, writing");
    logFile.print("Temperatura;");
    logFile.print(temp, DEC);
    logFile.println(";C");
    logFile.print("Presion;");
    logFile.print(pres, 3);
    logFile.println(";hPa");
    logFile.print("Humedad;");
    logFile.print(DHT.humidity, 1);
    logFile.println(";%");
    logFile.print("Temperatura 2;");
    logFile.print(DHT.temperature, 1);
    logFile.println(";C");
    logFile.close();
    Serial.println("File closed");
  }
  
  delay(1000);
}
