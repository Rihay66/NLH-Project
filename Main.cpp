#include<iostream>
using namespace std;
#include<string>
#include<stdlib.h>
#include"SerialPort.h"
#include"NLHChecker.h"
using namespace PC_Check;
char output[MAX_DATA_LENGTH];
// change the name of the port with the port name of your computer
// must remember that the backslashes are essential so do not remove them
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
}