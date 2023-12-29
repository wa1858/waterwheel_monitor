#pragma once

#include <stdarg.h>

#include <iostream>
#include <sstream>

namespace waterwheel::utils {
const static char *kWhitespace = "        ";

/**
 * kDebug: Additional info for debugging (object created, value set, etc)
 * kInfo: Useful info for user (readouts from the meter)
 * kWarning: Non-fatal errors for user (meter reading failed)
 * kfatal: Error which stops the program (serial failed to open)
 */
enum class LogLevel { kNone = 0, kDebug, kInfo, kWarning, kFatal };

class Logger {
 public:
  Logger(LogLevel level = LogLevel::kInfo);
  ~Logger();

  // TODO - Modify log() to print coloured text in the middle of an output
  /**
   * @brief Print an output string in a set colour as defined by the log level
   */
  void log(LogLevel logLevel, const char *message, ...);

 private:
  LogLevel level_;
  /**
   * Debug: Green text, black background
   * Info: Blue text, black background
   * Warning: Yellow text, black background
   * Fatal: Red text, black background
   */
  const char *kColourFormatDebug = "\x1b[32m";
  const char *kColourFormatInfo = "\x1b[34m";
  const char *kColourFormatWarning = "\x1b[33m";
  const char *kColourFormatFatal = "\x1b[31m";
  const char *kColourFormatReset = "\x1b[0m";
};
}  // namespace waterwheel::utils
