
#ifndef _KPMP_SOLUTION_H_
#define _KPMP_SOLUTION_H_


#include <vector>
#include <list>
#include <ostream>


typedef unsigned int vertex_t;
typedef int order_t;

typedef std::pair< vertex_t, vertex_t > edge_t ;



class solution
{
public:



    struct page
    {
        typedef std::list< edge_t >::iterator edge_iterator_t;
        typedef std::list< edge_t >::const_iterator const_edge_iterator_t;

        page() : crossings(0) {};

        int crossings;
        std::list< edge_t > edges;
    };

    typedef std::vector< page >::iterator page_iterator_t;
    typedef std::vector< page >::const_iterator const_page_iterator_t;

    typedef std::vector< vertex_t >::iterator vertex_iterator_t;
    typedef std::vector< vertex_t >::const_iterator const_vertex_iterator_t;

    solution(int num_pages, const std::vector< vertex_t > & spine_order  );

    solution(int num_pages, int num_vertices );

    solution(int num_pages, int num_vertices, const std::vector< vertex_t > & partial_spine_order  );

    int try_num_crossing( unsigned int page, const edge_t & ) const;
    int try_num_crossing( const page & p, const edge_t & e ) const;

    void add_edge(  unsigned int page, const edge_t & e );
    void add_edges(  unsigned int page, const std::list<edge_t> & l );
    void add_edge(  page & p, const edge_t & e );
    void add_edge(  unsigned int page, const edge_t & e, int crossings );
    void add_edge(  page &p, const edge_t & e, int crossings );
    
    void remove_edge(  unsigned int page, const edge_t & e );
    void remove_edges( unsigned int page, const std::list<edge_t> & l );
    void remove_edge(  page & p, const edge_t & e );
    void remove_edge(  unsigned int page, const edge_t & e, int crossings );
    void remove_edge(  page & p, const edge_t & e, int crossings );
 
    void add_vertices_to_spine_order( const std::vector<vertex_t> & v );

    void swap_vertices( size_t index1, size_t index2 );
    void recalc_page( page & p );

    bool vertex_in_spine( vertex_t v ) const;


    const int get_pages() const { return pages.size(); };

    std::vector< vertex_t > & get_spine_order( ) { return spine_order; };
    const std::vector< vertex_t > & get_spine_order( ) const { return spine_order; };

    int get_crossings() const;
    int get_num_edges() const;


    vertex_iterator_t vertex_begin() { return spine_order.begin(); };
    const_vertex_iterator_t vertex_begin() const { return spine_order.begin(); };

    vertex_iterator_t vertex_end() { return spine_order.end(); };
    const_vertex_iterator_t vertex_end() const { return spine_order.end(); };


    page_iterator_t page_begin() { return pages.begin(); };
    const_page_iterator_t page_begin() const { return pages.begin(); };

    page_iterator_t page_end() { return pages.end(); };
    const_page_iterator_t page_end() const { return pages.end(); };

    page & page_back() { return pages.back(); };
    const page & page_back() const { return pages.back(); };

    friend std::ostream& operator<<(std::ostream& os, const solution & );


private:


    /** this is used to exclude symmetric solutions
      * solutions are aequivalent regarding shifts and mirrors
      */
    // TODO std::vector<vertex_t> normalized_spine_order;

    /** has to be initialized for alle vertices 
        assigns the order number to each vertex for 
        quick lookup */
    std::vector< order_t > spine_order_map;

    std::vector< vertex_t > spine_order;


    std::vector< page > pages;
};


std::ostream& operator<<(std::ostream& os, const solution & );


#endif // _KPMP_SOLUTION_H_

