#include <iostream>
#include <memory>
#include <fstream>
#include <algorithm>
#include <vector>
#include <sstream>

#include <boost/timer/timer.hpp>

#include <assignment_helpers.h>

#include <kpmp_instance.h>
#include <kpmp_solution.h>

#include <spine.h>

#include <neighborhood_1_edge_move.h>
#include <neighborhood_1_node_flip.h>
#include <neighborhood_1_node_move.h>
#include <neighborhood_x_node_move_random.h>
#include <neighborhood_node_edge_shift.h>
#include <neighborhood_1_node_edge_move.h>

#include <time.h>
#include <stepfunctions.h>


// #define DEBUG 1

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



void deterministic_search( solution & best, 
                           const std::list<neighborhood_t> & deterministic_neighborhoods,
                           const timer & time )
{
    auto neighborhood_it= deterministic_neighborhoods.begin();

    while( neighborhood_it != deterministic_neighborhoods.end() )
    {
        solution sol= execute_neighborhood( best, *neighborhood_it, BEST, time );

        if( best.get_crossings() > sol.get_crossings() ) 
        {
            neighborhood_it= deterministic_neighborhoods.begin();

#ifdef DEBUG            
            std::cout << "DETERMINISTIC NEIGHBORHOODS: found improvement" << std::endl;
#endif // DEBUG            
            best= sol;
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
                  const timer & time )
{
 
    auto neighborhood_it= stochastic_neighborhoods.begin();

    while( neighborhood_it != stochastic_neighborhoods.end() )
    {
        solution sol= execute_neighborhood( best, *neighborhood_it, RANDOM, time );


        deterministic_search( sol, deterministic_neighborhoods, time );

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



neighborhood_t str_2_neighborhood( const std::string & neighborhood_str )
{
    if( neighborhood_str == "1-node" )
        return NODE_1;
    else if( neighborhood_str == "1-edge" )
        return EDGE_1;
    else if( neighborhood_str == "node-edge" )
        return NODE_EDGE;
    else if( neighborhood_str == "1-node-edge" )
        return NODE_EDGE_1;
    else if( neighborhood_str == "1-node-move" )
        return NODE_MOVE_1;
    else if( neighborhood_str == "2-node-move" )
        return NODE_MOVE_RANDOM_2;
    else if( neighborhood_str == "3-node-move" )
        return NODE_MOVE_RANDOM_3;

    std::stringstream ss;
    ss << "unkown neighborhood " << neighborhood_str ;

    throw std::runtime_error( ss.str() );
}


void str_2_neighborhood( const std::vector<std::string> & neighborhood_strs, std::list<neighborhood_t> & neighborhoods )
{
    for( auto i= neighborhood_strs.begin(); i!=neighborhood_strs.end(); ++i )
        neighborhoods.push_back( str_2_neighborhood( *i ) );

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
    std::string spine_order_opt_str;

    // use boost graph library to calc connected components


    desc.add_options()

         ("timeout", boost::program_options::value<int>(&timeout), 
 	                  "timeout of computation in seconds (default 300seconds = 5 minutes")

         ("spine-order", boost::program_options::value<std::string>(&spine_order_opt_str), 
                       "spine order: ascend|sorted|component default: sorted")

         ("stochastic-neighborhoods", boost::program_options::value<std::vector<std::string> >()->multitoken(), "stochastic neighborhoods to execute")

         ("deterministic-neighborhoods", boost::program_options::value<std::vector<std::string> >()->multitoken(), "deterministic neighborhoods to execute")

             ;

    if( common_cmdline( argc, argv,
                    desc, vm, 
                    input_filename, output_filename ) )
        return 1;


    


    enum spine_order_opt_t { SORTED, ASCEND };
    spine_order_opt_t spine_order_opt= SORTED;
    if( vm.count("spine-order") )
    {
        if( spine_order_opt_str == "sorted" )
            spine_order_opt= SORTED;
        else if( spine_order_opt_str == "ascend" )
            spine_order_opt= ASCEND;
	    else 
        {
            std::cerr << "unkown spine-order" << std::endl
                      << desc << std::endl;
            return 1;
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



   
    if( vm["stochastic-neighborhoods"].empty() || (vm["stochastic-neighborhoods"].as<vector<std::string> >()).size() == 0 )
    {
        std::cerr << "no stochastic neighborhoods given" << std::endl;
        return 1;
    }

    if( vm["deterministic-neighborhoods"].empty() || (vm["deterministic-neighborhoods"].as<vector<std::string> >()).size() == 0 )
    {
        std::cerr << "no deterministic neighborhoods given" << std::endl;
        return 1;
    }


    
    size_t n_vertices= instance->getNumVertices();

    //
    // spine order
    //

    // arbitrary insertion of edges
    bool ** am= instance->getAdjacencyMatrix() ;



    // now we have more than 2 options
    // we use the ascending as random alternative
    std::vector<vertex_t> spine_order( spine_order_opt == ASCEND ? spine_order_ascending(n_vertices) :
                                                                   spine_order_num_edges(instance->getAdjacencyList()) );
    
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


    //std::list<neighborhood_t> stochastic_neighborhoods { NODE_MOVE_1, NODE_MOVE_RANDOM_2, NODE_MOVE_RANDOM_3 };
    std::list<neighborhood_t> stochastic_neighborhoods;

    str_2_neighborhood( vm["stochastic-neighborhoods"].as<vector<std::string> >(), stochastic_neighborhoods );

    //std::list<neighborhood_t> deterministic_neighborhoods { EDGE_1, NODE_EDGE_1 };
    std::list<neighborhood_t> deterministic_neighborhoods;
    str_2_neighborhood( vm["deterministic-neighborhoods"].as<vector<std::string> >(), deterministic_neighborhoods );

    general_vns( sol, stochastic_neighborhoods, deterministic_neighborhoods, time );


    //
    // output
    //
    write_solution( outfile, sol );
	outfile.close();

    write_solution_statistics( std::cout, sol );

    return 0;
}
