#pragma once

#include <windows.h>
#include <array>

#include <utils/logger.hpp>

namespace waterwheel::hardware
{
    // TODO - Return optional type for constructors?
    class Serial
    {
    public:
        Serial(utils::Logger &logger, int portNumber);
        ~Serial();

        /**
         * Send a data request through the serial port
         * @param request   Array of hex values corresponding to
         *                  desired data request
         */
        void writeData(const std::array<char, 8> &request);

        /**
         * Retrieve data from the serial port
         * @param result    Array of resulting bytes from read request
         */
        void readData(std::array<char, 9> &result);

    private:
        /**
         * Open a serial port for communication on Windows
         * @param portNumber    The number of the COM port to open
         * @returns An object referencing the opened COM port
         */
        HANDLE setUpSerial(int portNumber);

    private:
        utils::Logger &logger;
        HANDLE serial;
    };
}