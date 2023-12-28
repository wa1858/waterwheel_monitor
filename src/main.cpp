#include <cstring>

#include <hardware/modbus.hpp>
#include <utils/logger.hpp>
#include <utils/utils.hpp>
#include <utils/debug.hpp>
// #include "config.h"

using namespace waterwheel;

int main(int argc, char *argv[])
{
    // Determine if debug mode is enabled
    bool debugMode = argc > 1 && strcmp(argv[0], utils::DEBUG_FLAG);
    utils::LogLevel level = debugMode ? utils::LogLevel::debug : utils::LogLevel::info;

    utils::Logger logger = utils::Logger(level);

    logger.log(utils::LogLevel::info, "Waterwheel Monitoring Program");
    // TODO - Restructure CMakeLists.txt to allow inclusion of config.h in build
    // logger.log(utils::LogLevel::debug, "Revision %d.%d.%d", PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH);
    // logger.log(utils::LogLevel::debug, "Build Date: %s", BUILD_DATE);

    // TODO - Imput validation
    int port_number;
    std::cout << "Enter the desired serial port number: ";
    std::cin >> port_number;
    logger.log(utils::LogLevel::debug, "Selected port: COM%d", port_number);

    int delayAmount = utils::defaultDelay;

    if (debugMode)
    {
        utils::debugConfigureDelay(logger, &delayAmount);
        // TODO - utils::debugSetFrequencyLimits();
    }

    hardware::Modbus monitor = hardware::Modbus(port_number, logger);

    // TODO - Can this big loop with print statements be cleaned up?
    while (true)
    {
        // Break between each record
        std::cout << std::endl;

        float frequency = monitor.getFrequency();
        float average_frequency = monitor.getAverageFrequency(frequency);
        // TODO - The centering of the output prints is done using tabs here; is there a better way?
        logger.log(utils::LogLevel::info, "Frequency (Hz):              %2.1f   Average Frequency (Hz):     %2.1f", frequency, average_frequency);
        monitor.checkAverageFrequency(average_frequency);

        float active_power = monitor.getActivePower();
        float average_active_power = monitor.getAverageActivePower(active_power);
        logger.log(utils::LogLevel::info, "Active Power (W):            %2.3f   Average Active Power (W):   %2.3f", active_power, average_active_power);

        float total_active_energy = monitor.getTotalActiveEnergy();
        logger.log(utils::LogLevel::info, "Total Active Energy (kWh):   %2.0f", total_active_energy);

        float voltage = monitor.getVoltage();
        logger.log(utils::LogLevel::info, "Voltage (V):                 %2.1f", voltage);

        float current = monitor.getCurrent();
        logger.log(utils::LogLevel::info, "Current (A):                 %2.3f", current);

        float reactive_power = monitor.getReactivePower();
        logger.log(utils::LogLevel::info, "Reactive Power (VAr):        %2.3f", reactive_power);

        float apparent_power = monitor.getApparentPower();
        logger.log(utils::LogLevel::info, "Apparent Power (VA):         %2.3f", apparent_power);

        float power_factor = monitor.getPowerFactor();
        logger.log(utils::LogLevel::info, "Power Factor:                %2.3f", power_factor);

        float phase_angle = monitor.getPhaseAngle();
        logger.log(utils::LogLevel::info, "Phase Angle (degrees):       %2.3f", phase_angle);

        // TODO - Find a better way to do this
        monitor.incrementAverage();

        utils::delay(delayAmount);
    }

    return 0;
}
