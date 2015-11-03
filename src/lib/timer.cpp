#include "timer.h"


timer::timer(int _threshold)
: threshold(_threshold)
{
}

#include <iostream>
bool timer::over_thresold()
{
    auto timestruct= cpu_time.elapsed();

    std::cout << timestruct.user+timestruct.system << std::endl;
    std::cout << ( (timestruct.user+timestruct.system) / 1000000000 ) << std::endl;

    return threshold < ( (timestruct.user+timestruct.system) / 1000000000 );
}


