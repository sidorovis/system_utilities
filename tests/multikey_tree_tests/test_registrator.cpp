#include "test_registrator.h"

#include <boost/test/unit_test.hpp>
using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* ts1 = BOOST_TEST_SUITE( "multikey_tree tests" );
	boost::unit_test::unit_test_log.set_threshold_level( boost::unit_test::log_messages );

	using namespace system_utilities::tests_::common;
	ts1->add( BOOST_TEST_CASE( &multikey_tree_node_smoke_test ) );
	ts1->add( BOOST_TEST_CASE( &multikey_tree_with_map_comparator_tests ) );

	
#ifdef RUN_PERFORMANCE_TESTS
	ts1->add( BOOST_TEST_CASE( &multikey_tree_node_smoke_performance_big_name_test ) );
	ts1->add( BOOST_TEST_CASE( &multikey_tree_node_smoke_performance_many_signals_test ) );
	ts1->add( BOOST_TEST_CASE( &multikey_tree_node_smoke_performance_many_threads_test ) );
#endif 

	return ts1;
}
