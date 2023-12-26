#pragma once

#include <array>
#include <windows.h>
#include <sstream>
#include <iostream>
#include <string.h>
#include <iomanip> // For setprecision

#include "logger.hpp"

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

class Modbus
{
public:
    Modbus(int port_number);
    ~Modbus();
    void getFrequency();
    void getActivePower();
    void getTotalActiveEnergy();

private:
    /**
     * Wrapper function for all MODBUS requests
     * @param request   Array of chars required to retrieve
     *                  data from MODBUS
     */
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

    constexpr static float FREQUENCY_MIN = 44.5;
    constexpr static float FREQUENCY_MAX = 48.5;
    HANDLE serial;
    std::array<float, 10> averaging_frequency_data = {};
    std::array<float, 10> averaging_power_data = {};
    int average_count = 0;
    // TODO - Implement pointer to existing logger, rather than creating a new logger within Modbus
    Logger logger = Logger();
};
