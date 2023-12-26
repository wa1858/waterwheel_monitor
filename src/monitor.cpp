// Program to monitor performance of the waterwheel

// TODO - Convert C functions to C++

#include <stdio.h>
#include <memory.h>
#include <time.h>
#include <string.h>
#include <windows.h>

#include <iostream>
#include <iomanip> // For setprecision
#include <array>
#include <sstream>

const static std::string WHITESPACE = "        ";

// TODO - Put these arrays in a separate header file
// Define frequency_request command per Energy Meter Modbus ICD
const static std::array<char, 8> REQUEST_FREQUENCY = {
    static_cast<char>(0x01),  // address 01
    static_cast<char>(0x04),  // read input registers
    static_cast<char>(0x00),  // Hi Byte Frequency
    static_cast<char>(0x46),  // Lo Byte Frequency
    static_cast<char>(0x00),  // Number of registers high
    static_cast<char>(0x02),  // Number of registers low
    static_cast<char>(0x90),  // Error check low - computed using online calculator
    static_cast<char>(0x1E)}; // Error check high - computed using online calculator

const static std::array<char, 8> REQUEST_ACTIVE_POWER = {
    static_cast<char>(0x01),  // address 01
    static_cast<char>(0x04),  // read input registers
    static_cast<char>(0x00),  // Hi Byte Frequency
    static_cast<char>(0x0C),  // Lo Byte Frequency
    static_cast<char>(0x00),  // Number of registers high
    static_cast<char>(0x02),  // Number of registers low
    static_cast<char>(0xB1),  // Error check low - computed using online calculator
    static_cast<char>(0xC8)}; // Error check high - computed using online calculator

const static std::array<char, 8> REQUEST_TOTAL_ACTIVE_ENERGY = {
    static_cast<char>(0x01),  // address 01
    static_cast<char>(0x04),  // read input registers
    static_cast<char>(0x01),  // Hi Byte Frequency
    static_cast<char>(0x56),  // Lo Byte Frequency
    static_cast<char>(0x00),  // Number of registers high
    static_cast<char>(0x02),  // Number of registers low
    static_cast<char>(0x90),  // Error check low - computed using online calculator
    static_cast<char>(0x27)}; // Error check high - computed using online calculator

// TODO - Configure for adding version number to code
// #include "config.h"

// TODO - Put Modbus into its own files
class Modbus
{
public:
    Modbus(int port_number);
    ~Modbus();
    void getFrequency();

protected:
private:
    float getData(const std::array<char, 8> &request);

    int writeData(const std::array<char, 8> &request);

    bool readData(std::array<char, 9> &bytes_to_read);
    /**
     * Create a HANDLE variable for accessing the serial port
     */
    HANDLE setupSerial(int port_number);
    /**
     * Converts four char bytes to an equivalent 32-bit float
     * Implemnted as the energy meter generates data in floating
     * point format but program reads data as four distinct bytes
     */
    float convertDataToFloat(const std::array<char, 9> &bytes_to_read);
    /**
     * Create time delay as required
     */
    void delay(int milli_seconds);

    constexpr static float FREQUENCY_MIN = 44.0;
    constexpr static float FREQUENCY_MAX = 48.0;

    HANDLE serial;
    // TODO - Implement rolling average of frequencies
    std::array<float, 10> averaging_frequency_data = {};
    int average_count = 0;
};

Modbus::Modbus(int port_number_)
{
    this->serial = this->setupSerial(port_number_);
}

Modbus::~Modbus()
{
}

float Modbus::getData(const std::array<char, 8> &request)
{
    writeData(request);
    std::array<char, 9> data;
    readData(data);
    return convertDataToFloat(data);
}

int Modbus::writeData(const std::array<char, 8> &request)
{
    DWORD bytes_written = 0;
    if (!WriteFile(this->serial, request.data(), request.size(), &bytes_written, nullptr))
    {
        return 1;
    }
    return 0;
};

// TODO - What should this return if okay?
bool Modbus::readData(std::array<char, 9> &bytes_to_read)
{
    DWORD bytes_read = 0;
    bool status = ReadFile(this->serial, bytes_to_read.data(), bytes_to_read.size(), &bytes_read, nullptr);
    if (!status)
    {
        std::cerr << "Error - Could not read from serial port" << std::endl;
    }
    return status;
}

HANDLE Modbus::setupSerial(int port_number)
{
    // Declare variables and structures
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};

    // String concatenation
    std::stringstream ss;
    ss << "\\\\.\\COM" << port_number;
    std::string port = ss.str();

    // Open the desired serial port
    std::cout << "Opening serial port COM" << port_number << "... ";
    HANDLE hSerial = CreateFile(
        port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hSerial == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Error" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "OK" << std::endl;

    // Set device parameters (9600 baud, 1 start bit, 1 stop bit, no parity)
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (GetCommState(hSerial, &dcbSerialParams) == 0)
    {
        std::cerr << "Error getting device state" << std::endl;
        CloseHandle(hSerial);
        exit(EXIT_FAILURE);
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if (SetCommState(hSerial, &dcbSerialParams) == 0)
    {
        std::cerr << "Error setting device parameters" << std::endl;
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
        std::cerr << "Error setting timeouts" << std::endl;
        CloseHandle(hSerial);
        exit(EXIT_FAILURE);
    }
    return hSerial;
}

float Modbus::convertDataToFloat(const std::array<char, 9> &bytes_to_read)
{
    // Convert the four char bytes into an equivalent 32-bit float
    // Note static_cast forces compiler to treat array members as int, not char
    int bytes[4];
    bytes[0] = static_cast<int>(bytes_to_read[3]);
    bytes[1] = static_cast<int>(bytes_to_read[4]);
    bytes[2] = static_cast<int>(bytes_to_read[5]);
    bytes[3] = static_cast<int>(bytes_to_read[6]);

    // Convert the four integers into a single integer representing a 32-bit number
    uint32_t int_data = (bytes[0] << 24) + (bytes[1] << 16) + (bytes[2] << 8) + bytes[3];

    // Assign result to memory assigned for float, resulting in float cast
    float f;
    memcpy(&f, &int_data, sizeof(float));

    return f;
}

void Modbus::delay(int milli_seconds)
{
    // Storing start time
    clock_t start_time = clock();

    // Loop for required time
    while (clock() < start_time + milli_seconds)
        ;
}

void Modbus::getFrequency()
{
    float frequency = getData(REQUEST_FREQUENCY);
    std::cout << "Class Frequency (Hz) " << std::fixed << std::setprecision(1) << frequency;
    averaging_frequency_data[average_count] = frequency;
    // TODO - Extract average calculation into its own function
    float average_frequency = 0.0;
    if (average_count < averaging_frequency_data.size())
    {
        average_count++;
    }
    else
    {
        average_count = 0;
    }
    for (int i = 0; i < averaging_frequency_data.size(); i++)
    {
        average_frequency += averaging_frequency_data[i];
    }
    average_frequency /= averaging_frequency_data.size();
    std::cout << WHITESPACE << "Average Frequency (Hz) " << average_frequency;
    if (average_frequency > FREQUENCY_MAX)
    {
        std::cerr << WHITESPACE << "OVERSPEED WARNING";
    }
    else if (average_frequency < FREQUENCY_MIN)
    {
        std::cerr << WHITESPACE << "UNDERSPEED WARNING";
    }
    std::cout << std::endl;

    delay(1000);
}

// TODO - Extract data retrieval from main()
int main()
{
    std::cout << "Monitor Program for Waterwheel" << std::endl;
    std::cout << "Revision 1.3" << std::endl;
    std::cout << "Build date: 26/12/2023" << std::endl;

    // TODO - Imput validation
    int port_number;
    std::cout << "Enter the desired serial port number: ";
    std::cin >> port_number;
    std::cout << "Selected port: COM" << port_number << std::endl;

    Modbus monitor = Modbus(port_number);

    for (int i = 1; i < 60000; i++)
    {
        monitor.getFrequency();
    }

    return 0;
}
