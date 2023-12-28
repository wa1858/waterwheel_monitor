#include "debug.hpp"

namespace waterwheel::utils
{
    void debugConfigureDelay(Logger &logger, int *delay)
    {
        logger.log(LogLevel::debug, "Enter the desired delay in milliseconds (Note default is %dms): ", defaultDelay);
        // TODO - Imput validation
        std::cin >> *delay;
        logger.log(LogLevel::debug, "Delay set to %dms", *delay);
    }
}
