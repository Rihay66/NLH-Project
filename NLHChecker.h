#include <iostream>
#include "SerialPort.h"

using namespace std;

namespace PC_Check {
	class CheckPort {
	private:
		bool checkForPort(bool isFound);
	public:
		const char* portSelected;
		bool checkDevice();
	};
}