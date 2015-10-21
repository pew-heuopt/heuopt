#ifndef _SPINE_H_
#define _SPINE_H_

#include "kpmp_solution.h"

//
// spine order helpers
//
std::vector<vertex_t> spine_order_ascending( unsigned int num_vertices );

std::vector<vertex_t> spine_order_num_edges( const std::vector<std::vector<unsigned int> > & adjacencyList );



class random_spine_generator
{
public:

    random_spine_generator( unsigned int num_vertices );
    random_spine_generator( const std::vector<vertex_t> & base_spine );
    
    void random_change_block( unsigned int block_size, unsigned int num_random_changes, std::vector<vertex_t> & new_random_vert);
    void add_random_to_spine( unsigned int num, std::vector<vertex_t> & new_random_vert );

    const std::vector<vertex_t> & get_spine() const { return spine; };
    const std::vector<vertex_t> & get_free_vertices() const { return free_vertices; };
    
private:

    vertex_t embed_vertex_to_spine( unsigned int v_index );

//    std::vector< order_t > spine_order_map;
    std::vector<vertex_t> free_vertices;
    std::vector<vertex_t> spine;

};


#endif // _SPINE_H_
