#include <iostream>
#include <memory>
#include <fstream>
#include <algorithm>
#include <vector>

#include <assignment_helpers.h>

#include <kpmp_instance.h>
#include <kpmp_solution.h>

#include <spine.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/config.hpp>
// ##############################################
// MAIN
// ##############################################
int main( int argc, char **argv)
{

  using namespace boost;
    // Declare the supported options.
    boost::program_options::options_description desc("Allowed options");
    boost::program_options::variables_map vm;

    std::string input_filename;
    std::string output_filename;

    std::string spine_order_opt_str;

    // use boost graph library to calc connected components

    desc.add_options()
         ("spine-order", boost::program_options::value<std::string>(&spine_order_opt_str), 
	  "spine order: ascend|sorted|component default: sorted")
             ;

    if( common_cmdline( argc, argv,
                    desc, vm, 
                    input_filename, output_filename ) )
        return 1;


    enum spine_order_opt_t { SORTED, ASCEND, COMPONENT };
    spine_order_opt_t spine_order_opt= SORTED;
    

    if( vm.count("spine-order") )
    {
        if( spine_order_opt_str == "sorted" )
            spine_order_opt= SORTED;
        else if( spine_order_opt_str == "ascend" )
            spine_order_opt= ASCEND;
        else if( spine_order_opt_str == "component" )
	    spine_order_opt= COMPONENT;
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

   
    
    size_t n_vertices= instance->getNumVertices();

    //
    // spine order
    //

    // arbitrary insertion of edges
    bool ** am= instance->getAdjacencyMatrix() ;


    //std::vector<int> component(num_vertices(G));

    //int num = connected_components(G, &component[0]);

    // cout << "Number of connected components: " << num << endl;

    // determine spine order
    // right now we support 3 options:
    // ascending, sorted and using connected components
    std::vector<vertex_t> spine_order;
    if(spine_order_opt == ASCEND)
      spine_order= spine_order_ascending(n_vertices);
    if(spine_order_opt == SORTED)
      spine_order= spine_order_num_edges(instance->getAdjacencyList());
    if(spine_order_opt == COMPONENT)
      spine_order= spine_order_component(instance->getAdjacencyList(),
					 instance->getAdjacencyMatrix());

    // now we have more than 2 options
    //std::vector<vertex_t> spine_order( spine_order_opt == ASCEND ? spine_order_ascending(n_vertices) :
    //                                                               spine_order_num_edges(instance->getAdjacencyList()) );
    
    // create an empty solution, locally optimal (?) spine order
    solution sol(instance->getK(), spine_order);
    //
    // edge page allication
    //

    // // create list of all edges
    // std::vector<edge_t > unallocatedEdges;

    //  for( unsigned int i=0;i<n_vertices;++i)
    //   for( unsigned int j = i+1; j<n_vertices; ++j)
    // 	if( am[i][j]) // found an edge
    // 	  {
    // 	    unallocatedEdges.push_back(edge_t(i,j));
    // 	  }
    //  // 

    //  // loop over all edges and allocate them
    //  int usedPages = 0; // remember how many pages we have used
    //  edge_t selEdge; // the currently selected edge
    //  while(!unallocatedEdges.empty()) {
    //    if(usedPages==0) {
    // 	 selEdge = unallocatedEdges[0];
    // 	 sol.add_edge(0,selEdge);
    // 	 for(unsigned int i=1;i<unallocatedEdges.size();++i) { // loop over remaining edges
    // 	   if(sol.try_num_crossing(0,unallocatedEdges[i])) {
    // 	     sol.add_edge(0,unallocatedEdges[i]);
    // 	     cout << "adding element" << endl;;
    // 	   }
    // 	 }
    // 	 usedPages=1;
    //    }
    //    unallocatedEdges.pop_back();
    //    cout << ".";
    //  }

    // int crossings;
    // // loop over all edges
    // for( unsigned int i=0;i<n_vertices;++i)
    //   for( unsigned int j = i+1; j<n_vertices; ++j)
    // 	if( am[i][j]) // found an edge
    // 	  { // if there is no crossing, add edge to this page
    // 	    crossings= sol.try_num_crossing(0,edge_t(i,j));
    // 	    if(crossings==0)
    // 	      sol.add_edge(0,edge_t(i,j));
    // 	  }


    for( unsigned int i= 0; i<n_vertices; ++i )
        for( unsigned int j= i+1; j<n_vertices; ++j )
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
