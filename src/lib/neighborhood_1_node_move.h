#ifndef _NEIGHBORHOOD_1_NODE_MOVE_H_
#define _NEIGHBORHOOD_1_NODE_MOVE_H_


#include "kpmp_solution.h"

class const_neighborhood_1_node_move_iterator ;

const_neighborhood_1_node_move_iterator neighborhood_1_node_move_begin( const solution & init_solution );
const_neighborhood_1_node_move_iterator neighborhood_1_node_move_end( const solution & init_solution ) ;



class const_neighborhood_1_node_move_iterator 
{

public:
    const_neighborhood_1_node_move_iterator( const solution & init_solution );

    virtual solution operator*() const;

    const_neighborhood_1_node_move_iterator & operator++() { increment(); return *this; };

    bool operator==( const const_neighborhood_1_node_move_iterator & rhs ) const;
    bool operator!=( const const_neighborhood_1_node_move_iterator & rhs ) const;

    void set_to_end();

protected:


    void increment();

private:

    bool is_end();

    const solution & init_solution;

    int index_node1, index_node2;
    int spine_size;

};



#endif // _NEIGHBORHOOD_1_NODE_MOVE_H_
