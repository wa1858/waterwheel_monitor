#include "utils.hpp"

namespace waterwheel::utils
{
    void delay(int milli_seconds)
    {
        // Storing start time
        clock_t start_time = clock();

        // Loop for required time
        while (clock() < start_time + milli_seconds)
            ;
    }
}
