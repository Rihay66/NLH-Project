#include "NLHChecker.h"

using namespace PC_Check;

bool CheckPort::checkForPort(bool isFound) {
	while (true) {
		const char* ports[] = { "\\\\.\\COM0", "\\\\.\\COM1", "\\\\.\\COM2", "\\\\.\\COM3", "\\\\.\\COM4" };
		int portSize = sizeof(ports) / sizeof(ports[0]);
		for (int i = 0; i < portSize; i++)
		{
			ports[i] = portSelected;
		}
	}
	if (isFound)
		return false;
}
