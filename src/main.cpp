// Program to monitor performance of the waterwheel

#include <hardware/modbus.hpp>
#include <utils/logger.hpp>
#include <utils/utils.hpp>
// #include "config.h"

using namespace waterwheel;

int main()
{
    utils::Logger logger = utils::Logger(utils::LogLevel::debug);
    logger.log(utils::LogLevel::info, "Waterwheel Monitoring Program");
    // TODO - Restructure CMakeLists.txt to allow inclusion of config.h in build
    // logger.log(utils::LogLevel::debug, "Revision %d.%d.%d", PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH);
    // logger.log(utils::LogLevel::debug, "Build Date: %s", BUILD_DATE);

    // TODO - Imput validation
    int portNumber;
    logger.log(utils::LogLevel::info, "Enter the desired serial port number: ");
    std::cin >> portNumber;
    logger.log(utils::LogLevel::debug, "Selected port: COM%d", portNumber);

    hardware::Modbus monitor = hardware::Modbus(logger, portNumber);

    while (true)
    {
        logger.log(utils::LogLevel::info, "");
        float frequency = monitor.getFrequency();
        float average_frequency = monitor.getAverageFrequency(frequency);
        logger.log(utils::LogLevel::info, "Frequency (Hz): %2.1f%sAverage Frequency (Hz): %2.1f", frequency, utils::WHITESPACE, average_frequency);
        monitor.checkAverageFrequency(average_frequency);

        float active_power = monitor.getActivePower();
        float average_active_power = monitor.getAverageActivePower(active_power);
        logger.log(utils::LogLevel::info, "Active Power (W): %2.3f%sAverage Active Power (W): %2.3f", active_power, utils::WHITESPACE, average_active_power);

        float total_active_energy = monitor.getTotalActiveEnergy();
        logger.log(utils::LogLevel::info, "Total Active Energy (kWh): %2.0f", total_active_energy);

        float voltage = monitor.getVoltage();
        logger.log(utils::LogLevel::info, "Voltage (V): %2.1f", voltage);

        float current = monitor.getCurrent();
        logger.log(utils::LogLevel::info, "Current (A): %2.3f", current);

        float reactive_power = monitor.getReactivePower();
        logger.log(utils::LogLevel::info, "Reactive Power (VAr): %2.3f", reactive_power);

        float apparent_power = monitor.getApparentPower();
        logger.log(utils::LogLevel::info, "Apparent Power (VA): %2.3f", apparent_power);

        float power_factor = monitor.getPowerFactor();
        logger.log(utils::LogLevel::info, "Power Factor: %2.3f", power_factor);

        float phase_angle = monitor.getPhaseAngle();
        logger.log(utils::LogLevel::info, "Phase Angle (degrees): %2.3f", phase_angle);

        // TODO - Find a better way to do this
        monitor.incrementAverage();

        utils::delay(1800);
    }

    return 0;
}
