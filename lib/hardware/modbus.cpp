#include "modbus.hpp"

namespace waterwheel::hardware
{
    Modbus::Modbus(utils::Logger &logger_, int port_number) : logger(logger_), serial(logger_, port_number)
    {
        this->logger.log(utils::LogLevel::debug, "New Modbus created");
    }

    Modbus::~Modbus()
    {
        this->serial.~Serial();
        this->logger.log(utils::LogLevel::debug, "Modbus object destroyed");
    }

    float Modbus::getData(const std::array<char, 8> &request)
    {
        serial.writeData(request);
        std::array<char, 9> data;
        serial.readData(data);
        return convertDataToFloat(data);
    }

    float Modbus::getAverage(float value, std::array<float, AVERAGES_ARRAY_SIZE> &array)
    {
        array[average_count] = value;
        float average = 0.0;
        for (int i = 0; i < AVERAGES_ARRAY_SIZE; i++)
        {
            average += array[i];
        }
        return average / AVERAGES_ARRAY_SIZE;
    }

    void Modbus::incrementAverage()
    {
        average_count++;
        if (average_count >= AVERAGES_ARRAY_SIZE)
        {
            average_count = 0;
        }
    }

    float Modbus::convertDataToFloat(const std::array<char, 9> &bytes_to_read)
    {
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

    float Modbus::getFrequency()
    {
        return getData(REQUEST_FREQUENCY);
    }

    float Modbus::getAverageFrequency(float frequency)
    {
        return getAverage(frequency, averaging_frequency_data);
    }

    // TODO - Find a better audio prompt than MessageBeep
    void Modbus::checkAverageFrequency(float average_frequency)
    {
        if (average_frequency > FREQUENCY_MAX)
        {
            logger.log(utils::LogLevel::warning, "%2.1fHz average frequency - OVERSPEED WARNING", average_frequency);
            MessageBeep(MB_ICONWARNING);
        }
        else if (average_frequency < FREQUENCY_MIN)
        {
            logger.log(utils::LogLevel::warning, "%2.1fHz average frequency - UNDERSPEED WARNING", average_frequency);
            MessageBeep(MB_ICONWARNING);
        }
    }

    float Modbus::getActivePower()
    {
        return getData(REQUEST_ACTIVE_POWER);
    }

    float Modbus::getAverageActivePower(float active_power)
    {
        return getAverage(active_power, averaging_power_data);
    }

    float Modbus::getTotalActiveEnergy()
    {
        return getData(REQUEST_TOTAL_ACTIVE_ENERGY);
    }

    float Modbus::getVoltage()
    {
        return getData(REQUEST_VOLTAGE);
    }

    float Modbus::getCurrent()
    {
        return getData(REQUEST_CURRENT);
    }

    float Modbus::getReactivePower()
    {
        return getData(REQUEST_REACTIVE_POWER);
    }

    float Modbus::getApparentPower()
    {
        return getData(REQUEST_APPARENT_POWER);
    }

    float Modbus::getPowerFactor()
    {
        return getData(REQUEST_POWER_FACTOR);
    }

    float Modbus::getPhaseAngle()
    {
        return getData(REQUEST_PHASE_ANGLE);
    }
}
