
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
