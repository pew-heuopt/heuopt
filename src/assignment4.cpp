#include <iostream>
#include <memory>
#include <fstream>
#include <algorithm>
#include <cassert>

#include <boost/shared_ptr.hpp>

#include <assignment_helpers.h>

#include <kpmp_instance.h>
#include <kpmp_solution.h>
#include <spine.h>

//#define DEBUG 1


/** collect edges and add none-added adjacent nodes to the hull 
    now present because of this next_vertex
 */
template<typename T1, typename T2>
void collect_hull( const solution * sol,
                   const std::vector<std::vector<vertex_t> > & adjacency_list,
                   const vertex_t & vertex,
                   T1 & vertex_hull, T2 & edge_hull )
{
    assert( vertex < adjacency_list.size() );

    auto neighbours= adjacency_list[vertex];

    for( auto neighbour= neighbours.begin(); neighbour!=neighbours.end(); ++neighbour )
    {
        if( sol->vertex_in_spine(*neighbour) ) 
            edge_hull.push_back( edge_t(vertex,*neighbour) );
        else
            vertex_hull[*neighbour]= 1;
    }
}



struct edge_cost
{
    edge_cost( int _page, edge_t _edge, int _cost )
    : page(_page),
      edge(_edge), 
      cost(_cost), 
      weight(0.0)
    {
    };



    int page;
    edge_t edge; 
    double cost; 
    double weight;
};



void build_local_edge_costs( const solution & sol,
                             const std::list<edge_t> & edges, 
                             std::vector<edge_cost> & edge_costs )
    
{
    assert( edge_costs.size() == 0 );

    int max_cost= 0;

    for( auto e= edges.begin(); e!=edges.end(); ++e )
    {
        for( int p= 0; p<sol.get_pages(); ++p )
        {
            int c= sol.try_num_crossing( p, *e );

            if( c > max_cost )
                max_cost= c;

            edge_costs.push_back( edge_cost(p, *e, c) );
        }
    }
                

#ifdef DEBUG
    std::cout << "build_local_edge_costs: max_cost= " << max_cost << std::endl;
#endif // DEBUG    


    for( auto ec= edge_costs.begin(); ec!=edge_costs.end(); ++ec )
    {
        if( max_cost == 0 )
            ec->weight= 1.0;
        else 
            ec->weight= 1.0 - ec->cost / max_cost ;
    }
}

std::vector<edge_cost> filter_edge( const std::vector<edge_cost> & edge_costs, const edge_t & rm_edge )
{
    std::vector<edge_cost> filtered;

    for( auto ec= edge_costs.begin(); ec!=edge_costs.end(); ++ec )
        if( ec->edge != rm_edge )
            filtered.push_back( *ec );
    return filtered;
}


void increment_local_edge_costs( const solution & sol,
                                 std::vector<edge_cost> & edge_costs )
{
    int max_cost= 0;

    for( auto ec= edge_costs.begin(); ec!=edge_costs.end(); ++ec )
    {
        int c= sol.try_num_crossing( ec->page, ec->edge ) + ec->cost;

        if( c > max_cost )
                max_cost= c;

        ec->cost= c;
    }

#ifdef DEBUG
    std::cout << "increment_local_edge_costs: max_cost= " << max_cost << std::endl;
#endif // DEBUG    


    for( auto ec= edge_costs.begin(); ec!=edge_costs.end(); ++ec )
    {
        if( max_cost == 0 )
            ec->weight= 1.0;
        else 
            ec->weight= 1.0 - ec->cost / max_cost ;
    }

}
 
struct vertex_cost
{
    vertex_cost( vertex_t _vertex, int _cost )
    : vertex(_vertex),
      cost(_cost), 
      weight(0.0)
    {
    };


    vertex_t vertex;
    double cost; 
    double weight;

};



void build_local_vertex_costs( const std::vector<std::vector<vertex_t> > & adjacency_list,
                               const std::map<vertex_t,bool> & vertex_hull,
                               std::vector<vertex_cost> & vertex_costs )
{
    int max_cost= 0;

    for( auto e= vertex_hull.begin(); e!=vertex_hull.end(); ++e )
    {
        vertex_t v= e->first; 
        int c= (adjacency_list[v]).size();

        if( c > max_cost )
            max_cost= c;

        vertex_costs.push_back( vertex_cost(v, c) );
    }
                

#ifdef DEBUG
    std::cout << "build_local_vertex_costs: max_cost= " << max_cost << std::endl;
#endif // DEBUG    


    for( auto ec= vertex_costs.begin(); ec!=vertex_costs.end(); ++ec )
    {
        if( max_cost == 0 )
            ec->weight= 1.0;
        else 
            ec->weight= 1.0 - ec->cost / max_cost ;
    }


} 

template<typename T>
bool weight_order_func( const T & e1, const T & e2 )

{
    return e1.weight > e2.weight;
}


template<typename T>
typename std::vector<T>::const_iterator weight_choose_random_best( const std::vector<T> & list )
{
    auto start= list.begin();
    auto best_val= start->weight;
    auto i= start+1;

    for( ; i!=list.end(); ++i )
    {
        if( i->weight!= best_val )
            break;
    }
    auto num= i-start;

    return start + rand() % num;
}

solution * ant_walk( int num_pages, /*const std::vector<vertex_t> & vertices,*/
               const std::vector<std::vector<vertex_t> > & adjacency_list )
{
    //std::vector<int> free_vertices( adjacency_list.size(), 0);

    // everything thats reachable from current added vertices
    std::map<vertex_t,bool> vertex_hull; 

    solution * ant_solution= new solution(num_pages, adjacency_list.size());
    assert(ant_solution);

//  TODO: salt if we are done, i like it deterministic when i debug;)    
//    srand ( time(NULL) );

    // generate random start vertex
    vertex_t next_vertex= rand()%adjacency_list.size() ;

    do
    {
        vertex_hull.erase(next_vertex);
        ant_solution->add_vertices_to_spine_order( next_vertex );


        std::list<edge_t> edge_hull;

        collect_hull( ant_solution, adjacency_list, next_vertex, 
                      vertex_hull, edge_hull );


        // decide edge together with global info


        std::vector<edge_cost> edge_costs;

        // build local info for edge decission
        build_local_edge_costs( *ant_solution,
                                edge_hull, edge_costs );


        while( edge_costs.size() )
        {
            sort( edge_costs.begin(), edge_costs.end(), weight_order_func<edge_cost> );
            edge_cost best_edge= *weight_choose_random_best(edge_costs); 

            //assert( ant_solution->try_num_crossing( best_edge.page, best_edge.edge ) == best_edge.cost );
            ant_solution->add_edge( best_edge.page, best_edge.edge, best_edge.cost );  // TODO: dangerous ;)


            // rebuild local info for next edge decission
            // we now add this choosen edge to the prior calculated costs to save time
            {
                solution increment_solution ( num_pages, adjacency_list.size(),ant_solution->get_spine_order() );
                increment_solution.add_edge( best_edge.page, best_edge.edge ); 


                edge_costs= filter_edge( edge_costs, best_edge.edge);

                increment_local_edge_costs( increment_solution, edge_costs );
            }

        }

 

        if( vertex_hull.size() )
        {
            // decide node together with global info
            std::vector<vertex_cost> vertex_costs;
            build_local_vertex_costs( adjacency_list, vertex_hull, vertex_costs);
            sort( vertex_costs.begin(), vertex_costs.end(), weight_order_func<vertex_cost> );

            // if there are multiple bests, choos a random
            next_vertex= weight_choose_random_best( vertex_costs )->vertex;
        }
        else
        {
            // we could have islands or not connected vertices
            // we just add them
            if( ant_solution->get_spine_order().size() < adjacency_list.size() )
            {
                
                for( auto i= adjacency_list.begin(); i!=adjacency_list.end(); ++i )
                {
                    vertex_t v= i - adjacency_list.begin();
                    if( ! ant_solution->vertex_in_spine(v) )
                    { 
                        next_vertex= v;
                        break;
                    }
                }
 
            }
        }

#ifdef DEBUG        
        std::cout << "vertex_hull.size(): " << vertex_hull.size() << std::endl;
        std::cout << "next_vertex= " << next_vertex << std::endl;
#endif // DEBUG

    }while( ant_solution->get_spine_order().size() != adjacency_list.size());

    return ant_solution;
}





int main( int argc, char **argv)
{

    // Declare the supported options.
    boost::program_options::options_description desc("Allowed options");
    boost::program_options::variables_map vm;

    std::string input_filename;
    std::string output_filename;


    int num_ants= 3;
    int num_runs= 5;
   

    desc.add_options()
         ("num-ants", boost::program_options::value<int>(&num_ants), "number of ants in one run")
         ("num-runs", boost::program_options::value<int>(&num_runs), "number of ant walks and pheromon steps")

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

    
    /*
    auto adjacency_list= instance->getAdjacencyList();
    for( auto i= adjacency_list.begin(); i!=adjacency_list.end(); ++i )
    {
        std::cout <<  i - adjacency_list.begin() << ": ";
        for( auto j= i->begin(); j!=i->end(); ++j )
            std::cout << *j << " ";

        std::cout << std::endl;
    }
    return 1;
    */

    boost::shared_ptr<solution> best_solution;
    
    for( int run=0; run<num_runs; ++run )
    {
        std::vector< boost::shared_ptr<solution> > ant_solutions(num_ants);

        for( int ant=0; ant<num_ants; ++ant )
        {
            ant_solutions[ant]= boost::shared_ptr<solution>( ant_walk(instance->getK(), instance->getAdjacencyList()) );

#ifdef DEBUG
                std::cout << "ant " << ant << ": " << * (ant_solutions[ant]) << ": " << (ant_solutions[ant])->get_crossings()<< std::endl;
#endif // DEBUG                

            if( !best_solution ||
                ant_solutions[ant]->get_crossings() < best_solution->get_crossings() )
            {
                best_solution= ant_solutions[ant];
            }

        }
    }


    //
    // output
    //
    write_solution( outfile, *best_solution );
	outfile.close();

    write_solution_statistics( std::cout, *best_solution );

    return 0;
}
