#include "inc/SerialPort.h"
#include "inc/TimeManager.h"
#include "inc/ArduinoCheck.h"

timeManager *tManager= new timeManager();
long long currentTime;

string userInput(){
	//Input from user in string form
	cout<< "Enter Command >> ";
	string inputRead;
	cin >> inputRead;

	return inputRead;
}

void serial(){
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
		SerialPort* serial = new SerialPort(com_port, 115200);

		cout << "Arduino board initialized succesfully" << endl;

		while(serial->is_connected){

			string input = userInput();
			
			//Optional user input to exit the program
			if(input == "exit"){
				serial->CloseSerialPort();
				break;
			}else if(input == "read"){
				string readMessage = serial->ReadSerialPort();
				cout << "SerailRead: " << readMessage << endl;
			}else{
				//translate into serial bytes
				char* data = &input[0];

				bool is_sent = serial->WriteSerialPort(data);

				if(is_sent){
					cout << "Message sent" << endl;
					currentTime = tManager->callTime();
					cout << "Time: " << ctime(&currentTime) << endl;
				}else{
					cout << "Error: no input sent" << endl;
					break;
				}
			}

			Sleep(1);
		}
	}else{
		cout << "Error: No Arduino board found" << endl;
	}
}

void timeSystem(){
	tManager->setTime();
}

//[]Make a time system that runs on a thread

int main(){
	//Initialize programs here
	std::thread thread_1(timeSystem);
	std::thread thread_2(serial);

	thread_1.join();
	thread_2.join();

	return 0;
}