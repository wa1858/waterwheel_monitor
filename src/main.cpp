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

    Modbus monitor = Modbus(port_number);

    while (true)
    {
        logger.log(LogLevel::none, "");
        monitor.getFrequency();
        monitor.getActivePower();
        monitor.getTotalActiveEnergy();
        delay(1800);
    }

    return 0;
}
