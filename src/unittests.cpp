#include <kpmp_solution.h>
#include <spine.h>


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


BOOST_AUTO_TEST_CASE( random_spine_generator_test )
{
    random_spine_generator rand( 10 );
    std::vector<vertex_t> new_random_vert;
    
    BOOST_CHECK( rand.get_spine().size() == 0 );
    BOOST_CHECK( rand.get_free_vertices().size() == 10 );


    rand.add_random_to_spine( 3, new_random_vert );

    BOOST_CHECK( new_random_vert.size() == 3 );
    BOOST_CHECK( rand.get_spine().size() == 3 );
    BOOST_CHECK( rand.get_free_vertices().size() == 7 );

    rand.add_random_to_spine( 2, new_random_vert );

    BOOST_CHECK( new_random_vert.size() == 2 );
    BOOST_CHECK( rand.get_spine().size() == 5 );
    BOOST_CHECK( rand.get_free_vertices().size() == 5 );

    // one too much
    rand.add_random_to_spine( 6, new_random_vert );
    BOOST_CHECK( rand.get_spine().size() == 10 );
    BOOST_CHECK( rand.get_free_vertices().size() == 0 );
}


BOOST_AUTO_TEST_CASE( random_spine_generator_permutation_test_const )
{
    random_spine_generator rand( 10 );
    std::vector<vertex_t> new_random_vert;
    
    BOOST_CHECK( rand.get_spine().size() == 0 );
    BOOST_CHECK( rand.get_free_vertices().size() == 10 );
 
    // get an unmodified block
    rand.random_change_block( 3, 0, new_random_vert);

    for(unsigned int i=0; i<3; ++i)
        BOOST_CHECK_MESSAGE( new_random_vert[i] == i, "should be " << i << "but is " << new_random_vert[i]  );

    // get a modified one
    rand.random_change_block( 3, 1, new_random_vert);
    int num_matches=0;
    for(unsigned int i=0; i<3; ++i)
        if ( new_random_vert[i] == i+3 )
            num_matches++;

    BOOST_CHECK( num_matches == 1  );
}


BOOST_AUTO_TEST_CASE( random_spine_generator_base_spine )
{
    std::vector< vertex_t > base_spine{3,1,2,0,6,5,4 };
    random_spine_generator rand( base_spine );
    std::vector<vertex_t> new_random_vert;
    
    BOOST_CHECK( rand.get_spine().size() == 0 );
    BOOST_CHECK( rand.get_free_vertices().size() == 7 );
 
    // get an unmodified block
    rand.random_change_block( 3, 0, new_random_vert);

    for( int i= 0; i<3; ++i )
        BOOST_CHECK( new_random_vert[i] == base_spine[i] );

    // get a modified one
    rand.random_change_block( 3, 1, new_random_vert);
    int num_matches=0;
    for(unsigned int i=0; i<3; ++i)
        if ( new_random_vert[i] == base_spine[i+3] )
            num_matches++;

    BOOST_CHECK( num_matches == 1  );

}


