#include "../inc/ArduinoCheck.h"

const char* ArduinoCheck::checkForBoardPorts(){
    const char * com_ports[16] = {"NULL PORT", "\\\\.\\COM0", "\\\\.\\COM1", "\\\\.\\COM2", "\\\\.\\COM3", "\\\\.\\COM4", 
    "\\\\.\\COM5", "\\\\.\\COM6", "\\\\.\\COM7", "\\\\.\\COM8", "\\\\.\\COM9", "\\\\.\\COM10", "\\\\.\\COM11", 
    "\\\\.\\COM12", "\\\\.\\COM13", "\\\\.\\COM14"
    };

    if(selectedPortNum < 16)
        selectedPortNum++;

    return com_ports[selectedPortNum];
}

const char* ArduinoCheck::boardCheck(){
    
    //Init local variable
    const char* port;
    bool Hresult;

    for(int i = 0; i < 16; i++){

        port = checkForBoardPorts();
        Hresult = verifyBoard(port);
        if(Hresult){
            return port;
        }else{
            continue;
        }  
    }

    return nullptr;
}

bool ArduinoCheck::verifyBoard(const char* COM_PORT){

    HANDLE io = CreateFileA(static_cast<LPCSTR>(COM_PORT), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, 0);

    if(io == INVALID_HANDLE_VALUE){
        cout << "Error: Invalid port handle" << endl;
        FlushFileBuffers(io);
        CloseHandle(io);
        return false;
    }else{
        cout << "Found Board on " << COM_PORT << endl;
        FlushFileBuffers(io);
        CloseHandle(io);
        return true;
    }
}