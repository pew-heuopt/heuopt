#include <iostream>
#include <memory>

#include <kpmp_instance.h>
#include <kpmp_solution.h>



int main( int argc, char **argv)
{

    if( argc != 2 )
    {
        std::cerr << "usage: " << argv[0] << " <instance-file>" << std::endl;
        return 1;
    }

    std::string instance_file( argv[1] );

    std::auto_ptr<KPMPInstance> instance( KPMPInstance::readInstance( instance_file ) );

    // node order
    // start with arbitrary edge because of shift property there is no better one
    
    // add all nodes that are connected to this 


    return 0;
}
