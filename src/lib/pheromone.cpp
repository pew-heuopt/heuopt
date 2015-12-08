
#include <boost/shared_ptr.hpp>

#include "pheromone.h"


/*************************************************************************************
 *
 * pheromone_matrix_edge
 *
*************************************************************************************/

double pheromone_matrix_edge::get_pheromone( const pathsegment_edge & pe) const
{
    auto ph_page_it= pheromone_tree.find( pe.page );

    if( ph_page_it == pheromone_tree.end() )
        return initial_val;

    vertex_t v1= std::min( pe.edge.first, pe.edge.second ) ;
    vertex_t v2= std::max( pe.edge.first, pe.edge.second ) ;

    auto ph_v1_it= ph_page_it->second.find( v1 );

    if( ph_v1_it == ph_page_it->second.end() )
        return initial_val;

    auto ph_v2_it= ph_v1_it->second.find( v2 );

    if( ph_v2_it == ph_v1_it->second.end() )
        return initial_val;


    return ph_v2_it->second;
}


    
void pheromone_matrix_edge::apply_pheromones( const solution & sol, double pheromone )
{
    int page_num= 0;

    for( auto page_it= sol.page_begin(); page_it != sol.page_end(); ++page_it )
    {
        for( auto edge_it= page_it->edges.begin(); 
             edge_it != page_it->edges.end(); ++edge_it )
        {
            double & val_writer= get_phero_writer( page_num, *edge_it );
            val_writer+= pheromone;
        }

        page_num++;
    }
}
 

void pheromone_matrix_edge::evaporate( double evaporation_rate )
{
    for( auto page_it=  pheromone_tree.begin(); page_it !=pheromone_tree.end(); ++page_it )
    {
        for( auto ph_v1_it=  page_it->second.begin(); ph_v1_it != page_it->second.end(); ++ph_v1_it )
        {
            for( auto ph_v2_it=  ph_v1_it->second.begin(); ph_v2_it != ph_v1_it->second.end(); ++ph_v2_it )
            {
                ph_v2_it->second*= 1.0-evaporation_rate;
            }
        }
    }
}


double & pheromone_matrix_edge::get_phero_writer(int page_num, const edge_t & e ) 
{
    vertex_t v1= std::min( e.first, e.second ) ;
    vertex_t v2= std::max( e.first, e.second ) ;

    auto ph_page_it= pheromone_tree.find( page_num );

    if( ph_page_it == pheromone_tree.end() )
    {
        double & val= pheromone_tree[page_num][v1][v2];
        val= initial_val;
        return val;
    }    


    auto ph_v1_it= ph_page_it->second.find( v1 );

    if( ph_v1_it == ph_page_it->second.end() )
    {
        double & val= pheromone_tree[page_num][v1][v2];
        val= initial_val;
        return val;
    }

    auto ph_v2_it= ph_v1_it->second.find( v2 );

    if( ph_v2_it == ph_v1_it->second.end() )
    {
        double & val= pheromone_tree[page_num][v1][v2];
        val= initial_val;
        return val;
    }

    return ph_v2_it->second;
}


std::ostream& operator<<(std::ostream& os, const pheromone_matrix_edge & edge_matrix)
{


    for( auto page_it=  edge_matrix.pheromone_tree.begin(); page_it !=edge_matrix.pheromone_tree.end(); ++page_it )
    {
        os << page_it->first << ": ";

        for( auto ph_v1_it=  page_it->second.begin(); ph_v1_it != page_it->second.end(); ++ph_v1_it )
        {
            for( auto ph_v2_it=  ph_v1_it->second.begin(); ph_v2_it != ph_v1_it->second.end(); ++ph_v2_it )
            {
                os << " (" << ph_v1_it->first << ":" << ph_v2_it->first << "):" << ph_v2_it->second;
            }
        }

        os<<std::endl;
    }

   return os; 
}


/*************************************************************************************
 *
 * pheromone_matrix_vertex
 *
*************************************************************************************/


double pheromone_matrix_vertex::get_pheromone( const pathsegment_vertex & pv) const
{

    auto ph_from_it= pheromone_tree.find( pv.from );

    if( ph_from_it == pheromone_tree.end() )
        return initial_val;

    auto ph_to_it= ph_from_it->second.find( pv.to );

    if( ph_to_it == ph_from_it->second.end() )
        return initial_val;


    return ph_to_it->second;
}


    

void pheromone_matrix_vertex::apply_pheromones( const solution & sol, double pheromone )
{
    auto spine_order= sol.get_spine_order();

    for( auto spine_it= spine_order.begin();  spine_it != spine_order.end(); ++spine_it )
    {
        
        auto next_spine_it= spine_it ; 
        next_spine_it++;

        double & overflow_phero= ( next_spine_it == spine_order.end() ) ? 

                                 // special case: back and front when we have an overflow
                                 get_phero_writer( *spine_it, spine_order.front() )
                                 :
                                 get_phero_writer( *spine_it, *next_spine_it )    
                                     ;
        overflow_phero= pheromone;
    }
}
 


void pheromone_matrix_vertex::evaporate( double evaporation_rate )
{
    for( auto ph_from_it=  pheromone_tree.begin(); ph_from_it != pheromone_tree.end(); ++ph_from_it )
    {
        for( auto ph_to_it=  ph_from_it->second.begin(); ph_to_it != ph_from_it->second.end(); ++ph_to_it )
        {
            ph_to_it->second*= 1.0-evaporation_rate;
        }
    }
}



double & pheromone_matrix_vertex::get_phero_writer( const vertex_t & from,  const vertex_t & to ) 
{
    auto from_it= pheromone_tree.find( from );

    if( from_it == pheromone_tree.end() )
    {
        double & val= pheromone_tree[from][to];
        val= initial_val;
        return val;
    }    


    auto to_it= from_it->second.find( to );

    if( to_it == from_it->second.end() )
    {
        double & val= pheromone_tree[from][to];
        val= initial_val;
        return val;
    }

    return to_it->second;
}


std::ostream& operator<<(std::ostream& os, const pheromone_matrix_vertex & vertex_matrix)
{

    for( auto ph_from_it=  vertex_matrix.pheromone_tree.begin(); ph_from_it != vertex_matrix.pheromone_tree.end(); ++ph_from_it )
    {
        for( auto ph_to_it=  ph_from_it->second.begin(); ph_to_it != ph_from_it->second.end(); ++ph_to_it )
        {
            os << " (" << ph_from_it->first << ":" << ph_to_it->first << "):" << ph_to_it->second;
        }
    }

    os<<std::endl;

   return os; 
}




/*************************************************************************************
 *
 * pheromone algorithms
 *
*************************************************************************************/

void pheromone_update( const std::vector< boost::shared_ptr<solution> > & solutions, 
                       pheromone_matrix_vertex & matrix_vertex,
                       pheromone_matrix_edge & matrix_edge,
                       double initial_val )

{

    // calculate the max and sum of crossings
    // we will use this 
    int max_crossings= 0, sum_crossings = 0;

    for( auto sol_it= solutions.begin(); sol_it!= solutions.end(); ++sol_it)
    {
        int cross= (*sol_it)->get_crossings();
        sum_crossings+= cross;

        if( cross > max_crossings )
            max_crossings= cross;
    }


#ifdef DEBUG    
std::cout << "phero: max: " << max_crossings << " sum:" << sum_crossings << std::endl;
#endif // DEBUG

    // for all solutions we can now calculate the pheromone level
    // and apply it to the respective path
    for( auto sol_it= solutions.begin(); sol_it!= solutions.end(); ++sol_it)
    {
        double phero= ( sum_crossings == 0 ) ?
                      initial_val
                      :
                      (max_crossings - (*sol_it)->get_crossings() ) / double(max_crossings);

        phero= phero * phero * phero * phero;

#ifdef DEBUG    
std::cout << "phero: spreading " << phero << std::endl; 
#endif // DEBUG

        matrix_edge.apply_pheromones( **sol_it, phero );
        matrix_vertex.apply_pheromones( **sol_it, phero );
    }



    // TODO: reset inital value after that?
}
