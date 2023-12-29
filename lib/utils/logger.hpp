#pragma once

#include <stdarg.h>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace waterwheel::utils {

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
  /**
   * @brief Generate a timestamp to accompany every log output
   */
  std::string getTimeStamp();

  /**
   * @brief Generate a log file for the current day on disk
   */
  void createLogFile();

 private:
  LogLevel level_;
  FILE *log_file_ = nullptr;

  const std::string kLogFileDirectory = "logs";
  const std::string kLogFilePrefix = "log_";
  const std::string kLogFileType = ".txt";

  /**
   * Debug: Green text, black background
   * Info: Blue text, black background
   * Warning: Yellow text, black background
   * Fatal: Red text, black background
   */
  const std::string kColourFormatDebug = "\x1b[32m";
  const std::string kColourFormatInfo = "\x1b[34m";
  const std::string kColourFormatWarning = "\x1b[33m";
  const std::string kColourFormatFatal = "\x1b[31m";
  const std::string kColourFormatReset = "\x1b[0m";
};
}  // namespace waterwheel::utils
