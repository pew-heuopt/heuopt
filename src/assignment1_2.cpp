#include <iostream>
#include <memory>
#include <fstream>
#include <algorithm>


#include <assignment_helpers.h>

#include <kpmp_instance.h>
#include <kpmp_solution.h>
#include <spine.h>

void greedy_edge_add( solution & sol, 
                      const std::vector<vertex_t> & new_vertices,
                      const std::vector<std::vector<vertex_t> > & adjacency_list )

{                     
    //TODO: ensure to prevent duplicates
    //      could happen using an adjacency
    std::vector<int> vertex_added( adjacency_list.size(), 0);



    // TODO: this sort order is very important for beeing better than solutuin 1.1 ;)
    //       so we have to work on edge allocation as well
    auto new_vertices_sorted= new_vertices;
    std::sort( new_vertices_sorted.begin(), new_vertices_sorted.end() );


    //for( auto new_vert= new_vertices.begin(); new_vert!=new_vertices.end(); ++new_vert )
    for( auto new_vert= new_vertices_sorted.begin(); new_vert!=new_vertices_sorted.end(); ++new_vert )
    {
        auto neighbours= adjacency_list[*new_vert];

        for( auto neighbour= neighbours.begin(); neighbour!=neighbours.end(); ++neighbour )
        {
            // only add edges to vertices which are already in the spine
            if( sol.vertex_in_spine(*neighbour) && 
                !vertex_added[*neighbour] )
            {
                insert_on_next_crossfree_page(sol, edge_t(*new_vert,*neighbour) );
            }
        }
        vertex_added[*new_vert] = 1;
    }
}

std::pair<random_spine_generator*, solution*> generate_solution( random_spine_generator * current_spine_generator, 
                                                                 solution *current_solution, 
                                                                 int stepsize, unsigned num_random_changes,
                                                                 const std::vector<std::vector<vertex_t> > & adjacency_list )
{
    random_spine_generator * random_spine= new random_spine_generator( *current_spine_generator );
    solution * random_solution= new solution( *current_solution );
            
    // generate new random vertices
    std::vector<vertex_t> new_vertices(stepsize);

    //random_spine->add_random_to_spine(stepsize,new_vertices);
    random_spine->random_change_block( stepsize, num_random_changes, new_vertices);

    // enlarge our solution
    random_solution->add_vertices_to_spine_order( new_vertices );

    greedy_edge_add( *random_solution, new_vertices, adjacency_list );

    return std::make_pair(random_spine,random_solution);
}




//#define DEBUG 1

int main( int argc, char **argv)
{

    // Declare the supported options.
    boost::program_options::options_description desc("Allowed options");
    boost::program_options::variables_map vm;

    std::string input_filename;
    std::string output_filename;


    int num_partials= 1; // determines the stepsize
    int random_instances= 5;
    int num_random_changes= 4;
   

    desc.add_options()
         ("num-partials", boost::program_options::value<int>(&num_partials), "we can caluclate partial solutions (default 1)")
         ("random-instances", boost::program_options::value<int>(&random_instances), "number of random tries for each partial (default 5)")
         ("random-changes", boost::program_options::value<int>(&num_random_changes), "max difference in spine changes of the instances (default 4)")

             ;

    if( common_cmdline( argc, argv,
                    desc, vm, 
                    input_filename, output_filename ) )
        return 1;


        

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


    // moegliche variante:
    //  
    //  nodes kommen in uckets dazu, 
    //  es werden nur edges reingeworfen die reinreichen
    //
    //  * wie schnell findet man nur edges die auf das set passen?
    //      mit eineme vector der die position im ergebnis angibt,
    //      mit dem auslesen der 
    //
    //  * wird die page allocation jedesmal vollkommen neu gemacht?
    //      ja
    //

    int num_vertices= instance->getNumVertices();

    
    int stepsize;

    if( num_partials > num_vertices )
    {
        num_partials= 1;
        stepsize= num_vertices;
    }
    else
    {
        stepsize= num_vertices / num_partials;

        if( (stepsize * num_partials) < num_vertices )
            stepsize++;
    }

#ifdef DEBUG            
    std::cout << "num_vertices: " << num_vertices << " stepsize: " << stepsize << " num_partials: " << num_partials << std::endl;
#endif

    // starting with empty sets
    //random_spine_generator* current_spine_generator= new random_spine_generator( num_vertices );
    random_spine_generator* current_spine_generator= new random_spine_generator( spine_order_num_edges(instance->getAdjacencyList()) );

    solution              * current_solution= new solution(instance->getK(), num_vertices );


	vector<vector<unsigned int> > adjacencyList= instance->getAdjacencyList();

    
    for( int sol_length= 0; sol_length<num_vertices; sol_length+=stepsize )
    {
        
                                                
        //initial partial
        auto best_partial = generate_solution( current_spine_generator, 
                                                current_solution, 
                                                stepsize, 0, 
                                                adjacencyList );

        // now we try to top
        for( int i=0; i<random_instances; ++i )
        {
            auto random_partial = generate_solution( current_spine_generator, 
                                                     current_solution, 
                                                     stepsize, num_random_changes,
                                                     adjacencyList );


            
#ifdef DEBUG            
            std::cout << "sbest: " << *(best_partial.second) << std::endl;
            std::cout << "random_partial: " << *(random_partial.second) << std::endl;
#endif


            if( random_partial.second->get_crossings() <
                best_partial.second->get_crossings() )
            {
                std::swap( best_partial, random_partial );
            }


            delete random_partial.first;
            delete random_partial.second;

        }


        // best partial is our next solution base
        delete current_spine_generator;
        delete current_solution;

        current_spine_generator= best_partial.first;
        current_solution= best_partial.second;

    }

    


    //
    // output
    //
    write_solution( outfile, *current_solution );
	outfile.close();

    write_solution_statistics( std::cout, *current_solution );

    return 0;
}
