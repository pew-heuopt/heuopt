#ifndef _TIMER_H_
#define _TIMER_H_


#include <boost/timer/timer.hpp>

class timer 
{
public:    

    /** threshold in seconds */
    timer(int _threshold);

    bool over_thresold() const;

private:        

    int threshold;
    boost::timer::cpu_timer cpu_time;
};

#endif // _TIMER_H_
