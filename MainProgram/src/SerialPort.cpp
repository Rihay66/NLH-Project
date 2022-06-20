#include "../inc/SerialPort.h"

SerialPort::SerialPort(const char* COM_Port, DWORD COM_BAUD_RATE){
    is_connected = false;

	io = CreateFileA(static_cast<LPCSTR>(COM_Port),
							GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL);

	if (io == INVALID_HANDLE_VALUE) {

		if (GetLastError() == ERROR_FILE_NOT_FOUND)
			printf("Warning: Handle was not attached. Reason: %s not available\n", COM_Port);
	}
	else {

		DCB dcbSerialParams = { 0 };

		if (!GetCommState(io, &dcbSerialParams)) {

			printf("Warning: Failed to get current serial params");
		}

		else {
			dcbSerialParams.BaudRate = COM_BAUD_RATE;
			dcbSerialParams.ByteSize = 8;
			dcbSerialParams.StopBits = ONESTOPBIT;
			dcbSerialParams.Parity = NOPARITY;
			dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

			if (!SetCommState(io, &dcbSerialParams))
				printf("Warning: could not set serial port params\n");
			else {
				is_connected = true;
				PurgeComm(io, PURGE_RXCLEAR | PURGE_TXCLEAR);
				verifyBoard(COM_Port);				
			}
		}
	}
}

SerialPort::~SerialPort(){
    if (is_connected) {
		is_connected = false;
		CloseHandle(io);		
	}
}

string SerialPort::userPasswordInput(){
	cout << "Enter password for board: ";
	string input;
	cin >> input;

	return input;
}

string SerialPort::ReadSerialPort(){
    DWORD bytes_read;	
	string complete_inc_msg;

	//debug
	std::this_thread::sleep_for(2000ms);

	//Check for errors on the COM
	if(ClearCommError(io, &errors_, &status_)){
		//Set the amount bytes on the COM
		int amount_bytes = status_.cbInQue;
		//Check if the bytes from the COM is empty
		if(amount_bytes > 0){
			
			//Make a char set to the size of the bytes on the COM
			char inc_msg[amount_bytes];
			
			//Read bytes from the Serial
			if (ReadFile(io, inc_msg, amount_bytes, &bytes_read, NULL)) {
				//Put every character from the bytes into a string
				complete_inc_msg.append(inc_msg, amount_bytes);
			}else{
				return "Error: reading the Serial.";
			}
			
		}else{
			return "Error: no bytes from Serial.";
		}
	}else{
		return "Error: unable to detect errors in COM.";
	}
	
	//Return whatever this string is set
	return complete_inc_msg;
}

bool SerialPort::WriteSerialPort(char* data_sent){
    DWORD bytes_sent;	

	unsigned int data_sent_length = strlen(data_sent);

	if (!WriteFile(io, (void*)data_sent, data_sent_length, &bytes_sent, NULL)) {
		ClearCommError(io, &errors_, &status_);
		printf("Failed to write file\n");
		return false;
	}
	else
		return true;
}

bool SerialPort::verifyBoard(const char* com_port){

	string input = userPasswordInput();

	char* dataToSend = &input[0];

	while(is_connected == true){

		bool write = WriteSerialPort(dataToSend);

		if(write){
			string read = ReadSerialPort();
			cout << "MSG:" << read << endl;
			if(read == "PC"){
				cout << "Board verified!" << endl;
				return true;
			}else{
				break;
			}		
		}else{
			break;
		}

		Sleep(1);
	}

	cout << "Error: Board no responding, not a arduino" << endl;
	CloseSerialPort();
	return false;
}

bool SerialPort::CloseSerialPort(){
    if (is_connected) {
		is_connected = false;
		CloseHandle(io);
		return true;
	}	
	else
		return false;
}