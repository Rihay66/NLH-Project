#include<iostream>
#include <windows.h>
#include <stdio.h>
#include <string>
#include"SerialPort.h"

using namespace std;

char output[MAX_DATA_LENGTH];
char incomingData[MAX_DATA_LENGTH];

const char* port;

int main(void) {
	//Give the program time to open completely
	Sleep(2000);

	bool board_detected = false;
	HANDLE hSerial = 0;
	SerialPort arduino;
	arduino.reCycle = false;
	arduino.selectedPort = 0;
	for (int i = 0; (i < maxports) && (board_detected != true); i++) 
	{
		port = arduino.Gen_Port_Name();

		hSerial = arduino.Init_Serial(port);

		if (hSerial != INVALID_HANDLE_VALUE && arduino.reCycle == false) {
			board_detected = arduino.Wait_Ready(hSerial);
		}
		else {
			Sleep(1500); // Used for debug
			continue; //Do a check of next port
		}	
	}
	
	if (board_detected == true)
		cout << "\nBoard Found on Port: " << port << "\n" << endl;
	else
		cout << "\nBoard could not be found!!!\n\n" << endl;
	
	//Tells if the boolean system works
	if (arduino.isConnected()) {
		printf("Arduino is connected!\n\n");	
		printf("MESSAGE: Use 'sysexit' to exit program!\n\n");
	}
	else {
		printf("ERROR: Arduino is not connected or something went wrong\n\n"); //Might be used to resolve issues with connectivity
	}

	//Similar to wait for milliseconds in c#
	Sleep(2500); //debugging

	//[] Make a function that detects if the connected device is a arduino, which the arduino will contain a char that determines the connection
	//Convert this these lines to a seperate function
	while (arduino.isConnected()) {
		cout << "Enter your command: ";
		string data;
		cin >> data;

		//If this command is entered the program will exit
		if (data.compare("sysexit") == 0)
		{
			cout << "Exiting program..." << endl;
			return 0;
		}

		char* charArray = new char[data.size() + 1];
		copy(data.begin(), data.end(), charArray);
		charArray[data.size()] = '\n';

		arduino.writeSerialPort(charArray, MAX_DATA_LENGTH);
		arduino.readSerialPort(output, MAX_DATA_LENGTH);

		cout << "Write >> " << charArray << "\n" << endl;
		if ((output != NULL) && (output[0] == '\0')) {
			cout << "MESSAGE: No read received from device" << "\n" << endl;
		}
		else {
			cout << "Read >> " << output << "\n" << endl;
		}

		delete[] charArray;
	}

	return 0;
}