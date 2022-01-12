#include "SerialPort.h"

namespace PC_Check {
	class CheckPort {
	private:
		bool checkForPort(bool isFound);
	public:
		const char* portSelected;
		bool checkDevice();
	};
}