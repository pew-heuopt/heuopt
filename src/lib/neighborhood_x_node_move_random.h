#ifndef _NEIGHBORHOOD_X_NODE_MOVE_RANDOM_H_
#define _NEIGHBORHOOD_X_NODE_MOVE_RANDOM_H_


#include "kpmp_solution.h"

class const_neighborhood_x_node_move_random_iterator ;

const_neighborhood_x_node_move_random_iterator neighborhood_x_node_move_random_begin( const solution & init_solution, int num_random_moves );
const_neighborhood_x_node_move_random_iterator neighborhood_x_node_move_random_end( const solution & init_solution, int num_random_moves ) ;



class const_neighborhood_x_node_move_random_iterator 
{

public:
    const_neighborhood_x_node_move_random_iterator( const solution & init_solution, int num_random_moves );

    virtual solution operator*() const;

    const_neighborhood_x_node_move_random_iterator & operator++() { increment(); return *this; };

    bool operator==( const const_neighborhood_x_node_move_random_iterator & rhs ) const;
    bool operator!=( const const_neighborhood_x_node_move_random_iterator & rhs ) const;

    void set_to_end();

protected:


    void increment();

private:

    bool is_end();

    const solution & init_solution;

    int num_random_moves;
    int spine_size;

};



#endif // _NEIGHBORHOOD_X_NODE_MOVE_RANDOM_H_
