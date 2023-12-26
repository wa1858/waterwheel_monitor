#include "logger.hpp"

Logger::Logger()
{
}
Logger::~Logger()
{
}

void Logger::log(LogLevel logLevel, std::string message, bool newLine)
{
    std::stringstream ss;
    switch (logLevel)
    {
    case LogLevel::info:
        ss << COLOUR_INFO << "[INFO] " << COLOUR_RESET;
        break;
    case LogLevel::warning:
        ss << COLOUR_WARNING << "[WARNING] " << COLOUR_RESET;
        break;
    case LogLevel::fatal:
        ss << COLOUR_FATAL << "[ERROR] " << COLOUR_RESET;
        break;
    case LogLevel::none:
    default:
        break;
    }
    ss << message;
    std::cout << ss.str();
    if (newLine)
    {
        std::cout << std::endl;
    }
}