#pragma once

#include <array>
#include <windows.h>
#include <sstream>
#include <iostream>
#include <string.h>

#include "logger.hpp"
#include "utils.hpp"
#include "requests.hpp"

class Modbus
{
public:
    Modbus(int port_number, Logger &logger);
    ~Modbus();
    float getFrequency();
    float getAverageFrequency(float frequency);
    void checkAverageFrequency(float average_frequency);
    float getActivePower();
    float getAverageActivePower(float active_power);
    float getTotalActiveEnergy();
    float getVoltage();
    float getCurrent();
    float getReactivePower();
    float getApparentPower();
    float getPowerFactor();
    void incrementAverage();

private:
    /**
     * Wrapper function for all MODBUS requests
     * @param request   Array of chars required to retrieve
     *                  data from MODBUS
     * @returns Float value read from meter
     */
    float getData(const std::array<char, 8> &request);

    float getAverage(float value, std::array<float, 10> &array);

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

private:
    constexpr static float FREQUENCY_MIN = 44.5;
    constexpr static float FREQUENCY_MAX = 48.5;
    constexpr static int AVERAGES_ARRAY_SIZE = 10;

    std::array<float, AVERAGES_ARRAY_SIZE> averaging_frequency_data = {};
    std::array<float, AVERAGES_ARRAY_SIZE> averaging_power_data = {};
    int average_count = 0;

    HANDLE serial;
    Logger &logger;
};
