#pragma once

#include <time.h>

namespace waterwheel::utils
{
    static const int defaultDelay = 1800;

    /**
     * Create time delay as required
     * @param milli_seconds Length of delay required
     */
    void delay(int milli_seconds);
}
