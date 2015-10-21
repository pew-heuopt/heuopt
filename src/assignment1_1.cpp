#include <iostream>
#include <memory>
#include <fstream>
#include <algorithm>


#include <assignment_helpers.h>

#include <kpmp_instance.h>
#include <kpmp_solution.h>


//#define DEBUG 1

void insert_on_next_crossfree_page(solution & sol, const edge_t & e )
{

    int num_pages= sol.get_pages();

#ifdef DEBUG
    std::cout << "trying edge: " << e.first << " " << e.second << std::endl;
#endif // DEBUG 

    int crossings;
    int min_crossings;
    int min_index;

    for( int i=0; i<num_pages; ++i )
    {
        crossings= sol.try_num_crossing(i, e);

        if( crossings == 0  )
        {
            sol.add_edge( i, e, crossings );
#ifdef DEBUG
            std::cout << "    0 crossing found inserting on page " << i << std::endl;
#endif // DEBUG
            return;
        }

#ifdef DEBUG
        std::cout << "    crossings on page " << i << ": " << crossings << std::endl;
#endif // DEBUG
        if( i == 0 || 
            crossings < min_crossings)
        {
            min_crossings= crossings;
            min_index= i;
        }



    }

#ifdef DEBUG
    std::cout << "    no zero found, inserting with " << min_crossings << " on page " << min_index << std::endl ;
#endif // DEBUG

    sol.add_edge( min_index, e, min_crossings );

}




//
// construction spine order
//


std::vector<vertex_t> spine_order_ascending( unsigned int num_vertices )
{
    std::vector<vertex_t> spine_order( num_vertices );

    for(unsigned int i= 0; i< num_vertices; ++i )
        spine_order[i]= i;

    return spine_order;
}


std::vector<vertex_t> spine_order_num_edges( const vector<vector<unsigned int> > & adjacencyList )
{
    std::vector<vertex_t> spine_order( spine_order_ascending(adjacencyList.size() ) );

    // this sort predicate just uses the number 
    // of neighbors of a vertex
    struct sort_pred_num_neighbors
    {
        sort_pred_num_neighbors(const vector<vector<unsigned int> > & _adjacencyList) 
        : adjacencyList(_adjacencyList) {} ;

        bool operator() (unsigned int x1, unsigned int x2) 
        { 
            return adjacencyList[x1].size() < adjacencyList[x2].size(); 
        };

        vector<vector<unsigned int> > adjacencyList;
    };

    std::sort( spine_order.begin(), spine_order.end(), sort_pred_num_neighbors(adjacencyList) );
    
    return spine_order;
}


// ##############################################
// MAIN
// ##############################################
int main( int argc, char **argv)
{

    // Declare the supported options.
    boost::program_options::options_description desc("Allowed options");
    boost::program_options::variables_map vm;

    std::string input_filename;
    std::string output_filename;

    std::string spine_order_opt_str;

    desc.add_options()
         ("spine-order", boost::program_options::value<std::string>(&spine_order_opt_str), "spine order: ascend|sorted default: sorted")
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
    

    // read specified problem instance 
    std::auto_ptr<KPMPInstance> instance( KPMPInstance::readInstance( input_filename ) );
    
    // open output file
    ofstream outfile(output_filename, ios::out);

    if( !outfile )
    {
        std::cerr << "error opening output file for writing" << std::endl;
        return 1;
    }

   
    
    size_t num_vertices= instance->getNumVertices();

    //
    // spine order
    //


    std::vector<vertex_t> spine_order( spine_order_opt == ASCEND ? spine_order_ascending(num_vertices) :
                                                                   spine_order_num_edges(instance->getAdjacencyList()) );
    
    // create an empty solution, locally optimal (?) spine order
    solution sol(instance->getK(), spine_order);

    //
    // edge order
    //

    // arbitrary insertion of edges
    bool ** am= instance->getAdjacencyMatrix() ;

    for( unsigned int i= 0; i<num_vertices; ++i )
        for( unsigned int j= i+1; j<num_vertices; ++j )
            if( am[i][j] )
            {
                insert_on_next_crossfree_page(sol, edge_t(i,j) );
            }
        

    //
    // output
    //
    write_solution( outfile, sol );
	outfile.close();

    write_solution_statistics( std::cout, sol );

    return 0;
}
