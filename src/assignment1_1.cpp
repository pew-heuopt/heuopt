#include <iostream>
#include <memory>
#include <fstream>
#include <algorithm>


#include <assignment_helpers.h>

#include <kpmp_instance.h>
#include <kpmp_solution.h>
#include <spine.h>




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


        

    //
    // instance input
    //
    std::auto_ptr<KPMPInstance> instance( KPMPInstance::readInstance( input_filename ) );

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
    
    solution sol(instance->getK(), spine_order);

    //
    // edge page allication
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
