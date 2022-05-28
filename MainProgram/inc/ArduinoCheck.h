#pragma once
#include <Windows.h>
#include <iostream>

using namespace std;

class ArduinoCheck{
    private:
        int selectedPortNum = 0;
        bool verifyBoard(const char* COM_Port);
        const char* checkForBoardPorts();

    public:
        const char* boardCheck();
};