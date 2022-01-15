#include "NLHChecker.h"

using namespace PC_Check;

bool CheckPort::checkForPort(bool isFound) {
	while (true) {
		if (!isFound) {
			return false;
		}
		const char* ports[] = { "\\\\.\\COM0", "\\\\.\\COM1", "\\\\.\\COM2", "\\\\.\\COM3", "\\\\.\\COM4" };
		int portSize = sizeof(ports) / sizeof(ports[0]);
		cout << "selected port is " << portSize << endl;
		for (int i = 0; i < portSize; i++)
		{
			portSelected = ports[i];
		}
	}
}

bool CheckPort::checkDevice() {
	while (true) {
		SerialPort device(portSelected);
		if (!device.isConnected()) {
			checkForPort(true);
		}
		else {
			checkForPort(false);
			return false;
		}
	}
}
