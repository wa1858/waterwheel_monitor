#include "logger.hpp"

namespace waterwheel::core {
Logger::Logger(LogLevel level) : level_(level) {
  this->log(LogLevel::kDebug, "Logger created");
}
Logger::~Logger() { this->log(LogLevel::kDebug, "Logger destroyed"); }

// TODO - Print to the millisecond
std::string Logger::getTimeStamp() {
  auto now = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(now);

  std::stringstream ss;
  ss << std::put_time(std::localtime(&in_time_t), "%Y/%m/%d %X");
  return ss.str();
}

void Logger::log(LogLevel logLevel, const char *message, ...) {
  if (this->level_ <= logLevel) {
    std::cout << "[" << getTimeStamp() << "] ";
    FILE *file = stdout;
    switch (logLevel) {
      case LogLevel::kDebug:
        printf("%s[DEBUG]   %s ", kColourFormatDebug, kColourFormatReset);
        break;
      case LogLevel::kInfo:
        printf("%s[INFO]    %s ", kColourFormatInfo, kColourFormatReset);
        break;
      case LogLevel::kWarning:
        printf("%s[WARNING] %s ", kColourFormatWarning, kColourFormatReset);
        break;
      case LogLevel::kFatal:
        printf("%s[ERROR]   %s ", kColourFormatFatal, kColourFormatReset);
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
  }
}
}  // namespace waterwheel::core
