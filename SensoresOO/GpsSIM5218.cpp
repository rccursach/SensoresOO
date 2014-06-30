#include "GpsSIM5218.h"

GpsSIM5218::GpsSIM5218(int on_pin, int uart_baud){
	this->onModulePin = on_pin;
	this->uart_baud_speed = uart_baud;
	this->counter = 0;
	this->answer = 0;
	this->gps_data = new char[100];
}

GpsSIM5218::~GpsSIM5218(){
	delete[] this->gps_data;
}

bool GpsSIM5218::begin(){

    pinMode(this->onModulePin, OUTPUT);
    Serial.begin(115200);

    power_on();

    delay(5000);
    
    // starts GPS session in stand alone mode
    this->answer = sendATcommand("AT+CGPS=1,1","OK",1000);    
    if (this->answer == 0)
    {
    	return false;
    }
    return true;
}

char* GpsSIM5218::read(){
   
    this->answer = sendATcommand("AT+CGPSINFO","+CGPSINFO:",1000);    // request info from GPS
    if (this->answer == 1)
    {

        this->counter = 0;
        do{
            while(Serial.available() == 0);
            this->gps_data[this->counter] = Serial.read();
            this->counter++;
        }
        while(this->gps_data[this->counter - 1] != '\r');
        this->gps_data[this->counter] = '\0';
        if(this->gps_data[0] == ',')
        {
             return "";
        }
        else
        {
            return this->gps_data;
        }       

    }
    else
    {
        return "";
    }

    delay(5000);
}

void GpsSIM5218::power_on(){

    this->answer=0;

    // checks if the module is started
    this->answer = sendATcommand("AT", "OK", 2000);
    if (this->answer == 0)
    {
        // power on pulse
        digitalWrite(this->onModulePin,HIGH);
        delay(3000);
        digitalWrite(this->onModulePin,LOW);

        // waits for an answer from the module
        while(this->answer == 0){    
            // Send AT every two seconds and wait for the answer
            this->answer = sendATcommand("AT", "OK", 2000);
        }
    }

}


int8_t GpsSIM5218::sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout)
{

    uint8_t x=0;
    this->answer=0;
    
    char response[100];
    unsigned long previous;

    memset(response, '\0', 100);    // Initialize the string

    delay(100);

    while( Serial.available() > 0) Serial.read();    // Clean the input buffer

    Serial.println(ATcommand);    // Send the AT command 


        x = 0;
    previous = millis();

    // this->loop waits for the answer
    do{

        if(Serial.available() != 0){    
            response[x] = Serial.read();
            x++;
            // check if the desired answer is in the response of the module
            if (strstr(response, expected_answer) != NULL)    
            {
                this->answer = 1;
            }
        }
        // Waits for the asnwer with time out
    }
    while((this->answer == 0) && ((millis() - previous) < timeout));    

    return this->answer;
}