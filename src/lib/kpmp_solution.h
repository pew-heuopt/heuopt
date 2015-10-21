
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
        page() : crossings(0) {};

        int crossings;
        std::list< edge_t > edges;
    };

    typedef std::vector< page >::const_iterator const_page_iterator;


    solution(int num_pages, const std::vector< vertex_t > & spine_order  );

    solution(int num_pages, int num_vertices );

    solution(int num_pages, int num_vertices, const std::vector< vertex_t > & partial_spine_order  );

    int try_num_crossing( unsigned int page, const edge_t & ) const;

    void add_edge(  unsigned int page, const edge_t & e );
    void add_edge(  unsigned int page, const edge_t & e, int crossings );
    

    void add_vertices_to_spine_order( const std::vector<vertex_t> & v );

    bool vertex_in_spine( vertex_t v ) const;


    const int get_pages() const { return pages.size(); };

    const std::vector< vertex_t > & get_spine_order( ) const { return spine_order; };

    int get_crossings() const;


    const_page_iterator page_begin() const { return pages.begin(); };
    const_page_iterator page_end() const { return pages.end(); };

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

