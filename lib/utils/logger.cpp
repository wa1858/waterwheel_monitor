#include "logger.hpp"

namespace waterwheel::utils {
Logger::Logger(LogLevel level) : level_(level) {
  this->createLogFile();
  this->log(LogLevel::kDebug, "Logger created");
}
Logger::~Logger() {
  this->log(LogLevel::kDebug, "Logger destroyed");
  fclose(this->log_file_);
}

void Logger::log(LogLevel logLevel, const char *message, ...) {
  if (this->level_ <= logLevel) {
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

    if (log_file_ != nullptr) {
      output = time + head + message;
      va_list args;
      va_start(args, message);
      vfprintf(log_file_, output.c_str(), args);
      fprintf(log_file_, "\n");
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

void Logger::createLogFile() {
  if (!std::filesystem::exists(kLogFileDirectory)) {
    std::filesystem::create_directory(kLogFileDirectory);
    this->log(LogLevel::kDebug, "Log file directory created at .\\%s",
              kLogFileDirectory.c_str());
  }

  // Get the current date for use in the name of the log file
  auto currentTime = std::chrono::system_clock::now();
  auto currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
  auto *currentTm = std::localtime(&currentTime_t);
  std::ostringstream ss;
  ss << std::put_time(currentTm, "%Y%m%d");
  std::string date = ss.str();

  std::string log_file_path =
      kLogFileDirectory + "/" + kLogFilePrefix + date + kLogFileType;
  // Open the file, appending if it already exists
  log_file_ = fopen(log_file_path.c_str(), "a");
  if (log_file_ == nullptr) {
    this->log(LogLevel::kWarning, "Unable to create log file on disk");
    return;
  }
  this->log(LogLevel::kDebug, "Log File Opened: %s", log_file_path.c_str());
}

}  // namespace waterwheel::utils
