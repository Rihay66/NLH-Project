#include "inc/SerialPort.h"
#include "inc/ArduinoCheck.h"

string rgbinput(){
	//Input from user
	cout<< ">> ";
	string inputRead;
	cin >> inputRead;

	return inputRead;
}

int main(){

	ArduinoCheck* ar = new ArduinoCheck;
	const char* com_port;

	com_port = ar->boardCheck();

	if(com_port != nullptr){
		cout << "Arduino is ready to initialize" << endl;
		delete ar;

		SerialPort serial(com_port, 9600);

		cout << "Arduino board initialized succesfully" << endl;

		while(serial.is_connected){
			string input = rgbinput();

			if(input == "sysexit")
				return 0;
			//translate
			char* charArray = new char[input.size() + 1];
			copy(input.begin(), input.end(), charArray);
			charArray[input.size()] = '\n';
			bool is_sent = serial.WriteSerialPort(charArray);
			if(is_sent){
				cout << "Message sent" << endl;
			}else{
				cout << "Error: no input sent" << endl;
				return 1;
			}
			
			Sleep(1);
		}
	}else{
		cout << "Error: No Arduino board found" << endl;
		return 1;
	}

	return 0;
}