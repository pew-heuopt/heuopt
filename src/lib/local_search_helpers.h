#ifndef _LOCAL_SEARCH_HELPERS_H_
#define _LOCAL_SEARCH_HELPERS_H_

#include "stepfunctions.h"

enum step_func_t { FIRST, BEST, RANDOM };
enum neighborhood_t { NODE_1, EDGE_1, NODE_EDGE, NODE_EDGE_1, NODE_MOVE_1, NODE_MOVE_RANDOM_2, NODE_MOVE_RANDOM_3};



template<typename T>
solution execute_step( T  begin, T  end, 
                       step_func_t step_func, int max_random, const timer & time )
{
    switch( step_func )
    {
        case FIRST: return stepfunction::first_improvement(begin,end,time);
        case BEST: return stepfunction::best_improvement(begin,end,time);
        case RANDOM: return stepfunction::random(begin,end,max_random,time);

        default:
                   throw std::runtime_error("unsupported stepfunction");


       
    };
}


solution execute_neighborhood( const solution & sol, neighborhood_t neighborhood, step_func_t step_func, const timer & time );

void vnd( solution & best, 
          const std::list<neighborhood_t> & deterministic_neighborhoods,
          const step_func_t & step_func,
          const timer & time );


void general_vns( solution & best, 
                  const std::list<neighborhood_t> & stochastic_neighborhoods, 
                  const std::list<neighborhood_t> & deterministic_neighborhoods, 
                  const step_func_t & vnd_step_func,
                  const timer & time );

#endif // _LOCAL_SEARCH_HELPERS_H_
