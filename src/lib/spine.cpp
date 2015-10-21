#include <algorithm>
#include <cstdlib>
#include <cassert>
#include <stdexcept>

#include "spine.h"




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


std::vector<vertex_t> spine_order_num_edges( const std::vector<std::vector<unsigned int> > & adjacencyList )
{
    std::vector<vertex_t> spine_order( spine_order_ascending(adjacencyList.size() ) );

    // this sort predicate just uses the number 
    // of neighbors of a vertex
    struct sort_pred_num_neighbors
    {
        sort_pred_num_neighbors(const std::vector<std::vector<unsigned int> > & _adjacencyList) 
        : adjacencyList(_adjacencyList) {} ;

        bool operator() (unsigned int x1, unsigned int x2) 
        { 
            return adjacencyList[x1].size() < adjacencyList[x2].size(); 
        };

        std::vector<std::vector<unsigned int> > adjacencyList;
    };

    std::sort( spine_order.begin(), spine_order.end(), sort_pred_num_neighbors(adjacencyList) );
    
    return spine_order;
}



//
// random_spine_generator
//

random_spine_generator::random_spine_generator( unsigned int num_vertices )
: free_vertices( spine_order_ascending(num_vertices) ),
  spine()
{
}
    

random_spine_generator::random_spine_generator( const std::vector<vertex_t> & base_spine )
: free_vertices( base_spine ),
  spine()
{
}

void random_spine_generator::random_change_block( unsigned int block_size, unsigned int num_random_changes, std::vector<vertex_t> & new_random_vert)
{
    if( free_vertices.size() == 0 )
        throw std::out_of_range("no more vertices to randomly add");

    if( block_size > free_vertices.size() )
        block_size= free_vertices.size();

    spine.reserve( spine.size() + block_size );
    new_random_vert.resize( block_size );


    // this is a block definig permutations
    std::vector<unsigned int> permutation_map(block_size);
    for(unsigned int i= 0; i< block_size; ++i )
        permutation_map[i]= i;

    // now we make permutations
    srand ( time(NULL) );
    for( unsigned int i=0; i<num_random_changes; ++i )
    {
        unsigned index1, index2, cnt=0;

        do 
        {
            index1= rand() % block_size;
            index2= rand() % block_size;

        }while( index1 == index2 && ++cnt<10 ); // yay ;)

        if(cnt >= 10)
            throw std::runtime_error("random generator is eval");

        std::swap( permutation_map[index1], permutation_map[index2]);

    }

    for( unsigned int i= 0; i<block_size; ++i )
    {
        vertex_t v= free_vertices[ permutation_map[i] ];
        spine.push_back(v);
        new_random_vert[i]= v;
    }
            
    free_vertices.erase( free_vertices.begin(), 
                         free_vertices.begin() + block_size );


}


void random_spine_generator::add_random_to_spine( unsigned int num, std::vector<vertex_t> & new_random_vert )
{
    if( free_vertices.size() == 0 )
        throw std::out_of_range("no more vertices to randomly add");

    if( num > free_vertices.size() )
        num= free_vertices.size();

    spine.reserve( spine.size() + num );
    new_random_vert.resize( num );

    srand ( time(NULL) );

    for( unsigned int i= 0; i<num; ++i )
        new_random_vert[i]= embed_vertex_to_spine( rand() % free_vertices.size() );
}

vertex_t random_spine_generator::embed_vertex_to_spine( unsigned int v_index )
{
    assert( v_index < free_vertices.size() );

    auto i= free_vertices.begin() + v_index;

    vertex_t v= *i;
    spine.push_back( v );
    free_vertices.erase( i );

    return v;
}
