
#ifndef _KPMP_SOLUTION_H_
#define _KPMP_SOLUTION_H_


#include <vector>
#include <list>


typedef unsigned int vertex_t;
typedef unsigned int order_t;

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

    int try_num_crossing( unsigned int page, const edge_t & ) const;


    void add_edge(  unsigned int page, const edge_t & e );
    void add_edge(  unsigned int page, const edge_t & e, int crossings );
    
    const int get_pages() const { return pages.size(); };

    const std::vector< vertex_t > & get_spine_order( ) const { return spine_order; };

    int get_crossings() const;


    const_page_iterator page_begin() const { return pages.begin(); };
    const_page_iterator page_end() const { return pages.end(); };



private:


    /** this is used to exclude symmetric solutions
      * solutions are aequivalent regarding shifts and mirrors
      */
    std::vector<vertex_t> normalized_spine_order;

    /** has to be initialized for alle vertices 
        assigns the order number to each vertex for 
        quick lookup */
    std::vector< order_t > spine_order_map;

    std::vector< vertex_t > spine_order;


    std::vector< page > pages;
};




#endif // _KPMP_SOLUTION_H_

