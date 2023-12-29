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

// TODO - Refactor required
void Logger::log(LogLevel logLevel, const char *message, ...) {
  if (this->level_ <= logLevel) {
    std::stringstream ss_terminal;
    std::stringstream ss_log_file;
    std::string time = getTimeStamp();
    ss_terminal << "[" << time << "] ";
    ss_log_file << "[" << time << "] ";

    FILE *file = stdout;
    switch (logLevel) {
      case LogLevel::kDebug:
        ss_terminal << kColourFormatDebug << "[DEBUG]   " << kColourFormatReset
                    << " ";
        ss_log_file << "[DEBUG]    ";
        break;
      case LogLevel::kInfo:
        ss_terminal << kColourFormatInfo << "[INFO]    " << kColourFormatReset
                    << " ";
        ss_log_file << "[INFO]     ";
        break;
      case LogLevel::kWarning:
        ss_terminal << kColourFormatWarning << "[WARNING] "
                    << kColourFormatReset << " ";
        ss_log_file << "[WARNING]  ";
        break;
      case LogLevel::kFatal:
        ss_terminal << kColourFormatFatal << "[ERROR]   " << kColourFormatReset
                    << " ";
        ss_log_file << "[ERROR]    ";
        break;
      case LogLevel::kNone:
      default:
        break;
    }

    ss_terminal << message;
    ss_log_file << message;
    std::string output_terminal = ss_terminal.str();
    std::string output_log_file = ss_log_file.str();

    va_list args;
    va_start(args, message);
    vfprintf(file, output_terminal.c_str(), args);
    fprintf(file, "\n");
    va_end(args);

    // TODO - Why do some values print as -0.0, instead of 0.0
    if (log_file_ != nullptr) {
      va_list args;
      va_start(args, message);
      vfprintf(log_file_, output_log_file.c_str(), args);
      fprintf(log_file_, "\n");
      va_end(args);
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
  // log_file_.open(log_file_path);
  log_file_ = fopen(log_file_path.c_str(), "w");
  if (log_file_ == nullptr) {
    this->log(LogLevel::kWarning, "Unable to create log file on disk");
    return;
  }
  this->log(LogLevel::kDebug, "Log File Opened: %s", log_file_path.c_str());
}

}  // namespace waterwheel::utils
