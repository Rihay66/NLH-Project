#include<iostream>
#include <windows.h>
#include <stdio.h>
#include <string>
#include"SerialPort.h"

using namespace std;

char output[MAX_DATA_LENGTH];
char incomingData[MAX_DATA_LENGTH];

const char* port;

class portCheck {
public:
	bool connection;
	bool CheckPort(bool board_detected, SerialPort arduino, HANDLE hSerial, int selectedPort);
	//Returnable value
	//float loopTime();
};

int main(void) {
	//Give the program time to open completely;
	portCheck check;
	bool board_detected = false;
	HANDLE hSerial = 0;
	SerialPort arduino;
	arduino.reCycle = false;
	arduino.selectedPort = 0;
	check.connection = false;
	check.CheckPort(board_detected, arduino, hSerial, arduino.selectedPort);

	//[] Make a function that detects if the connected device is a arduino, which the arduino will contain a char that determines the connection
	while (check.connection) {
		//hSerial = arduino.Init_Serial(currentPort);
		Sleep(1000); //debugging
		cout << "Enter your command: ";
		string data;
		cin >> data;

		//If this command is entered the program will exit
		if (data.compare("sysexit") == 0)
		{
			cout << "\nExiting program...\n" << endl;
			return 0;
		}
		if (data.compare("syschangeport") == 0) //Change to next port
		{
			board_detected = false;
			cout << "\nChanging Port...\n" << endl;
			Sleep(1000);
			arduino.selectedPort++;
			check.CheckPort(board_detected, arduino, hSerial, arduino.selectedPort);
		}
		//Transform string to char bytes
		char* charArray = new char[data.size() + 1];
		copy(data.begin(), data.end(), charArray);
		charArray[data.size()] = '\n';

		//Write command into bytes and sent to arduino
		arduino.writeSerialPort(charArray, MAX_DATA_LENGTH, hSerial, port);

		//This function might need a rewrite
		arduino.readSerialPort(output, MAX_DATA_LENGTH);

		//cout << "Write >> " << charArray << "\n" << endl;
		if ((output == NULL) && (output[0] == '\0') && check.connection == true) {
			cout << "MESSAGE: No read received from device" << "\n" << endl;
		}
		else if(check.connection == true && (output != NULL)) {
			cout << "Read >> " << output << "\n" << endl;
		}

		delete[] charArray;
	}

	if (arduino.selectedPort > maxports) {
		printf("\nERROR: No Board is connected or program didn't check for more ports"); // No more ports to try
		return 0;
	}
}
/*
//value must return a value that will be pasted into Sleep()
float portCheck::loopTime() {

}
*/

bool portCheck::CheckPort(bool board_detected, SerialPort arduino, HANDLE hSerial, int selectedPort) {
	for (int i = selectedPort; (i < maxports) && (board_detected != true); i++)
	{
		if (selectedPort >= maxports) {
			printf("\nERROR: No Board is connected on all available ports or program didn't check for more ports"); // No more ports to try
			return 0;
		}
		else {
			port = arduino.Gen_Port_Name();

			hSerial = arduino.Init_Serial(port);
			if (hSerial != INVALID_HANDLE_VALUE && arduino.reCycle == false) {
				printf("Getting bytes?");
				board_detected = arduino.Wait_Ready(hSerial);
			}
			else {
				Sleep(1500); // Used for debug
				continue; //Do a check of next port
			}
		}
	}

	if (selectedPort >= maxports) {
		printf("\nERROR: No Board is connected on all available ports or program didn't check for more ports"); // No more ports to try
		return 0;
	}

	connection = board_detected;

	if (board_detected == true && arduino.isConnected()) {
		printf("Arduino is connected!\n");
		printf("MESSAGE: Use 'sysexit' to exit program or use 'syschangeport' to change port!\n");
		cout << "\nBoard Found on Port: " << port << "\n" << endl;
		return (connection);
	}
	else {
		printf("\nERROR: No Board is connected on all available ports or program didn't check for more ports\n"); // No more ports to try
		//printf("ERROR: Arduino is not connected or something went wrong\n\n"); //Might be used to resolve issues with connectivity
		cout << "\nBoard could not be found!!!\n\n" << endl;
		connection = false;
		return(connection);
	}
		
	return (connection);
}