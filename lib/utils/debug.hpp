#pragma once

#include <utils/logger.hpp>
#include <utils/utils.hpp>

// TODO Expand the capabilities of debug mode

/* TODO - Allow user to define which requests they want to print (frequency,
 voltage, current etc) */

namespace waterwheel::utils {
constexpr static const char *DEBUG_FLAG = "-d";

/**
 * @brief Allow user to set their own delay time between measurements
 */
void debugConfigureDelay(Logger &logger, int *delay);
}  // namespace waterwheel::utils