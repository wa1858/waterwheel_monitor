// Program to monitor performance of the waterwheel

#include "modbus.hpp"
#include "logger.hpp"
#include "utils.hpp"
#include "config.h"

int main()
{
    Logger logger = Logger();
    logger.log(LogLevel::info, "Waterwheel Monitoring Program");
    logger.log(LogLevel::info, "Revision %d.%d.%d", PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH);
    logger.log(LogLevel::info, "Build Date: %s", BUILD_DATE);

    // TODO - Imput validation
    int port_number;
    logger.log(LogLevel::none, "Enter the desired serial port number: ");
    std::cin >> port_number;
    logger.log(LogLevel::info, "Selected port: COM%d", port_number);

    Modbus monitor = Modbus(port_number, logger);

    while (true)
    {
        logger.log(LogLevel::none, "");
        float frequency = monitor.getFrequency();
        float average_frequency = monitor.getAverageFrequency(frequency);
        logger.log(LogLevel::info, "Frequency (Hz) %2.1f%sAverage Frequency (Hz) %2.1f", frequency, WHITESPACE, average_frequency);
        monitor.checkAverageFrequency(average_frequency);

        float active_power = monitor.getActivePower();
        float average_active_power = monitor.getAverageActivePower(active_power);
        logger.log(LogLevel::info, "Active Power (W) %2.3f%sAverage Active Power (W) %2.3f", active_power, WHITESPACE, average_active_power);

        float total_active_energy = monitor.getTotalActiveEnergy();
        logger.log(LogLevel::info, "Total Active Energy (kWh) %2.0f", total_active_energy);

        float voltage = monitor.getVoltage();
        logger.log(LogLevel::info, "Voltage (V) %2.1f", voltage);

        float current = monitor.getCurrent();
        logger.log(LogLevel::info, "Current (A) %2.3f", current);

        float reactive_power = monitor.getReactivePower();
        logger.log(LogLevel::info, "Reactive Power (VAr) %2.3f", reactive_power);

        monitor.incrementAverage();

        delay(1800);
    }

    return 0;
}
