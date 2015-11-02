#include <iostream>
#include <stdexcept>
#include <cassert>

#include "neighborhood_1_node_flip.h"


//#define DEBUG 1


const_neighborhood_1_node_flip_iterator neighborhood_1_node_flip_begin( const solution & init_solution )
{
    assert( init_solution.get_pages() && "solution with no pages not supported" );
    return const_neighborhood_1_node_flip_iterator( init_solution );
}


const_neighborhood_1_node_flip_iterator neighborhood_1_node_flip_end( const solution & init_solution )
{
    assert( init_solution.get_pages() && "solution with no pages not supported" );
    auto it= neighborhood_1_node_flip_begin(init_solution);
    it.set_to_end();


    return it;
}

const_neighborhood_1_node_flip_iterator::const_neighborhood_1_node_flip_iterator( const solution & _init_solution )
: init_solution( _init_solution ),
  index_node1(0), index_node2(1),
  spine_size( init_solution.get_spine_order().size() )
{

}


solution const_neighborhood_1_node_flip_iterator::operator*() const
{
    solution sol( init_solution );


#ifdef DEBUG    
    std::cout << sol << std::endl;
    std::cout << "permutation " << index_node1 << " " << index_node2 << std::endl;
#endif // DEBUG    

    sol.swap_vertices( index_node1, index_node2 ); 

#ifdef DEBUG    
    std::cout << sol << std::endl;
#endif // DEBUG    


    return sol;
}


void const_neighborhood_1_node_flip_iterator::set_to_end()
{
    index_node1= spine_size-1;
    index_node2= spine_size;
}


bool const_neighborhood_1_node_flip_iterator::is_end()
{
    return index_node1 == (spine_size-1) &&
           index_node2 == spine_size;

}

void const_neighborhood_1_node_flip_iterator::increment()
{
    if( is_end() )
        throw std::out_of_range("increment after last");

    index_node2++;

    if( index_node2 >= spine_size )
    {
        index_node1++;
        index_node2= index_node1 + 1;
    }

}


bool const_neighborhood_1_node_flip_iterator::operator==( const const_neighborhood_1_node_flip_iterator & rhs ) const
{
    return index_node1 == rhs.index_node1 &&
           index_node2 == rhs.index_node2 ;
}

bool const_neighborhood_1_node_flip_iterator::operator!=( const const_neighborhood_1_node_flip_iterator & rhs ) const
{
    return !operator==(rhs);
}


