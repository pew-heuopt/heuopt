#include "timer.h"


timer::timer(int _threshold)
: threshold(_threshold)
{
}

#include <iostream>
bool timer::over_thresold()
{
    auto timestruct= cpu_time.elapsed();

    return threshold < ( (timestruct.user+timestruct.system) / 1000000000 );
}


