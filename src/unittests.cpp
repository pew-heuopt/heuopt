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


BOOST_AUTO_TEST_CASE( vertex_remove )
{
    solution sol(2, std::vector< vertex_t > {0,1,2,3}  );


    sol.add_edge( 0, edge_t(0,2) );

    BOOST_CHECK( sol.get_crossings() == 0 );

    sol.add_edge( 0, edge_t(1,3) );

    BOOST_CHECK( sol.get_crossings() == 1 );


    sol.remove_edge( 0, edge_t(0,2) );
    BOOST_CHECK( sol.get_crossings() == 0 );
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


//
// spine swap
//
BOOST_AUTO_TEST_CASE( spine_swap )
{
    solution sol(2, std::vector< vertex_t > {0,1,2,3}  );

    sol.add_edge( 0, edge_t(0,2) );
    sol.add_edge( 0, edge_t(1,3) );

    BOOST_CHECK( sol.get_crossings() == 1 );

    sol.swap_vertices( 1, 2 );
    
    BOOST_CHECK( sol.get_crossings() == 0 );

    sol.swap_vertices( 1, 2 );
    
    BOOST_CHECK( sol.get_crossings() == 1 );
}



//
// neighborhood_1_edge_move iterator
//

#include <neighborhood_1_edge_move.h>

BOOST_AUTO_TEST_CASE( neighborhood_1_edge_move_single_page  )
{
    solution sol(1, std::vector< vertex_t > {0,1,2,3}  );

    sol.add_edge( 0, edge_t(0,2) );
    sol.add_edge( 0, edge_t(1,3) );
   

    unsigned num_iterations= 0;
    for( auto i= neighborhood_1_edge_move_begin( sol ); 
         i != neighborhood_1_edge_move_end( sol ); 
         ++i )
    {
        ++num_iterations;
    }

    BOOST_CHECK_MESSAGE( num_iterations == 0, "should be " << 0 << "but is " << num_iterations  );
}

BOOST_AUTO_TEST_CASE( neighborhood_1_edge_move_empty_page  )
{
    solution sol(2, std::vector< vertex_t > {0,1,2,3}  );

    sol.add_edge( 0, edge_t(0,2) );
    sol.add_edge( 0, edge_t(1,3) );
   
    BOOST_CHECK( sol.get_crossings() == 1 );

/*
    auto i= neighborhood_1_edge_move_begin( sol );

    std::cout << "1: " << *i << std::endl;
    ++i;
    std::cout << "2: " << *i << std::endl;
    ++i;

    auto end= neighborhood_1_edge_move_end( sol );

    //auto end= neighborhood_1_edge_move_end( sol );

    std::cout << "equal: " << (i == end) << std::endl;
*/
    unsigned num_iterations= 0;
    for( auto i= neighborhood_1_edge_move_begin( sol ); 
         i != neighborhood_1_edge_move_end( sol ); 
         ++i )
    {
        ++num_iterations;
        BOOST_CHECK( (*i).get_crossings() == 0 );
    }
    
    BOOST_CHECK_MESSAGE( num_iterations == 2, "should be " << 2 << "but is " << num_iterations  );


    auto first_iteration= neighborhood_1_edge_move_begin( sol );

    BOOST_CHECK( (*first_iteration).page_begin()[0].edges.size() == 1 );
    BOOST_CHECK( (*first_iteration).page_begin()[1].edges.size() == 1 );

    BOOST_CHECK( (*first_iteration).page_begin()[0].edges.begin()->first == 1 );
    BOOST_CHECK( (*first_iteration).page_begin()[0].edges.begin()->second == 3 );

    BOOST_CHECK( (*first_iteration).page_begin()[1].edges.begin()->first == 0 );
    BOOST_CHECK( (*first_iteration).page_begin()[1].edges.begin()->second == 2 );

}


BOOST_AUTO_TEST_CASE( neighborhood_1_edge_move_multiple_full_pages  )
{
    solution sol(4, std::vector< vertex_t > {0,1,2,3,4,5}  );

    sol.add_edge( 0, edge_t(0,4) );
    sol.add_edge( 1, edge_t(1,5) );
    sol.add_edge( 2, edge_t(2,5) );
    sol.add_edge( 3, edge_t(3,5) );
   
    BOOST_CHECK( sol.get_crossings() == 0 );

    unsigned num_iterations= 0;
    for( auto i= neighborhood_1_edge_move_begin( sol ); 
         i != neighborhood_1_edge_move_end( sol ); 
         ++i )
    {
        if( num_iterations < 3 )
            BOOST_CHECK( (*i).get_crossings() == 1 );
        else if( num_iterations == 3 )
            BOOST_CHECK( (*i).get_crossings() == 1 );
        else if( num_iterations == 6 )
            BOOST_CHECK( (*i).get_crossings() == 1 );
        else if( num_iterations == 9 )
            BOOST_CHECK( (*i).get_crossings() == 1 );
        else
            BOOST_CHECK( (*i).get_crossings() == 0 );

        ++num_iterations;
    }
    
    BOOST_CHECK_MESSAGE( num_iterations == 12, "should be " << 12 << "but is " << num_iterations  );



}



//
// neighborhood_1_node_flip iterator
//

#include <neighborhood_1_node_flip.h>

BOOST_AUTO_TEST_CASE( neighborhood_1_node_flip  )
{
    solution sol(1, std::vector< vertex_t > {0,1,2,3}  );

    sol.add_edge( 0, edge_t(0,2) );
    sol.add_edge( 0, edge_t(1,3) );
   

    unsigned num_iterations= 0;
    for( auto i= neighborhood_1_node_flip_begin( sol ); 
         i != neighborhood_1_node_flip_end( sol ); 
         ++i )
    {
        solution s= *i;

        if( num_iterations == 0 ||
            num_iterations == 2 ||
            num_iterations == 3 ||
            num_iterations == 5 )
        {
            BOOST_CHECK( s.get_crossings() == 0 );
        }
        else
            BOOST_CHECK( s.get_crossings() == 1 );

        ++num_iterations;
    }

    BOOST_CHECK_MESSAGE( num_iterations == 6, "should be " << 6 << "but is " << num_iterations  );
}

//
// stepfunction tests
//
//

#include <stepfunctions.h>

BOOST_AUTO_TEST_CASE( stepfunction_test  )
{

    solution sol(5, std::vector< vertex_t > {0,1,2,3,4,5,6,7}  );

    sol.add_edge( 0, edge_t(0,6) );
    sol.add_edge( 0, edge_t(1,7) );
    sol.add_edge( 0, edge_t(2,7) );
    sol.add_edge( 1, edge_t(3,7) );
    sol.add_edge( 1, edge_t(4,7) );
    sol.add_edge( 2, edge_t(5,7) );

    timer t(0);

    solution first_improvement_sol= 
        
            stepfunction::first_improvement( neighborhood_1_edge_move_begin( sol ), 
                                             neighborhood_1_edge_move_end( sol ), t );

    BOOST_CHECK_MESSAGE( first_improvement_sol.get_crossings() == 1, "should be 1  but is " << first_improvement_sol.get_crossings( ) );

    solution best_improvement_sol= 
        
            stepfunction::best_improvement( neighborhood_1_edge_move_begin( sol ), 
                                             neighborhood_1_edge_move_end( sol ), t );

    BOOST_CHECK_MESSAGE( best_improvement_sol.get_crossings() == 0, "should be 0  but is " << best_improvement_sol.get_crossings( ) );

}
 
