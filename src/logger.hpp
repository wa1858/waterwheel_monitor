#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <sstream>

const static std::string WHITESPACE = "        ";
const static bool NO_NEWLINE = false;

// TODO - Modify Logger to print coloured text in the middle of an output
enum class LogLevel
{
    none,
    info,
    warning,
    fatal
};

class Logger
{
public:
    Logger();
    ~Logger();
    /**
     * Print an output string in a set colour as
     * defined by the log level
     * @param logLevel  Logging level for message
     * @param message   Message to be printed
     * @param newLine   True if new line should be printed
     */
    void log(LogLevel logLevel, std::string message, bool newLine = true);

private:
    /**
     * Colour scheme:
     * Info: Blue text, black background
     * Warning: Yellow text, black background
     * Fatal: Red text, black background
     */
    const std::string COLOUR_INFO = "\x1b[34m";
    const std::string COLOUR_WARNING = "\x1b[33m";
    const std::string COLOUR_FATAL = "\x1b[31m";
    const std::string COLOUR_RESET = "\x1b[0m";
};

#endif