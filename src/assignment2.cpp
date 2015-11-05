#include <iostream>
#include <memory>
#include <fstream>
#include <algorithm>
#include <vector>

#include <boost/timer/timer.hpp>

#include <assignment_helpers.h>

#include <kpmp_instance.h>
#include <kpmp_solution.h>

#include <spine.h>

#include <neighborhood_1_edge_move.h>
#include <neighborhood_1_node_flip.h>
#include <neighborhood_node_edge_move.h>
#include <neighborhood_1_node_edge_move.h>

#include <time.h>
#include <stepfunctions.h>


enum step_func_t { FIRST, BEST, RANDOM };
enum neighborhood_t { NODE_1, EDGE_1, NODE_EDGE, NODE_EDGE_1 };



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
	  return execute_step( neighborhood_node_edge_move_begin(sol),
                               neighborhood_node_edge_move_end(sol),
                               step_func, num_vertices * (num_pages-1), time);

        case NODE_EDGE_1:

    	  return execute_step( neighborhood_node_edge_move_begin(sol),
                               neighborhood_node_edge_move_end(sol),
                               step_func, 

                               // actually this is not true ;) there would be min max
                               num_vertices * (num_pages-1), 

                               time); 


        default:
                   throw std::runtime_error("unsupported stepfunction");

    };
}



int main( int argc, char **argv)
{

    // Declare the supported options.
    boost::program_options::options_description desc("Allowed options");
    boost::program_options::variables_map vm;

    std::string input_filename;
    std::string output_filename;

    std::string step_func_str;
    std::string neighborhood_str;
    int timeout= 60 * 5;

    // use boost graph library to calc connected components

    desc.add_options()

         ("timeout", boost::program_options::value<int>(&timeout), 
 	                  "timeout of computation in seconds (default 300seconds = 5 minutes")

         ("neighborhood", boost::program_options::value<std::string>(&neighborhood_str), 
 	                   "stepping function: 1-node | 1-edge | node-edge | 1-node-edge")


         ("step-func", boost::program_options::value<std::string>(&step_func_str), 
 	                   "stepping function: first|best|random default: first")
             ;

    if( common_cmdline( argc, argv,
                    desc, vm, 
                    input_filename, output_filename ) )
        return 1;


    
    if( !vm.count("neighborhood") )
    {
        std::cerr << "neighborhood not given" << std::endl 
                  << desc << std::endl ;
        return 1;
    }


    neighborhood_t neighborhood;
    {
        if( neighborhood_str == "1-node" )
            neighborhood= NODE_1;
        else if( neighborhood_str == "1-edge" )
            neighborhood= EDGE_1;
	    else if( neighborhood_str == "node-edge" )
	        neighborhood= NODE_EDGE;
	    else if( neighborhood_str == "1-node-edge" )
	        neighborhood= NODE_EDGE_1;
        else 
        {
            std::cerr << "unkown neighborhood" << std::endl
                      << desc << std::endl;
            return 1;
        }
    }

    step_func_t step_func= FIRST;
    {
        if( vm.count("step-func") )
        {
            if( step_func_str == "first" )
                step_func= FIRST;
            else if( step_func_str == "best" )
                step_func= BEST;
            else if( step_func_str == "random" )
                step_func= RANDOM;
            else 
            {
                std::cerr << "unkown step function" << std::endl
                          << desc << std::endl;
                return 1;
            }
        }
    } 

        

    //
    // instance input
    //
    std::auto_ptr<KPMPInstance> instance( KPMPInstance::readInstance( input_filename ) );
    
    // open output file
    ofstream outfile(output_filename, ios::out);

    if( !outfile )
    {
        std::cerr << "error opening output file for writing" << std::endl;
        return 1;
    }

   
    
    size_t n_vertices= instance->getNumVertices();

    //
    // spine order
    //

    // arbitrary insertion of edges
    bool ** am= instance->getAdjacencyMatrix() ;



    // now we have more than 2 options
    std::vector<vertex_t> spine_order( spine_order_num_edges(instance->getAdjacencyList()) );
    
    solution sol(instance->getK(), spine_order);
    //
    // edge page allication
    //
    for( unsigned int i= 0; i<n_vertices; ++i )
        for( unsigned int j= i+1; j<n_vertices; ++j )
            if( am[i][j] )
            {
                insert_on_next_crossfree_page(sol, edge_t(i,j) );
            }
        

    // start the timer
    timer time(timeout);


    // local search
    //
    solution best= sol;
    while(1)
    {
        solution local_search_sol= execute_neighborhood( best, neighborhood, step_func, time );

        if( best.get_crossings() > local_search_sol.get_crossings() ) 
        {
	        cout << "improvement found";
            best= local_search_sol;
	    }
        if( time.over_thresold() )
        {
            break;
        }
    }


    //
    // output
    //
    write_solution( outfile, best );
	outfile.close();

    write_solution_statistics( std::cout, best );

    return 0;
}
