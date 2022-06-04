#include "../inc/ArduinoCheck.h"
#include <string>

string skipOption(){
    cout << "Skip selected port: [y/n] ";
    string input;
    cin >> input;

    return input;
}

const char* ArduinoCheck::checkForBoardPorts(){
    const char * com_ports[16] = {"NULL PORT", "\\\\.\\COM0", "\\\\.\\COM1", "\\\\.\\COM2", "\\\\.\\COM3", "\\\\.\\COM4", 
    "\\\\.\\COM5", "\\\\.\\COM6", "\\\\.\\COM7", "\\\\.\\COM8", "\\\\.\\COM9", "\\\\.\\COM10", "\\\\.\\COM11", 
    "\\\\.\\COM12", "\\\\.\\COM13", "\\\\.\\COM14"
    };

    //Always cycle one port once by one each every loop
    if(selectedPortNum < 16)
        selectedPortNum++;
    
    return com_ports[selectedPortNum];
}

const char* ArduinoCheck::boardCheck(){
    
    //Init local variable
    const char* port;

    //Cycle through all 16 com ports
    for(int i = 0; i < 15; i++){

        port = checkForBoardPorts();

        //Use whichever `port selected and verify it
        if(verifyBoard(port)){
            //If the selected port is valid return it but check if user wants to skip the selected port
            string option = skipOption();

            if(option == "y" || option == "Y"){
                //Check for other ports
                continue;
            }else if(option == "n" || option == "N"){
                //Return the valid port
                return port;
            }
        }else{
            //Continue to the next com port and check if its valid
            continue;
        }  
    }

    //When no port was found to be valid return a null pointer
    return nullptr;
}

bool ArduinoCheck::verifyBoard(const char* COM_PORT){

    //Create a file to be sent to a com port
    HANDLE io = CreateFileA(static_cast<LPCSTR>(COM_PORT), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, 0);

    //Check for returning value of the handle
    if(io == INVALID_HANDLE_VALUE){
        cout << "Error: Invalid port: " << COM_PORT << endl;
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