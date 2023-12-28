#include "serial.hpp"

namespace waterwheel::hardware
{
    Serial::Serial(utils::Logger &logger, int portNumber) : logger(logger)
    {
        serial = setUpSerial(portNumber);
        logger.log(utils::LogLevel::debug, "New Serial created");
    }

    Serial::~Serial()
    {
        CloseHandle(serial);
        logger.log(utils::LogLevel::debug, "Serial destroyed");
    }

    int Serial::selectSerialPort(utils::Logger &logger)
    {
        // TODO - Use getSerialPorts() here present list of ports to user
        // TODO - Imput validation
        int portNumber;
        std::cout << "Enter the desired serial port number: ";
        std::cin >> portNumber;
        logger.log(utils::LogLevel::debug, "Selected port: COM%d", portNumber);
        return portNumber;
    }

    // TODO - getSerialPorts();

    HANDLE Serial::setUpSerial(int portNumber)
    {
        // Declare variables and structures
        DCB dcbSerialParams = {0};
        COMMTIMEOUTS timeouts = {0};

        // String concatenation
        std::stringstream ss;
        ss << "\\\\.\\COM" << portNumber;
        std::string port = ss.str();

        // Open the desired serial port
        logger.log(utils::LogLevel::info, "Opening serial port COM%d", portNumber);

        HANDLE hSerial = CreateFile(
            port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr,
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (hSerial == INVALID_HANDLE_VALUE)
        {
            logger.log(utils::LogLevel::fatal, "Error");
            exit(EXIT_FAILURE);
        }
        logger.log(utils::LogLevel::info, "Port opened successfully");
        // Set device parameters (9600 baud, 1 start bit, 1 stop bit, no parity)
        dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
        if (GetCommState(hSerial, &dcbSerialParams) == 0)
        {
            logger.log(utils::LogLevel::fatal, "Error getting device state");
            CloseHandle(hSerial);
            exit(EXIT_FAILURE);
        }

        dcbSerialParams.BaudRate = CBR_9600;
        dcbSerialParams.ByteSize = 8;
        dcbSerialParams.StopBits = ONESTOPBIT;
        dcbSerialParams.Parity = NOPARITY;
        if (SetCommState(hSerial, &dcbSerialParams) == 0)
        {
            logger.log(utils::LogLevel::fatal, "Error setting device parameters");
            CloseHandle(hSerial);
            exit(EXIT_FAILURE);
        }

        // Set COM port timeout settings
        timeouts.ReadIntervalTimeout = 50;
        timeouts.ReadTotalTimeoutConstant = 50;
        timeouts.ReadTotalTimeoutMultiplier = 10;
        timeouts.WriteTotalTimeoutConstant = 50;
        timeouts.WriteTotalTimeoutMultiplier = 10;
        if (SetCommTimeouts(hSerial, &timeouts) == 0)
        {
            logger.log(utils::LogLevel::fatal, "Error setting timeouts");
            CloseHandle(hSerial);
            exit(EXIT_FAILURE);
        }
        return hSerial;
    }

    void Serial::writeData(const std::array<char, 8> &request)
    {
        DWORD bytes_written = 0;
        bool status = WriteFile(serial, request.data(), request.size(), &bytes_written, nullptr);
        if (!status)
        {
            logger.log(utils::LogLevel::warning, "Could not send request to serial port");
        }
    };

    void Serial::readData(std::array<char, 9> &result)
    {
        DWORD bytesRead = 0;
        bool status = ReadFile(serial, result.data(), result.size(), &bytesRead, nullptr);
        if (!status)
        {
            logger.log(utils::LogLevel::warning, "Could not read from serial port");
        }
    }
}