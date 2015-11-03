#include <iostream>
#include <stdexcept>
#include <cassert>

#include "neighborhood_1_node_edge_move.h"


//#define DEBUG 1


const_neighborhood_1_node_edge_move_iterator neighborhood_1_node_edge_move_begin( const solution & init_solution )
{
    assert( init_solution.get_pages() && "solution with no pages not supported" );
    return const_neighborhood_1_node_edge_move_iterator( init_solution );
}


const_neighborhood_1_node_edge_move_iterator neighborhood_1_node_edge_move_end( const solution & init_solution )
{
    assert( init_solution.get_pages() && "solution with no pages not supported" );
    auto it= neighborhood_1_node_edge_move_begin(init_solution);
    it.set_to_end();


    return it;
}

const_neighborhood_1_node_edge_move_iterator::const_neighborhood_1_node_edge_move_iterator( const solution & _init_solution )
: init_solution( _init_solution ),
  solution_without_current_edge( _init_solution ),

  current_page( init_solution.page_begin() ),

  current_node( init_solution.vertex_begin() )
{

    //skip nodes wich don't carry edges
    while( fetch_current_node_edges() == 0 )
        increment_current_node();

    start_new_target_iteration();
}


size_t fetch_current_node_edges()
{
    // TODO: fetch node from the current page which are containing 

    return current_node_edges;
}


void const_neighborhood_1_node_edge_move_iterator::start_new_target_iteration()
{
    assert( current_edge_it != current_page->edges.end() );

    target_page= 0;

    // we skip the first page if it is the same
    if( current_and_target_same_page() )
    {
        increment();
    }

    
    if( !is_end() )
    {
        // copy the init solution and extract the current edge
        solution_without_current_node_edges= solution(init_solution);
        solution_without_current_node_edges.remove_edges(current_page-init_solution.page_begin(), current_node_edges );
    }
    

}

solution const_neighborhood_1_node_edge_move_iterator::operator*() const
{
    solution sol( solution_without_current_node_edges );

    sol.add_edges( target_page, current_node_edges );

#ifdef DEBUG    
//    std::cout << "*: target_page: " << target_page <<  " current edge: " << current_edge_it->first << " " <<  current_edge_it->second  << std::endl;
#endif // DEBUG    

    return sol;
}


void const_neighborhood_1_node_edge_move_iterator::set_to_end()
{
    current_page= init_solution.page_end();
    current_node= init_solution.vertex_end();
    target_page = solution_without_current_edge.get_pages();
}


bool const_neighborhood_1_node_edge_move_iterator::is_end()
{
    return current_page == init_solution.page_end();
}

void const_neighborhood_1_node_edge_move_iterator::increment_current_node()
{
    if( current_page == init_solution.page_end() )
        throw std::out_of_range("trying to increment iterator in end position");

    
    if( current_node != init_solution.vertex_end() )
        ++current_node;

// TODO: fetch     current_node_edges, skip nodes which don't have edges
    // page wrap around  
    while( current_node == init_solution.vertex_end() ) // to skip empty pages
    {
        // if this was the last page we are done
        if( ++current_page == init_solution.page_end() )
        {

            return;
        }

        current_node= init_solution.vertex_end();
    }
// =================================    

#ifdef DEBUG    
//    std::cout << "increment current edge: " << current_edge_it->first << " " <<  current_edge_it->second  << std::endl;
#endif // DEBUG    

}

bool const_neighborhood_1_node_edge_move_iterator::current_and_target_same_page()
{
    return( target_page ==
            ( current_page - init_solution.page_begin() ) );
}



void const_neighborhood_1_node_edge_move_iterator::increment()
{
    if( is_end() )
        throw std::out_of_range("trying to increment end iterator");

    do
    {
        target_page++;

    }
    while( target_page != solution_without_current_edge.get_pages() &&
           current_and_target_same_page() );

    // we are done with our target pages, next current edge select
    if( target_page == solution_without_current_node_edges.get_pages() )
    {
        increment_current_node();

        // if we are not at the end we prepare a now solution
        if( !is_end() )
        {
            start_new_target_iteration();
        }

    }

}


bool const_neighborhood_1_node_edge_move_iterator::operator==( const const_neighborhood_1_node_edge_move_iterator & rhs ) const
{
    return current_page == rhs.current_page &&
           current_edge_it == rhs.current_edge_it &&
           target_page == rhs.target_page ;
}

bool const_neighborhood_1_node_edge_move_iterator::operator!=( const const_neighborhood_1_node_edge_move_iterator & rhs ) const
{
    return !operator==(rhs);
}


