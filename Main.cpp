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

<<<<<<< HEAD
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
=======
	SerialPort arduino(port);

	SerialPort arduino(port);

	if (arduino.isConnected()) {
		isFound = true;
		cout << "Port found on port " << port << endl;
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

		//Checks if the input string is similar to the required set strings to send a output to device
		if (data == "ON" || data == "OFF") {
			char* charArray = new char[data.size() + 1];
			copy(data.begin(), data.end(), charArray);
			charArray[data.size()] = '\n';

			arduino.writeSerialPort(charArray, MAX_DATA_LENGTH);
			arduino.readSerialPort(output, MAX_DATA_LENGTH);

			cout << ">> " << output << endl;

			delete[] charArray;
		}
		else {
			cout << "Unknown command" << endl;
		}
	}
	return 0;
>>>>>>> e0177840b346a8117269e81eaa58b4672141ddd4
}