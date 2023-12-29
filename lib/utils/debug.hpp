#pragma once

#include <utils/logger.hpp>
#include <utils/utils.hpp>

/* TODO - Allow user to define which requests they want to print (frequency,
 voltage, current etc) */

namespace waterwheel::utils {
constexpr static const char *DEBUG_FLAG = "-d";

/**
 * Allow user to set delay between readings from
 * waterwheel monitor
 * @param logger    Logger object
 * @param delay     Delay to be used by program
 */
void debugConfigureDelay(Logger &logger, int *delay);
}  // namespace waterwheel::utils