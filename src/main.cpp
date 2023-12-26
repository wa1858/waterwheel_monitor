// Program to monitor performance of the waterwheel

#include "modbus.hpp"
#include "logger.hpp"
#include "utils.hpp"
#include "config.h"

int main()
{
    Logger logger = Logger();
    logger.log(LogLevel::info, "Waterwheel Monitoring Program");
    // TODO - Find a better way to form strings like this for logging (pass ints straight into logger.log)
    std::stringstream ss_;
    ss_ << "Revision " << PROJECT_VERSION_MAJOR << "." << PROJECT_VERSION_MINOR << "." << PROJECT_VERSION_PATCH;
    logger.log(LogLevel::info, ss_.str());
    std::stringstream ss__;
    ss__ << "Build Date: " << BUILD_DATE;
    logger.log(LogLevel::info, ss__.str());

    // TODO - Imput validation
    int port_number;
    logger.log(LogLevel::none, "Enter the desired serial port number: ", false);
    std::cin >> port_number;
    logger.log(LogLevel::info, "Selected port: COM", false);
    std::stringstream ss;
    ss << port_number;
    logger.log(LogLevel::none, ss.str());

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
