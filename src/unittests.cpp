#include <kpmp_solution.h>


#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE KPMP_testsuite


#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE( vertex_spine_order_check )
{
    solution sol(1, std::vector< vertex_t > {3,1,2,0}  );

    sol.add_edge( 0, edge_t(2,0) );
    sol.add_edge( 0, edge_t(1,3) );

    // if spine order is correct this is no crossing
    BOOST_CHECK( sol.get_crossings() == 0 );

    sol.add_edge( 0, edge_t(3,2) );

    BOOST_CHECK( sol.get_crossings() == 0 );

    sol.add_edge( 0, edge_t(1,0) );

    BOOST_CHECK( sol.get_crossings() == 1 );
}


BOOST_AUTO_TEST_CASE( crossing_test_num_crossings_and_pages )
{
    solution sol(3, std::vector< vertex_t > {0,1,2,3}  );

    
    // we try this on all pages
    for( int i= 0; i<3; ++i )
    {
        BOOST_CHECK( sol.try_num_crossing(i, edge_t(0,2) ) == 0 );

        sol.add_edge( i, edge_t(0,2) );

        BOOST_CHECK( sol.try_num_crossing(i, edge_t(1,3) ) == 1 );

        sol.add_edge( i, edge_t(1,3) );
    }
   

    // try correct page sum of crossings
    BOOST_CHECK( sol.get_crossings() == 3 );
}
