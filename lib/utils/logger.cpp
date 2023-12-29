#include "logger.hpp"

namespace waterwheel::utils {
Logger::Logger(LogLevel level) : level_(level) {
  this->log(LogLevel::kDebug, "Logger created");
}
Logger::~Logger() {}

void Logger::log(LogLevel logLevel, const char *message, ...) {
  if (this->level_ <= logLevel) {
    FILE *file = stdout;
    switch (logLevel) {
      case LogLevel::kDebug:
        printf("%s[DEBUG]%s ", kColourFormatDebug, kColourFormatReset);
        break;
      case LogLevel::kInfo:
        printf("%s[INFO]%s ", kColourFormatInfo, kColourFormatReset);
        break;
      case LogLevel::kWarning:
        printf("%s[WARNING]%s ", kColourFormatWarning, kColourFormatReset);
        break;
      case LogLevel::kFatal:
        printf("%s[ERROR]%s ", kColourFormatFatal, kColourFormatReset);
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
}  // namespace waterwheel::utils
