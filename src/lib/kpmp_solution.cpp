#include <stdexcept>
#include <cassert>

#include "kpmp_solution.h"


solution::solution( int num_pages, const std::vector<vertex_t> & _spine_order )
: 
  spine_order_map( _spine_order.size() ),
  spine_order(_spine_order),
  pages( num_pages )
{
    int order_count= 0;

    for( auto v=spine_order.begin(); v!= spine_order.end(); ++v  )
    {
        spine_order_map[ *v ]= order_count++;
    }
}


/*
edge_t solution::order_edge( const edge_t & e )
{
    if( spine_order_map[e.first] <= spine_order_map[e.second] )
        return e;

    return edge_t(e.second, e.first);
}
*/


void solution::add_edge(  unsigned int page, const edge_t & e )
{
    add_edge( page, e, try_num_crossing(page, e) );
}


void solution::add_edge(  unsigned int page, const edge_t & e, int crossings )
{
    if( page >= pages.size() )
        throw std::out_of_range("requested page out of range");

    solution::page &p = pages[page];

    p.edges.push_back( e );
    p.crossings+= crossings;

}



int solution::try_num_crossing( unsigned int page, const edge_t & e ) const
{
    if( page >= pages.size() )
        throw std::out_of_range("requested page out of range");

    order_t check_edge_v1= spine_order_map[ e.first ];
    order_t check_edge_v2= spine_order_map[ e.second ];

    // to make things easy we order the edge
    if( check_edge_v1 > check_edge_v2 )
        std::swap( check_edge_v1, check_edge_v2 );

    int crossing_sum= 0;

    for( auto edge_it= pages[page].edges.begin(); 
              edge_it != pages[page].edges.end(); ++edge_it )
    {
        order_t page_edge_v1= spine_order_map[ edge_it->first ];
        order_t page_edge_v2= spine_order_map[ edge_it->second ];

        // to make things easy we order the edge
        if( page_edge_v1 > page_edge_v2 )
            std::swap( page_edge_v1, page_edge_v2 );

        if( check_edge_v1 < page_edge_v1 )
        {
            if( page_edge_v1 < check_edge_v2 && 
                check_edge_v2 < page_edge_v2 )
            {
                crossing_sum+= 1;
            }                
        }    
        else if (page_edge_v1 < check_edge_v1 )
        {
            if( check_edge_v1 < page_edge_v2 &&
                page_edge_v2 < check_edge_v2 )
            {
                crossing_sum+= 1;
            }
        }

    }

    return crossing_sum;
}


int solution::get_crossings() const
{
    int crossings= 0;

    for( auto page= pages.begin(); page != pages.end(); ++page ) 
        crossings+= page->crossings;

    return crossings;
}
