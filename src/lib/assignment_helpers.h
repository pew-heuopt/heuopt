#ifndef _ASSIGNMENT_HELPERS_H_
#define _ASSIGNMENT_HELPERS_H_

#include <ostream>

#include <boost/program_options.hpp>

#include "kpmp_solution.h"



int common_cmdline( int argc, 
                    char ** argv, 
                     
                    boost::program_options::options_description & desc, 
                    boost::program_options::variables_map & vm,

                    std::string & input_filename,
                    std::string & output_filename );


//
// edge page allocation
//

void insert_on_next_crossfree_page(solution & sol, const edge_t & e );


//
// output
//
void write_solution( std::ostream & out, const solution & sol );

void write_solution_statistics(  std::ostream & out, const solution & sol );



#endif // _ASSIGNMENT_HELPERS_H_

