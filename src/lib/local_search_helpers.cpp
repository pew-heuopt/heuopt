#include "local_search_helpers.h"


#include "neighborhood_1_edge_move.h"
#include "neighborhood_1_node_flip.h"
#include "neighborhood_1_node_move.h"
#include "neighborhood_x_node_move_random.h"
#include "neighborhood_node_edge_shift.h"
#include "neighborhood_1_node_edge_move.h"




solution execute_neighborhood( const solution & sol, neighborhood_t neighborhood, step_func_t step_func, const timer & time )
{
    int num_vertices= sol.get_spine_order().size();
    int num_edges= sol.get_num_edges();
    int num_pages= sol.get_pages();

    switch( neighborhood )
    {
        case NODE_1: 

            return execute_step( neighborhood_1_node_flip_begin(sol),
                                 neighborhood_1_node_flip_end(sol),
                                 step_func, num_vertices * (num_vertices-1) / 2, 
                                 time  );

        case EDGE_1: 

            return execute_step( neighborhood_1_edge_move_begin(sol),
                                 neighborhood_1_edge_move_end(sol),
                                 step_func, (num_pages-1)*num_edges,
                                 time  );
	    
        case NODE_EDGE:
	  // for each node, we can move each edge only (num_pages-1) times.
	  return execute_step( neighborhood_node_edge_shift_begin(sol),
                               neighborhood_node_edge_shift_end(sol),
                               step_func, num_vertices * (num_pages-1), time);

        case NODE_EDGE_1:

    	  return execute_step( neighborhood_1_node_edge_move_begin(sol),
                               neighborhood_1_node_edge_move_end(sol),
                               step_func, 

                               // actually this is not true ;) there would be min max
                               num_vertices * (num_pages-1), 

                               time); 

        case NODE_MOVE_1: 

            return execute_step( neighborhood_1_node_move_begin(sol),
                                 neighborhood_1_node_move_end(sol),
                                 step_func, (num_vertices-1) * (num_vertices-1), 
                                 time  );

        case NODE_MOVE_RANDOM_2: 

            return execute_step( neighborhood_x_node_move_random_begin(sol,2),
                                 neighborhood_x_node_move_random_end(sol,2),
                                 step_func,1, 
                                 time  );

        case NODE_MOVE_RANDOM_3: 

            return execute_step( neighborhood_x_node_move_random_begin(sol,3),
                                 neighborhood_x_node_move_random_end(sol,3),
                                 step_func,1, 
                                 time  );


        default:
                   throw std::runtime_error("unsupported stepfunction");

    };
}



void vnd( solution & best, 
          const std::list<neighborhood_t> & deterministic_neighborhoods,
          const step_func_t & step_func,
          const timer & time )
{
    auto neighborhood_it= deterministic_neighborhoods.begin();

    while( neighborhood_it != deterministic_neighborhoods.end() )
    {
        solution sol= execute_neighborhood( best, *neighborhood_it, step_func, time );

        if( best.get_crossings() > sol.get_crossings() ) 
        {
            neighborhood_it= deterministic_neighborhoods.begin();

#ifdef DEBUG            
            std::cout << "DETERMINISTIC NEIGHBORHOODS: found improvement" << std::endl;
#endif // DEBUG            
            best= sol;
            neighborhood_it= deterministic_neighborhoods.begin();
	    }
        else
        {

            neighborhood_it++;
#ifdef DEBUG            
            std::cout << "DETERMINISTIC NEIGHBORHOODS: no improvement" << std::endl;
#endif // DEBUG            
        }


        if( time.over_thresold() )
        {
            std::cout << "timeout: 1" << std::endl;
            break;
        }
    } 
}

void general_vns( solution & best, 
                  const std::list<neighborhood_t> & stochastic_neighborhoods, 
                  const std::list<neighborhood_t> & deterministic_neighborhoods, 
                  const step_func_t & vnd_step_func,
                  const timer & time )
{
 
    auto neighborhood_it= stochastic_neighborhoods.begin();

    while( neighborhood_it != stochastic_neighborhoods.end() )
    {
        solution sol= execute_neighborhood( best, *neighborhood_it, RANDOM, time );


        vnd( sol, deterministic_neighborhoods, vnd_step_func, time );

        if( best.get_crossings() > sol.get_crossings() ) 
        {
            neighborhood_it= stochastic_neighborhoods.begin();

#ifdef DEBUG            
            std::cout << "STOCHASTIC NEIGHBORHOODS: found improvement" << std::endl;
#endif // DEBUG            
            best= sol;
	    }
        else
        {

            neighborhood_it++;
#ifdef DEBUG            
            std::cout << "STOCHASTIC NEIGHBORHOODS: no improvement" << std::endl;
#endif // DEBUG            
        }


        if( time.over_thresold() )
        {
            std::cout << "timeout: 1" << std::endl;
            break;
        }
    } 
   

}


