#include "NLHChecker.h"
#include <iostream>

using namespace std;

using namespace PC_Check;

bool CheckPort::checkForPort(bool isFound) {
	while (!isFound) {
		const char* ports[] = { "\\\\.\\COM0", "\\\\.\\COM1", "\\\\.\\COM2", "\\\\.\\COM3", "\\\\.\\COM4" };
		int portSize = sizeof(ports) / sizeof(ports[0]);
		cout << "selected port is " << portSize << endl;
		for (int i = 0; i < portSize; i++)
		{
			ports[i] = portSelected;
		}
	}
	if (isFound)
		return false;
}
