#pragma once

#include <time.h>

namespace waterwheel::utils {
constexpr static int kDefaultDelay = 1800;

/**
 * @brief Create a time delay as required
 */
void delay(int milli_seconds);
}  // namespace waterwheel::utils
