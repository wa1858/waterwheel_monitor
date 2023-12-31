#include "logger.hpp"

namespace waterwheel::core {
Logger::Logger(LogLevel level) : level_(level) {
  createLogFiles();
  log(LogLevel::kDebug, "Logger created");
}
Logger::~Logger() {
  log(LogLevel::kDebug, "Logger destroyed");
  fclose(log_file_all_time);
}

void Logger::log(LogLevel logLevel, const char *message, ...) {
  if (level_ <= logLevel) {
    std::string time = "[" + getTimeStamp() + "] ";
    std::string head, head_coloured;

    FILE *file = stdout;
    switch (logLevel) {
      case LogLevel::kDebug:
        head = "[DEBUG]    ";
        head_coloured = kColourFormatDebug + head + kColourFormatReset;
        break;
      case LogLevel::kInfo:
        head = "[INFO]     ";
        head_coloured = kColourFormatInfo + head + kColourFormatReset;
        break;
      case LogLevel::kWarning:
        head = "[WARNING]  ";
        head_coloured = kColourFormatWarning + head + kColourFormatReset;
        break;
      case LogLevel::kFatal:
        head = "[ERROR]    ";
        head_coloured = kColourFormatFatal + head + kColourFormatReset;
        break;
      case LogLevel::kNone:
      default:
        break;
    }
    std::string output = time + head_coloured + message;

    va_list args;
    va_start(args, message);
    vfprintf(file, output.c_str(), args);
    fprintf(file, "\n");
    va_end(args);

    // TODO - Format logs for saving to disk as a record
    // TODO - Write logs to files
    if (log_file_all_time != nullptr) {
      output = time + head + message;
      va_list args;
      va_start(args, message);
      vfprintf(log_file_all_time, output.c_str(), args);
      fprintf(log_file_all_time, "\n");
      va_end(args);
    }
  }
}

std::string Logger::getTimeStamp() {
  auto currentTime = std::chrono::system_clock::now();
  auto currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
  auto *timeInfo = std::localtime(&currentTime_t);

  // Total number of milliseconds in time % 1000 = milliseconds of timestamp
  auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
                          currentTime.time_since_epoch())
                          .count() %
                      1000;

  // Format the time as a string
  std::ostringstream ss;
  ss << std::put_time(timeInfo, "%Y/%m/%d %H:%M:%S");
  ss << "." << std::setfill('0') << std::setw(3) << milliseconds;
  return ss.str();
}

void Logger::createLogFiles() {
  if (!std::filesystem::exists(kLogFileDirectory)) {
    std::filesystem::create_directory(kLogFileDirectory);
    log(LogLevel::kDebug, "Log file directory created at .\\%s",
        kLogFileDirectory.c_str());
  }

  // TODO - Logs only being saved after std::cin - Needs fixing
  std::string path_log_file_all_time =
      kLogFileDirectory + "/" + kLogFileAllTime;
  // Open the all time file for appending
  log_file_all_time = fopen(path_log_file_all_time.c_str(), "a");
  if (log_file_all_time == nullptr) {
    log(LogLevel::kWarning, "Unable to open %s",
        path_log_file_all_time.c_str());
    return;
  }

  // Check if file is empty and needs header applied
  fseek(log_file_all_time, 0, SEEK_END);
  uint8_t size = ftell(log_file_all_time);
  if (size == 0) {
    addLogHeader(log_file_all_time);
  }
  log(LogLevel::kDebug, "Log File Opened: %s", path_log_file_all_time.c_str());

  // TODO - Create and open the rolling log file
}

void Logger::addLogHeader(FILE *file) {
  // Construct the header from format array
  std::string s = "";
  for (int i = 0; i < kLogFileRecordsFormat.size(); ++i) {
    s += kLogFileRecordsFormat[i] + " ";
  }
  // Replace final space with new line
  s[s.length() - 1] = '\n';
  fwrite(s.c_str(), 1, s.length(), file);
}

}  // namespace waterwheel::core
