#include "inc/SerialPort.h"
#include "inc/ArduinoCheck.h"

string userInput(){
	//Input from user in string form
	cout<< "Enter Command >> ";
	string inputRead;
	cin >> inputRead;

	return inputRead;
}

int main(){

	//Initialize arduino check as a pointer
	ArduinoCheck* ar = new ArduinoCheck;
	//Initialize local com port to be set to whichever port was valid
	const char* com_port;

	//Use the class to cycle all COM ports
	com_port = ar->boardCheck();

	if(com_port != nullptr){
		cout << "Arduino is ready to initialize" << endl;
		//Free memory by deleting the arduino check class
		delete ar;

		//Initialize the connection with arduino board
		SerialPort* serial = new SerialPort(com_port, 9600);

		cout << "Arduino board initialized succesfully" << endl;

		while(serial->is_connected){

			string input = userInput();
			
			//Optional user input to exit the program
			if(input == "exit"){
				serial->CloseSerialPort();
				return 0;
			}

			//translate into serial bytes
			char* data = &input[0];

			bool is_sent = serial->WriteSerialPort(data);

			if(is_sent){
				cout << "Message sent" << endl;
			}else{
				cout << "Error: no input sent" << endl;
				return 1;
			}

			string readMessage = serial->ReadSerialPort(3);
			cout << "SerailRead: " << readMessage << endl;

			Sleep(1);
		}
	}else{
		cout << "Error: No Arduino board found" << endl;
		return 1;
	}

	return 0;
}