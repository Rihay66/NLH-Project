#pragma once

#include "iostream"
#include "fstream"
#include <Windows.h>
#include "string"
#include "stdlib.h"
#include "stdio.h"

using namespace std;

class SerialPort{
	private:
		HANDLE io;
		COMSTAT status_;
		DWORD errors_;

		string syntax_name;
		char f_delimiter;
		char e_delimiter;
	public:
		bool is_connected;

		SerialPort(const char* com_port, DWORD COM_BAUD_RATE);

		string ReadSerialPort(const int byte_amountToRead);
		bool WriteSerialPort(char* data_sent);
		bool CloseSerialPort();
		~SerialPort();
};