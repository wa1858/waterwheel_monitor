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
   * @brief Generate two log files on disk: An all-time file for appending, and
   * a rolling file to hold the last ~2 hours of data
   */
  void createLogFiles();

  /**
   * @brief Upon creation of a new log file, write the record format to the top
   * of the file (for use by records writer and by log parser)
   */
  void addLogHeader(FILE *file);

 private:
  LogLevel level_;
  FILE *log_file_all_time = nullptr;
  FILE *log_file_rolling = nullptr;

  const std::string kLogFileDirectory = "logs";
  const std::string kLogFileType = ".txt";
  const std::string kLogFileRolling = "log_rolling" + kLogFileType;
  const std::string kLogFileAllTime = "log_all_time" + kLogFileType;

  // Constant used to pad space in log record format
  const std::string kLogFileHeaderSpare = "spare";

  /**
   * Format of records in log file:
   * - Timestamp
   * - Log level
   * - All measurements taken (with spare slots for future expansion)
   */
  const std::array<std::string, 20> kLogFileRecordsFormat = {
      "time",
      "level",
      "freq",
      "freq_avg",
      "active_power",
      "active_power_avg",
      "total_energy",
      "voltage",
      "current",
      "reactive_power",
      "apparent_power",
      "power_factor",
      "phase_angle",
      kLogFileHeaderSpare,
      kLogFileHeaderSpare,
      kLogFileHeaderSpare,
      kLogFileHeaderSpare,
      kLogFileHeaderSpare,
      kLogFileHeaderSpare,
      kLogFileHeaderSpare};

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
