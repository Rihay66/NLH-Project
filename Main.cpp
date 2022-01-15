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
	bool board_detected = false;
	HANDLE hSerial = 0;
	SerialPort arduino;
	for (int i = 0; (i < maxports) && (board_detected != true); i++) 
	{
		port = arduino.Gen_Port_Name();

		hSerial = arduino.Init_Serial(port);

		if (hSerial != INVALID_HANDLE_VALUE)
			board_detected = arduino.Wait_Ready(hSerial);
		else
			continue;
	}
	
	if (board_detected == true)
		cout << "\nBoard Found on Port: %s\n\n" << port << endl;
	else
		cout << "\nBoard could not be found!!!\n\n" << endl;
	

	Sleep(10000); //debugging


	return 0;

	//[] Make input to throw at the arduino

	/*
	CheckPort check;
	while (!isFound) 
	{
		//checks for every available port and might cycle through them until the correct port is found
	    //Takes returned port char	
		check.checkDevice();
		if (!check.checkDevice()) {
			port = check.portSelected;
			SerialPort arduino(port);
			if (arduino.isConnected()) {
				isFound = false;
				cout << "Connection made" << endl << endl;
			}
			else {
				cout << "Port not found" << endl << endl;
				isFound = true; //Cancel this whole function
			}
			while (arduino.isConnected() && isFound == true) {
				cout << "Enter your command: " << endl;
				string data;
				cin >> data;

				char* charArray = new char[data.size() + 1];
				copy(data.begin(), data.end(), charArray);
				charArray[data.size()] = '\n';

				arduino.writeSerialPort(charArray, MAX_DATA_LENGTH);
				arduino.readSerialPort(output, MAX_DATA_LENGTH);

				cout << ">> " << output << endl;

				delete[] charArray;
			}
			return 0;
		}
	}
	*/
}