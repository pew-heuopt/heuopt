#ifndef _NEIGHBORHOOD_1_NODE_EDGE_MOVE_H_
#define _NEIGHBORHOOD_1_NODE_EDGE_MOVE_H_


#include "kpmp_solution.h"

class const_neighborhood_1_node_edge_move_iterator ;

const_neighborhood_1_node_edge_move_iterator neighborhood_1_node_edge_move_begin( const solution & init_solution );
const_neighborhood_1_node_edge_move_iterator neighborhood_1_node_edge_move_end( const solution & init_solution ) ;



class const_neighborhood_1_node_edge_move_iterator 
{

public:
    const_neighborhood_1_node_edge_move_iterator( const solution & init_solution );

    virtual solution operator*() const;

    const_neighborhood_1_node_edge_move_iterator & operator++() { increment(); return *this; };

    bool operator==( const const_neighborhood_1_node_edge_move_iterator & rhs ) const;
    bool operator!=( const const_neighborhood_1_node_edge_move_iterator & rhs ) const;

    void set_to_end();

protected:


    void increment();

private:

    bool is_end();
    void increment_current_node();

    void start_new_target_iteration();
    bool current_and_target_same_page();

    const solution & init_solution;

    solution solution_without_current_node_edges;
    std::list<edge_t> current_node_edges;

    solution::const_page_iterator_t current_page;
    solution::const_vertex_iterator_t current_node;

    int target_page;

};



#endif // _NEIGHBORHOOD_1_NODE_EDGE_MOVE_H_
