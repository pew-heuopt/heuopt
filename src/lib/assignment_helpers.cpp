
#include "assignment_helpers.h"

#include "kpmp_solution_writer.h"




int common_cmdline( int argc, 
                     char ** argv, 
                     
                     boost::program_options::options_description & desc, 
                     boost::program_options::variables_map & vm,

                     std::string & input_filename,
                     std::string & output_filename )
{
    namespace po= boost::program_options ;

    desc.add_options()
         ("help", "produce help message")
         ("input", boost::program_options::value<std::string>(&input_filename), "instance input file")
         ("output", boost::program_options::value<std::string>(&output_filename), "instance output file")
         ;

    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);    

        

     if( vm.count("help") ) 
     {
         std::cerr << desc << std::endl;
         return 1;
     }


     if( ! vm.count("input" )  )
     {
         std::cerr << "input file not given" << std::endl
                   << desc << std::endl;
         return 1;
     }
    
     if( ! vm.count("output" )  )
     {
         std::cerr << "output file not given" << std::endl
                   << desc << std::endl;
         return 1;
     }
 
    return 0;
}



//
// edge page allocation
//



void insert_on_next_crossfree_page(solution & sol, const edge_t & e )
{

    int num_pages= sol.get_pages();

#ifdef DEBUG
    std::cout << "trying edge: " << e.first << " " << e.second << std::endl;
#endif // DEBUG

    int crossings;
    int min_crossings;
    int min_index;

    for( int i=0; i<num_pages; ++i )
    {
        crossings= sol.try_num_crossing(i, e);

        if( crossings == 0  )
        {
            sol.add_edge( i, e, crossings );
#ifdef DEBUG
            std::cout << "    0 crossing found inserting on page " << i << std::endl;
#endif // DEBUG
            return;
        }

#ifdef DEBUG
        std::cout << "    crossings on page " << i << ": " << crossings << std::endl;
#endif // DEBUG
        if( i == 0 || 
            crossings < min_crossings)
        {
            min_crossings= crossings;
            min_index= i;
        }



    }

#ifdef DEBUG
    std::cout << "    no zero found, inserting with " << min_crossings << " on page " << min_index << std::endl ;
#endif // DEBUG

    sol.add_edge( min_index, e, min_crossings );

}




void write_solution( ostream & out, const solution & sol )
{    
    KPMPSolutionWriter writer( sol.get_pages() );

    writer.setSpineOrder( sol.get_spine_order() );


    unsigned int page= 0;
    for( auto page_it= sol.page_begin(); page_it != sol.page_end(); ++page_it )
    {
        for( auto edge_it= page_it->edges.begin(); edge_it != page_it->edges.end(); ++edge_it )
	        writer.addEdgeOnPage(edge_it->first, edge_it->second, page);

        page++;
    }

    writer.write(out);
}



void write_solution_statistics(  ostream & out, const solution & sol )
{
    //
    // solution stat
    //
    out << "statistics" << std::endl
        << "==========" << std::endl;
    

    unsigned int page= 0;
    int edges_sum= 0;
    for( auto page_it= sol.page_begin(); page_it != sol.page_end(); ++page_it )
    {
        out << "  crossings page  " << page << ": " << page_it->crossings << " (" << page_it->edges.size()<< " edges)" << std::endl;
        page++;
        edges_sum+= page_it->edges.size();
    }

    out << "crossings sum: " << sol.get_crossings() << " (" << edges_sum << " edges)" << std::endl;

}
