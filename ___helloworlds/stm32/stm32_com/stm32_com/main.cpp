


#include <windows.h>
#include <string>
#include <iostream>

class SerialPort {
private:
    HANDLE hSerial;
    bool connected;
    COMSTAT status;
    DWORD errors;

public:
    SerialPort(const char* portName) : connected(false) {
        hSerial = CreateFileA(portName,
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

        if (hSerial == INVALID_HANDLE_VALUE) {
            std::cerr << "错误：无法打开串口 " << portName << std::endl;
            return;
        }

        DCB dcbSerialParams = { 0 };
        if (!GetCommState(hSerial, &dcbSerialParams)) {
            std::cerr << "错误：无法获取串口参数" << std::endl;
            return;
        }

        dcbSerialParams.BaudRate = CBR_115200;
        dcbSerialParams.ByteSize = 8;
        dcbSerialParams.StopBits = ONESTOPBIT;
        dcbSerialParams.Parity = NOPARITY;
        dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

        if (!SetCommState(hSerial, &dcbSerialParams)) {
            std::cerr << "错误：无法设置串口参数" << std::endl;
            return;
        }

        PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);
        connected = true;
        Sleep(2000);
    }

    ~SerialPort() {
        if (connected) {
            connected = false;
            CloseHandle(hSerial);
        }
    }

    std::string readData() {
        DWORD bytesRead;
        char buffer[256] = { 0 };
        std::string result = "";

        ClearCommError(hSerial, &errors, &status);
        if (status.cbInQue > 0) {
            if (ReadFile(hSerial, buffer, 255, &bytesRead, NULL)) {
                buffer[bytesRead] = 0;
                result = buffer;
            }
        }
        return result;
    }

    bool isConnected() { return connected; }
};

int main() {
    //SerialPort serial("\\\\.\\COM3"); // 根据实际串口修改
    SerialPort serial("COM5"); // 根据实际串口修改

    if (!serial.isConnected()) {
        std::cerr << "串口连接失败" << std::endl;
        return 1;
    }

    std::cout << "开始监听串口数据..." << std::endl;

    while (serial.isConnected()) {
        std::string data = serial.readData();
        if (!data.empty()) {
            std::cout << "接收到数据: " << data;
        }
        Sleep(100);
    }

    return 0;
}

