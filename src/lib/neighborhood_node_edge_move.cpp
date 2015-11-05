#include <iostream>
#include <stdexcept>
#include <cassert>

#include "neighborhood_node_edge_move.h"


//#define DEBUG 1


const_neighborhood_node_edge_move_iterator neighborhood_node_edge_move_begin( const solution & init_solution )
{
    assert( init_solution.get_pages() && "solution with no pages not supported" );
    return const_neighborhood_node_edge_move_iterator( init_solution );
}


const_neighborhood_node_edge_move_iterator neighborhood_node_edge_move_end( const solution & init_solution )
{
    assert( init_solution.get_pages() && "solution with no pages not supported" );
    auto it= neighborhood_node_edge_move_begin(init_solution);
    it.set_to_end();


    return it;
}

// constructor
const_neighborhood_node_edge_move_iterator::const_neighborhood_node_edge_move_iterator( const solution & _init_solution )
: init_solution( _init_solution ),
  solution_without_node_edges( _init_solution ),

  current_page( init_solution.page_begin() ),

  current_edge_it( (*current_page).edges.begin() ),
  spine_size( init_solution.get_spine_order().size() ),
  index_node (0),
  num_pages( init_solution.get_pages() )

{
  

}

// void const_neighborhood_node_edge_move_iterator::start_new_target_iteration()
// {
//     assert( current_edge_it != current_page->edges.end() );

//     // set index_node to zero as we start at the beginning
//     index_node= 0;
//     // set page_counter to zero, thus this is the initial solution
//     page_counter= 0; 

//     // skip the initial solution and just increment
//     //{
//     //    increment();
//     //}

    
// //     // copy the init solution and extract the current edge

// //     solution_without_node_edges= solution(init_solution);
// //     for(page_iterator_t iter init_solution.page_begin(),iter != init_solution.page_end();iter++) {
// //       for(edge_iterator_t edgeIt iter.edges.begin(),edgeIt != iter.edges.end();edgeIter++) {
// // 	cout << "bla";
// //       }
// //     }
// }

// /*
//  return solution corresponding to current iterator state?
// */

solution const_neighborhood_node_edge_move_iterator::operator*() const
{
    solution sol( init_solution );

    /* generate neighbourhood corresponding to the state of the iterator:
     1) remove all edges that are incident to index_node
     2) add it to the 'next' page. If edge is already on the last page, add it to page 1;
     FIXME: not sure what the most efficient way to do this is. We have to make sure that we do not move an edge twice.
     => remove all edges and put them in a list; remember their page; add them back later to the next page
    */ 
    std::list<edge_t> removedEdges;
    std::list<unsigned int> correspondingPage;

    // get page

    unsigned int selPage= 0;
    unsigned int selEdge= 0;
    std::cout << "looping over all edges" << std::endl;
    // loop over all pages
    for(solution::const_page_iterator_t page_it = sol.page_begin(); page_it != sol.page_end();++page_it) {
      std::cout << "loooop:" << selPage << std::endl;
      selEdge =0;
      for(solution::page::const_edge_iterator_t edge_it = (*page_it).edges.begin();edge_it != (*page_it).edges.end();++edge_it) {
	std::cout << "on edge: " << selEdge << " "<< (*edge_it).first << "/" << (*edge_it).second << std::endl;
	// if we find an edge then put it into list and remove it
	if( (*edge_it).first == index_node || (*edge_it).second == index_node) {
	  removedEdges.push_back(*edge_it);
	  correspondingPage.push_back(selPage);

	  // sol.remove_edge(selPage, *edge_it); // 
	}
	++selEdge;
      }
      


      // increment selPage
      ++selPage;
    }

    std::cout << "Number of removed edges: " << index_node << "/" << removedEdges.size() << std::endl;	  

    // sol.add_edge( target_page, *current_edge_it );

#ifdef DEBUG    
    //    std::cout << "*: target_page: " << target_page <<  " current edge: " << current_edge_it->first << " " <<  current_edge_it->second  << std::endl;
#endif // DEBUG    

    return sol;
}








// set iterator to last iteration
void const_neighborhood_node_edge_move_iterator::set_to_end()
{
  // set index_node to last node and maximum page_move
  std::cout << spine_size << " " << num_pages << " ";
  index_node= spine_size;
  page_counter= num_pages-1;
}



bool const_neighborhood_node_edge_move_iterator::is_end()
{
  // final node and we can only move each edge a maximum of (num_pages - 1) times
  return index_node == spine_size && page_counter == num_pages-1;
}




void const_neighborhood_node_edge_move_iterator::increment_current_edge()
{
    // if( current_page == init_solution.page_end() )
    //     throw std::out_of_range("trying to increment iterator in end position");

    
    // if( current_edge_it != current_page->edges.end() )
    //     ++current_edge_it;

    // // page wrap around
    // while( current_edge_it == current_page->edges.end() ) // to skip empty pages
    // {
    //     // if this was the last page we are done
    //     if( ++current_page == init_solution.page_end() )
    //     {

    //         return;
    //     }

    //     current_edge_it= current_page->edges.begin();
    // }

#ifdef DEBUG    
    std::cout << "increment current edge: " << current_edge_it->first << " " <<  current_edge_it->second  << std::endl;
#endif // DEBUG    

}

// bool const_neighborhood_node_edge_move_iterator::current_and_target_same_page()
// {
//     return( target_page ==
//             ( current_page - init_solution.page_begin() ) );
// }


// increment the iterator
void const_neighborhood_node_edge_move_iterator::increment()
{
  page_counter++;
  if(page_counter>(num_pages-1)) {
    page_counter= 1; // set to start value, i.e. move one page
    index_node++;
  }

#ifdef DEBUG  
std::cout << "page_counter: " <<page_counter << " index node: " << index_node << std::endl;
#endif // DEBUG
    // while( target_page != solution_without_node_edges.get_pages() &&
    //        current_and_target_same_page() );

    // // we are done with our target pages, next current edge select
    // if( target_page == solution_without_current_edge.get_pages() )
    // {
    //     increment_current_edge();

    //     // if we are not at the end we prepare a now solution
    //     if( !is_end() )
    //     {
    //         start_new_target_iteration();
    //     }

    // }

}


bool const_neighborhood_node_edge_move_iterator::operator==( const const_neighborhood_node_edge_move_iterator & rhs ) const
{
  // if selected node and page_counter are equal:
  return index_node == rhs.index_node &&
    page_counter == rhs.page_counter;
}

bool const_neighborhood_node_edge_move_iterator::operator!=( const const_neighborhood_node_edge_move_iterator & rhs ) const
{
    return !operator==(rhs);
}


