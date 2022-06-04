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

string SerialPort::ReadSerialPort(const int byte_amountToRead){
    DWORD bytes_read;
	char inc_msg[byte_amountToRead];	
	string complete_inc_msg;

	if (ReadFile(io, inc_msg, byte_amountToRead, &bytes_read, NULL)) {
		ClearCommError(io, &errors_, &status_);	
		complete_inc_msg.append(inc_msg, byte_amountToRead);		
	}
	else
		return "Warning: Failed to receive data.\n";

	return complete_inc_msg;
}

bool SerialPort::WriteSerialPort(char* data_sent){
    DWORD bytes_sent;	

	unsigned int data_sent_length = strlen(data_sent);

	if (!WriteFile(io, (void*)data_sent, data_sent_length, &bytes_sent, NULL)) {
		ClearCommError(io, &errors_, &status_);
		return false;
	}
	else
		return true;
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