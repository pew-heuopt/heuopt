#include <iostream>
#include <stdexcept>
#include <cassert>
#include <cstdlib>

#include "neighborhood_x_node_move_random.h"


//#define DEBUG 1


const_neighborhood_x_node_move_random_iterator neighborhood_x_node_move_random_begin( const solution & init_solution, int num_random_moves  )
{
    assert( init_solution.get_pages() && "solution with no pages not supported" );
    return const_neighborhood_x_node_move_random_iterator( init_solution, num_random_moves );
}


const_neighborhood_x_node_move_random_iterator neighborhood_x_node_move_random_end( const solution & init_solution, int num_random_moves )
{
    assert( init_solution.get_pages() && "solution with no pages not supported" );
    auto it= neighborhood_x_node_move_random_begin(init_solution, num_random_moves);

    return it;
}

const_neighborhood_x_node_move_random_iterator::const_neighborhood_x_node_move_random_iterator( const solution & _init_solution,  int _num_random_moves )
: init_solution( _init_solution ),
  num_random_moves( _num_random_moves ),
  spine_size( init_solution.get_spine_order().size() )
{

}


solution const_neighborhood_x_node_move_random_iterator::operator*() const
{
    solution sol( init_solution );


#ifdef DEBUG    
    std::cout << "    before: " << sol << std::endl;
#endif // DEBUG    


    srand ( time(NULL) );

    std::list< std::pair<size_t,size_t> > move_list;

    for( int i=0; i<num_random_moves; ++i )
    {
        int index= rand() % spine_size;
        int to_pos= rand() % (spine_size+1);

        move_list.push_back( std::make_pair(index, to_pos) );
    }

    sol.move_vertices( move_list ); 

#ifdef DEBUG    
    std::cout << "    after: " << sol << std::endl;
#endif // DEBUG    


    return sol;
}


void const_neighborhood_x_node_move_random_iterator::set_to_end()
{
}


bool const_neighborhood_x_node_move_random_iterator::is_end()
{
    return false;

}

void const_neighborhood_x_node_move_random_iterator::increment()
{
}


bool const_neighborhood_x_node_move_random_iterator::operator==( const const_neighborhood_x_node_move_random_iterator & rhs ) const
{
    return false;
}

bool const_neighborhood_x_node_move_random_iterator::operator!=( const const_neighborhood_x_node_move_random_iterator & rhs ) const
{
    return !operator==(rhs);
}


