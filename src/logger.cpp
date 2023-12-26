#include "logger.hpp"

Logger::Logger()
{
}
Logger::~Logger()
{
}

void Logger::log(LogLevel logLevel, const char *message, ...)
{
    FILE *file = stdout;
    switch (logLevel)
    {
    case LogLevel::info:
        printf("%s[INFO]%s ", COLOUR_INFO, COLOUR_RESET);
        break;
    case LogLevel::warning:
        printf("%s[WARNING]%s ", COLOUR_WARNING, COLOUR_RESET);
        break;
    case LogLevel::fatal:
        printf("%s[ERROR]%s ", COLOUR_FATAL, COLOUR_RESET);
        break;
    case LogLevel::none:
    default:
        break;
    }

    va_list args;
    va_start(args, message);
    vfprintf(file, message, args);
    fprintf(file, "\n");
    va_end(args);
}