#include "logger.hpp"

namespace waterwheel::utils
{
    Logger::Logger(LogLevel level) : level(level)
    {
        this->log(LogLevel::debug, "Logger created");
    }
    Logger::~Logger()
    {
        this->log(LogLevel::debug, "Logger destroyed");
    }

    std::string Logger::getTimeStamp()
    {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);

        std::stringstream ss;
        ss << std::put_time(std::localtime(&in_time_t), "%Y/%m/%d %X");
        return ss.str();
    }

    void Logger::log(LogLevel logLevel, const char *message, ...)
    {
        if (this->level <= logLevel)
        {
            std::cout << "[" << getTimeStamp() << "] ";
            FILE *file = stdout;
            switch (logLevel)
            {
            case LogLevel::debug:
                printf("%s[DEBUG]   %s ", COLOUR_DEBUG, COLOUR_RESET);
                break;
            case LogLevel::info:
                printf("%s[INFO]    %s ", COLOUR_INFO, COLOUR_RESET);
                break;
            case LogLevel::warning:
                printf("%s[WARNING] %s ", COLOUR_WARNING, COLOUR_RESET);
                break;
            case LogLevel::fatal:
                printf("%s[ERROR]   %s ", COLOUR_FATAL, COLOUR_RESET);
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
    }
}
