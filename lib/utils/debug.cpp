#include "debug.hpp"

namespace waterwheel::utils {
void debugConfigureDelay(Logger &logger, int *delay) {
  std::cout << "Enter the desired delay in milliseconds (Note default is "
            << kDefaultDelay << "ms): ";
  // TODO - Imput validation
  std::cin >> *delay;
  logger.log(LogLevel::kDebug, "Delay set to %dms", *delay);
}
}  // namespace waterwheel::utils
