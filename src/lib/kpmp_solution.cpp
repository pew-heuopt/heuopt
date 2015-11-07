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


solution::solution( int num_pages, int num_vertices  )
: 
  spine_order_map( num_vertices, -1 ),
  spine_order(),
  pages( num_pages )
{
}

solution::solution( int num_pages, int num_vertices, const std::vector< vertex_t > & partial_spine_order  )
: 
  spine_order_map( num_vertices, -1 ),
  spine_order(partial_spine_order),
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


void solution::add_edges(  unsigned int page, const std::list<edge_t> & l )
{
    for(auto i= l.begin(); i!=l.end(); ++i )
        add_edge( page, *i );
}

void solution::add_edge(  unsigned int page, const edge_t & e )
{
    add_edge( page, e, try_num_crossing(page, e) );
}

void solution::add_edge(  page & p , const edge_t & e )
{
    add_edge( p, e, try_num_crossing(p, e) );
}



void solution::add_edge(  unsigned int page, const edge_t & e, int crossings )
{
    if( page >= pages.size() )
        throw std::out_of_range("requested page out of range");

    add_edge( pages[page], e, crossings );

}

void solution::add_edge( page & p, const edge_t & e, int crossings )
{
    p.edges.push_back( e );
    p.crossings+= crossings;
}


void solution::remove_edges( unsigned int page, const std::list<edge_t> & l )
{
    for(auto i= l.begin(); i!=l.end(); ++i )
        remove_edge( page, *i );
}

void solution::remove_edge(  unsigned int page, const edge_t & e )
{
    remove_edge( page, e, try_num_crossing(page, e) );
}

void solution::remove_edge(  page & p, const edge_t & e )
{
    remove_edge( p, e, try_num_crossing(p, e) );
}



void solution::remove_edge(  unsigned int page, const edge_t & e, int crossings )
{
    if( page >= pages.size() )
        throw std::out_of_range("requested page out of range");

    remove_edge(pages[page], e, crossings);
}


void solution::remove_edge(  page & p, const edge_t & e, int crossings )
{
    p.edges.remove( e );
    p.crossings-= crossings;

}

void solution::add_vertices_to_spine_order( const std::vector<vertex_t> & new_vertices )
{
    int spine_i= spine_order.size();

    spine_order.resize( spine_order.size() + new_vertices.size() );


    for( auto v= new_vertices.begin(); v != new_vertices.end(); ++v )
    {
        spine_order[spine_i]= *v;

        if( vertex_in_spine(*v) )
            throw std::runtime_error("vertex already in spine");

        spine_order_map[*v] = spine_i;

        spine_i++;
    }
}


#include <iostream>
void solution::swap_vertices( size_t index1, size_t index2 )
{

    if( index1 >= spine_order.size() ||
        index2 >= spine_order.size() )
    {
        throw std::out_of_range("swap vertices outside of index range");
    }

    std::list<std::pair<page_iterator_t,edge_t> > touching_edges;

    // collect touching edges
    for( auto p= pages.begin(); p != pages.end(); ++p )
        for( auto e= p->edges.begin(); e != p->edges.end(); ++e )
            if( e->first == spine_order[index1] || e->first== spine_order[index2] ||
                e->second == spine_order[index1] || e->second== spine_order[index2] )
            {
                touching_edges.push_back( make_pair(p,*e) );
            }

    // remove touching edges
    for( auto i= touching_edges.begin(); i != touching_edges.end(); ++i )
        remove_edge( * i->first, i->second );

    // 
    // swapping
    std::swap( spine_order[index1], spine_order[index2] );

    // new order map
    int order_count= 0;
    for( auto v=spine_order.begin(); v!= spine_order.end(); ++v  )
    {
        spine_order_map[ *v ]= order_count++;
    }


    // re -add edges
    for( auto i= touching_edges.begin(); i != touching_edges.end(); ++i )
        add_edge( * i->first, i->second );

    
    
    /*
    // recalc all pages
    for( auto i= pages.begin(); i != pages.end(); ++i )
        recalc_page( *i ); 

    std::cerr << "get_crossings(): " << get_crossings() << " before " << cr << std::endl;
   */ 
}

void solution::recalc_page( page & p )
{

    // the cheap one ;)
    auto edge_list= p.edges;

    p.crossings= 0;
    
    p.edges.clear();

    for( auto i= edge_list.begin(); i != edge_list.end(); ++i )
        add_edge( p, *i);
}


bool solution::vertex_in_spine( vertex_t v ) const
{
    return spine_order_map[v] != -1 ;
}




int solution::try_num_crossing( unsigned int page, const edge_t & e ) const
{
    if( page >= pages.size() )
        throw std::out_of_range("requested page out of range");
    
    return try_num_crossing( pages[page], e );
}

int solution::try_num_crossing( const page & p, const edge_t & e ) const
{
    order_t check_edge_v1= spine_order_map[ e.first ];
    order_t check_edge_v2= spine_order_map[ e.second ];

    // to make things easy we order the edge
    if( check_edge_v1 > check_edge_v2 )
        std::swap( check_edge_v1, check_edge_v2 );

    if( check_edge_v1 == -1 )
        throw std::out_of_range("trying an edge with a vertex which is not part of spine");

    int crossing_sum= 0;

    for( auto edge_it= p.edges.begin(); 
              edge_it != p.edges.end(); ++edge_it )
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

int solution::get_num_edges() const
{
    int num_edges= 0;

    for( auto page= pages.begin(); page != pages.end(); ++page ) 
        num_edges+= page->edges.size();

    return num_edges;
}


std::ostream& operator<<(std::ostream& os, const solution & sol)
{
    for( auto sv= sol.spine_order.begin(); sv!=sol.spine_order.end(); ++sv )
        os << *sv << " ";
   return os; 
}
