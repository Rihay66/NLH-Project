#include "NLHChecker.h"
<<<<<<< HEAD
#include "SerialPort.h"
=======
#include <iostream>

using namespace std;
>>>>>>> e0177840b346a8117269e81eaa58b4672141ddd4

using namespace PC_Check;

bool CheckPort::checkForPort(bool isFound) {
<<<<<<< HEAD
	while (true) {

		if (!isFound) {
			return false;
		}

=======
	while (!isFound) {
>>>>>>> e0177840b346a8117269e81eaa58b4672141ddd4
		const char* ports[] = { "\\\\.\\COM0", "\\\\.\\COM1", "\\\\.\\COM2", "\\\\.\\COM3", "\\\\.\\COM4" };
		int portSize = sizeof(ports) / sizeof(ports[0]);
		cout << "selected port is " << portSize << endl;
		for (int i = 0; i < portSize; i++)
		{
			ports[i] = portSelected;
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
