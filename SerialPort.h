
#define ARDUINO_WAIT_TIME 2000
#define MAX_DATA_LENGTH 255

#define PORT_SIZE 5
#define maxports 13

#include <windows.h>
#include <stdio.h>
#include <iostream>

using namespace std;

class SerialPort
{
private:
	HANDLE handler;
	COMSTAT status;
	bool connected;
	DWORD errors;
public:
	int serial_params(HANDLE hSerial);
	HANDLE Init_Serial(const char* portname);
	const char* Gen_Port_Name(void);
	int Wait_Ready(HANDLE hSerial);

	int readSerialPort(char* buffer, unsigned int buf_size);
	bool writeSerialPort(char* buffer, unsigned int buf_size, HANDLE hSerial);
	bool isConnected();
	int selectedPort;
	bool reCycle;
};