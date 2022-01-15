#include "SerialPort.h"

const char* SerialPort::Gen_Port_Name(void) {
    /* Return ERROR if Max Ports Reached */

    //[] Add a few more ports at least up to 12
    const char* ports[] = { "\\\\.\\COM0", "\\\\.\\COM1", "\\\\.\\COM2", "\\\\.\\COM3", "\\\\.\\COM4" };
    int currentport = 0;

    if (currentport > maxports)
        printf("ERROR"); // No more ports to try
    else
        currentport++; // Update to next port

    return(ports[currentport]);
}

HANDLE SerialPort::Init_Serial(const char* portname) {
    
    printf("\n\nPortName: %s\n\n", portname);
    this->handler = CreateFileA(static_cast<LPCSTR>(portname), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, 0);

    if (handler == INVALID_HANDLE_VALUE)
    {
        FlushFileBuffers(handler); // flush buffers to tell Ardunio to bark it's data 
        CloseHandle(handler); // Prevent Handle leaks
        return INVALID_HANDLE_VALUE; // Move on to next port since this one doesn't exist
    }
    else
    {
        int attempt = serial_params(handler);

        if (attempt != (-1))
            return handler;
        else
            return INVALID_HANDLE_VALUE;
    }
}

int SerialPort::serial_params(HANDLE hSerial) {
    DWORD testWrote = 0;
    DWORD testRead = 0;
    DCB dcbSerialParams = { 0 };
    COMMTIMEOUTS timeouts = { 0 };

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        printf("\nError Getting Com Port State!\n\n");
        return (-1);
    }

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        printf("\n\nError Setting Serial Port STATE!\n\n");
        return(-1);
    }

    if (!GetCommState(this->handler, &dcbSerialParams)) {
        printf("failed to get current serial parameters");
    }
    else {
        dcbSerialParams.BaudRate = CBR_9600;
        dcbSerialParams.ByteSize = 8;
        dcbSerialParams.StopBits = ONESTOPBIT;
        dcbSerialParams.Parity = NOPARITY;
        dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

        if (!SetCommState(handler, &dcbSerialParams))
        {
            printf("ALERT: could not set Serial port parameters\n");
        }
        else {
            PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
            Sleep(ARDUINO_WAIT_TIME);
        }
    }
    // To Prevent Timing out the serial port Tell Windows not to wait up for us

    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        printf("\n\n Serious Timeout error occured!!\n\n");
        return 0;
    }

}

int SerialPort::Wait_Ready(HANDLE hSerial) {
    /* This function waits for the ready Status of the Arduino Board */
      // TODO ERROR HANDLING FOR WRITE AND READFILE FUNCTIONS

    int Read_Timeout = 0;
    DWORD testRead = 0;
    DWORD testWrote = 0;
    char readystatus[8] = { 0 };

    WriteFile(hSerial, "~READY~", sizeof(readystatus), &testWrote, NULL);

    FlushFileBuffers(hSerial); // flush buffers to tell Ardunio to bark it's data 
    /*
    int iWanted = 8;
    char* pCurrentBuf = readystatus;
    
    DWORD result;
    do {
        result = ReadFile(hSerial, pCurrentBuf, iWanted, &testRead, NULL);
        if (result != ERROR_SUCCESS)
        {
            // ok, do some good ol' error checking here.
        }
        pCurrentBuf += testRead;
        iWanted -= testRead;
        Sleep(1);
        Read_Timeout++;

    } while ((iWanted > 0) && Read_Timeout <= 100);
    *pCurrentBuf = NULL;
    */
    do {
        Sleep(1);
        printf("\nWrote %ld BYTES\n", testWrote);
    } while (testWrote < 8);

    do
    {
        return ReadFile(hSerial, readystatus, 8, &testRead, NULL);
        Sleep(1);
        Read_Timeout++;
    } while ((testRead < 8) && (Read_Timeout <= 100));

    readystatus[7] = '\0';
    printf("\n\nREADY STATUS: %s\n\n", readystatus);
    if (strstr(readystatus, "READ1") != NULL)
        return true; // Board Detected
    else
        return false; // Board not here
}

int SerialPort::readSerialPort(char* buffer, unsigned int buf_size)
{
    DWORD bytesRead;
    unsigned int toRead = 0;

    ClearCommError(this->handler, &this->errors, &this->status);

    if (this->status.cbInQue > 0) {
        if (this->status.cbInQue > buf_size) {
            toRead = buf_size;
        }
        else toRead = this->status.cbInQue;
    }

    if (ReadFile(this->handler, buffer, toRead, &bytesRead, NULL)) return bytesRead;

    return 0;
}

bool SerialPort::writeSerialPort(char* buffer, unsigned int buf_size)
{
    DWORD bytesSend;

    if (!WriteFile(this->handler, (void*)buffer, buf_size, &bytesSend, 0)) {
        ClearCommError(this->handler, &this->errors, &this->status);
        return false;
    }
    else return true;
}


/*
SerialPort::SerialPort(const char* portName)
{
    this->connected = false;

    this->handler = CreateFileA(static_cast<LPCSTR>(portName),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (this->handler == INVALID_HANDLE_VALUE) {
        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            printf("ERROR: Handle was not attached. Reason: %s not available\n", portName);
        }
        else
        {
            printf("ERROR!!!");
        }
    }
    else {
        DCB dcbSerialParameters = { 0 };

        if (!GetCommState(this->handler, &dcbSerialParameters)) {
            printf("failed to get current serial parameters");
        }
        else {
            dcbSerialParameters.BaudRate = CBR_9600;
            dcbSerialParameters.ByteSize = 8;
            dcbSerialParameters.StopBits = ONESTOPBIT;
            dcbSerialParameters.Parity = NOPARITY;
            dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

            if (!SetCommState(handler, &dcbSerialParameters))
            {
                printf("ALERT: could not set Serial port parameters\n");
            }
            else {
                this->connected = true;
                PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
                Sleep(ARDUINO_WAIT_TIME);
            }
        }
    }
}

SerialPort::~SerialPort()
{
    if (this->connected) {
        this->connected = false;
        CloseHandle(this->handler);
    }
}

int SerialPort::readSerialPort(char* buffer, unsigned int buf_size)
{
    DWORD bytesRead;
    unsigned int toRead = 0;

    ClearCommError(this->handler, &this->errors, &this->status);

    if (this->status.cbInQue > 0) {
        if (this->status.cbInQue > buf_size) {
            toRead = buf_size;
        }
        else toRead = this->status.cbInQue;
    }

    if (ReadFile(this->handler, buffer, toRead, &bytesRead, NULL)) return bytesRead;

    return 0;
}

bool SerialPort::writeSerialPort(char* buffer, unsigned int buf_size)
{
    DWORD bytesSend;

    if (!WriteFile(this->handler, (void*)buffer, buf_size, &bytesSend, 0)) {
        ClearCommError(this->handler, &this->errors, &this->status);
        return false;
    }
    else return true;
}

bool SerialPort::isConnected()
{
    return this->connected;
}
*/