#include<iostream>
using namespace std;
#include<string>
#include<stdlib.h>
#include"SerialPort.h"
#include"NLHChecker.h"
using namespace PC_Check;
char output[MAX_DATA_LENGTH];
const char *port;
char incomingData[MAX_DATA_LENGTH];
bool isFound = false;

int main() {
	SerialPort arduino(port);

	CheckPort check;
	check.checkForPort(isFound); //checks for every available port and might cycle through them until the correct port is found
	port = check.portSelected; //Takes returned port char

	if (arduino.isConnected()) {
		isFound = true;
		cout << "Connection made" << endl << endl;
	}
	else {
		cout << "Port not found" << endl << endl;
		isFound = false;
	}
	while (arduino.isConnected()) {
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