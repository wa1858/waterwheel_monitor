#pragma once

#include <stdarg.h>
#include <iostream>
#include <sstream>

const static char *WHITESPACE = "        ";

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
     */
    void log(LogLevel logLevel, const char *message, ...);

private:
    /**
     * Colour scheme:
     * Info: Blue text, black background
     * Warning: Yellow text, black background
     * Fatal: Red text, black background
     */
    const char *COLOUR_INFO = "\x1b[34m";
    const char *COLOUR_WARNING = "\x1b[33m";
    const char *COLOUR_FATAL = "\x1b[31m";
    const char *COLOUR_RESET = "\x1b[0m";
};
