#include "logger.hpp"

namespace waterwheel::utils {
Logger::Logger(LogLevel level) : level_(level) {
  this->createLogFile();
  this->log(LogLevel::kDebug, "Logger created");
}
Logger::~Logger() {
  this->log(LogLevel::kDebug, "Logger destroyed");
  this->log_file_.close();
}

void Logger::log(LogLevel logLevel, const char *message, ...) {
  if (this->level_ <= logLevel) {
    std::cout << "[" << getTimeStamp() << "] ";
    FILE *file = stdout;
    switch (logLevel) {
      case LogLevel::kDebug:
        printf("%s[DEBUG]   %s ", kColourFormatDebug.c_str(),
               kColourFormatReset.c_str());
        break;
      case LogLevel::kInfo:
        printf("%s[INFO]    %s ", kColourFormatInfo.c_str(),
               kColourFormatReset.c_str());
        break;
      case LogLevel::kWarning:
        printf("%s[WARNING] %s ", kColourFormatWarning.c_str(),
               kColourFormatReset.c_str());
        break;
      case LogLevel::kFatal:
        printf("%s[ERROR]   %s ", kColourFormatFatal.c_str(),
               kColourFormatReset.c_str());
        break;
      case LogLevel::kNone:
      default:
        break;
    }

    va_list args;
    va_start(args, message);
    vfprintf(file, message, args);
    fprintf(file, "\n");
    va_end(args);

    if (log_file_.is_open()) {
      logToDisk();
    }
  }
}

// TODO - Print to the millisecond
std::string Logger::getTimeStamp() {
  auto now = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(now);

  std::stringstream ss;
  ss << std::put_time(std::localtime(&in_time_t), "%Y/%m/%d %X");
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
  // TODO - Open a file without overwriting it if it already exists
  log_file_.open(log_file_path);
  if (!log_file_.is_open()) {
    this->log(LogLevel::kWarning, "Unable to create log file on disk");
    return;
  }
  this->log(LogLevel::kDebug, "Log File Opened: %s", log_file_path.c_str());
}

// TODO - Implement fully
void Logger::logToDisk() {
  log_file_ << "[" << getTimeStamp() << "] "
            << "LEVEL"
            << " DEMO PRINT" << std::endl;
}

}  // namespace waterwheel::utils
