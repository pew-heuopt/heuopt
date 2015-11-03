#ifndef _STEPFUNCTIONS_H_
#define _STEPFUNCTIONS_H_

#include <stdexcept>
#include <cstdlib>

#include "timer.h"

#include "kpmp_solution.h"

namespace stepfunction
{

template<typename T>
solution first_improvement( T begin,
                            T end, const timer & time  )
{

    if( begin == end )
        throw std::runtime_error( "empty neighborhood iterator");

    auto iterator= begin;

    solution best= *iterator;

    for( ; iterator != end; ++iterator )
    {
        if( time.over_thresold() )
            break;

        solution sol=  *iterator;
        if( sol.get_crossings() < best.get_crossings() )
        {
            best= sol;
            break;
        }
    }

    return best;
}


template<typename T>
solution best_improvement( T begin,
                           T end, const timer & time  )
{

    if( begin == end )
        throw std::runtime_error( "empty neighborhood iterator");

    auto iterator= begin;

    solution best= *iterator;

    for( ; iterator != end; ++iterator )
    {
        if( time.over_thresold() )
            break;

        solution sol=  *iterator;

        if( sol.get_crossings() < best.get_crossings() )
        {
            best= sol;
        }
    }

    return best;
}



template<typename T>
solution random( T begin,
                 T end,
                 int max_rand, const timer & timeout )
{

    if( begin == end )
        throw std::runtime_error( "empty neighborhood iterator");


    auto iterator= begin;

    srand ( time(NULL) );

    int random_num= rand() % max_rand;
    int cnt= 0;

    for( ; cnt<random_num && iterator != end; 
           ++iterator, ++cnt )
    {
    }

    if( iterator == end )
        throw std::out_of_range("random number longer than iterator");

    return *iterator;
}


}

#endif // _STEPFUNCTIONS_H_
