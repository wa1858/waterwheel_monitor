#pragma once

#include <array>
#include <windows.h>

#include <utils/logger.hpp>
#include <utils/utils.hpp>
#include "requests.hpp"

namespace waterwheel::hardware
{
    class Modbus
    {
    public:
        Modbus(int port_number, utils::Logger &logger);
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
        float getPhaseAngle();
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
        static constexpr float FREQUENCY_MIN = 44.5;
        static constexpr float FREQUENCY_MAX = 48.5;
        static constexpr int AVERAGES_ARRAY_SIZE = 10;

        std::array<float, AVERAGES_ARRAY_SIZE> averaging_frequency_data = {};
        std::array<float, AVERAGES_ARRAY_SIZE> averaging_power_data = {};
        int average_count = 0;

        HANDLE serial;
        utils::Logger &logger;
    };
}