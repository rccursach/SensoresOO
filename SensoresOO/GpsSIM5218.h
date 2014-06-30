//#include <stdlib.h>
#include <Arduino.h>

class GpsSIM5218 {
public:
	GpsSIM5218(int on_pin, int uart_baud = 115200);
	~GpsSIM5218();
	bool begin();
	char* read();
private:
	int8_t sendATcommand(char* ATcommand, char* expected_answer1, unsigned int timeout);
	void power_on();
	//
	uint8_t answer;
	int onModulePin;
	int uart_baud_speed;
	char* gps_data;
	int counter;
};