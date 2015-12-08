#ifndef _PHEROMONE_H_
#define _PHEROMONE_H_

#include <map>
#include <ostream>
#include "kpmp_solution.h"

struct pathsegment
{
   pathsegment( int _cost )
    : cost(_cost), 
      weight(0.0)
    {
    };

    double cost; 
    double weight;
};


struct pathsegment_edge : pathsegment
{
    pathsegment_edge( int _page, edge_t _edge, int _cost )
    : pathsegment(_cost),
      page(_page),
      edge(_edge)
    {
    };

    int page;
    edge_t edge; 
};


struct pathsegment_vertex : pathsegment
{
    pathsegment_vertex( vertex_t _from, vertex_t _to, int _cost )
    : pathsegment(_cost),
      from(_from),
      to(_to)
    {
    };

    vertex_t from,to;
};



class pheromone_matrix
{
public:    
    pheromone_matrix(double _initial_val, double _alpha, double _beta)
    : alpha(_alpha), 
      beta(_beta),
      initial_val(_initial_val)
    {
    }

    const double alpha, beta;

protected:

    double initial_val;
};


class pheromone_matrix_edge : public pheromone_matrix
{
public:    
    pheromone_matrix_edge(double initial_val, double alpha, double beta)
    : pheromone_matrix( initial_val, alpha, beta )
    {
    }

    double get_pheromone( const pathsegment_edge & pe) const;

    void apply_pheromones( const solution & sol, double pheromone );
    void evaporate( double evaporation_rate );


    friend std::ostream& operator<<(std::ostream& os, const pheromone_matrix_edge & edge_matrix);

private:

    double & get_phero_writer(int page_num, const edge_t & e ) ;


    std::map<int, std::map<vertex_t, std::map<vertex_t, double> > > pheromone_tree;
};



std::ostream& operator<<(std::ostream& os, const pheromone_matrix_edge & edge_matrix);





class pheromone_matrix_vertex : public pheromone_matrix
{

public:    
    pheromone_matrix_vertex(double initial_val, double alpha, double beta)
    : pheromone_matrix( initial_val, alpha, beta )
    {
    }

    double get_pheromone(const pathsegment_vertex & pv) const;

    void apply_pheromones( const solution & sol, double pheromone );
    void evaporate( double evaporation_rate );


    friend std::ostream& operator<<(std::ostream& os, const pheromone_matrix_vertex & vertex_matrix);

private:

    double & get_phero_writer( const vertex_t & from,  const vertex_t & to ) ;

    double initial_val;
    std::map<vertex_t, std::map<vertex_t, double> > pheromone_tree;
};


std::ostream& operator<<(std::ostream& os, const pheromone_matrix_vertex & vertex_matrix);


void pheromone_update( const std::vector< boost::shared_ptr<solution> > & solutions, 
                       pheromone_matrix_vertex & matrix_vertex,
                       pheromone_matrix_edge & matrix_edge,
                       double initial_val );




#endif // _PHEROMONE_H_
