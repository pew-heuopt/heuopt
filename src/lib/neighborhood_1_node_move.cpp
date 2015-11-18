#include <iostream>
#include <stdexcept>
#include <cassert>

#include "neighborhood_1_node_move.h"


//#define DEBUG 1


const_neighborhood_1_node_move_iterator neighborhood_1_node_move_begin( const solution & init_solution )
{
    assert( init_solution.get_pages() && "solution with no pages not supported" );
    return const_neighborhood_1_node_move_iterator( init_solution );
}


const_neighborhood_1_node_move_iterator neighborhood_1_node_move_end( const solution & init_solution )
{
    assert( init_solution.get_pages() && "solution with no pages not supported" );
    auto it= neighborhood_1_node_move_begin(init_solution);
    it.set_to_end();


    return it;
}

const_neighborhood_1_node_move_iterator::const_neighborhood_1_node_move_iterator( const solution & _init_solution )
: init_solution( _init_solution ),
  index_node1(0), index_node2(2),
  spine_size( init_solution.get_spine_order().size() )
{

}


solution const_neighborhood_1_node_move_iterator::operator*() const
{
    solution sol( init_solution );


#ifdef DEBUG    
    std::cout << "* " << index_node1 << " " << index_node2 << std::endl;
    std::cout << "    before: " << sol << std::endl;
#endif // DEBUG    

    sol.move_vertex( index_node1, index_node2 ); 

#ifdef DEBUG    
    std::cout << "    after: " << sol << std::endl;
#endif // DEBUG    


    return sol;
}


void const_neighborhood_1_node_move_iterator::set_to_end()
{
    index_node1= spine_size;
    index_node2= 0;
}


bool const_neighborhood_1_node_move_iterator::is_end()
{
    return index_node1 == (spine_size) &&
           index_node2 == 0;

}

void const_neighborhood_1_node_move_iterator::increment()
{
    if( is_end() )
        throw std::out_of_range("increment after last");

    index_node2++;

    if( index_node2 == index_node1  || // identical move
        index_node2 == (index_node1-1) )   // this move has been covered in prior iteratiosn
        index_node2 = index_node1 + 2;

    while( index_node2 > spine_size && !is_end() )
    {
        index_node1++;
        index_node2= 0;


        if( ( index_node2 == index_node1 )  || // identical move
            ( index_node2 == (index_node1-1) ) )   // this move has been covered in prior iteratiosn
        {
            index_node2 = index_node1 + 2;
        }

    }
#ifdef DEBUG
    std::cout << "INC: index1: " << index_node1 << " index2: " << index_node2 << std::endl;
#endif // DEBUG
}


bool const_neighborhood_1_node_move_iterator::operator==( const const_neighborhood_1_node_move_iterator & rhs ) const
{
    return index_node1 == rhs.index_node1 &&
           index_node2 == rhs.index_node2 ;
}

bool const_neighborhood_1_node_move_iterator::operator!=( const const_neighborhood_1_node_move_iterator & rhs ) const
{
    return !operator==(rhs);
}


