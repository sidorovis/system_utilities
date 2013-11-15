#include "test_registrator.h"

#include <boost/test/unit_test.hpp>

TEST_INIT
{
	
	boost::unit_test_framework::test_suite& master_test_suite = boost::unit_test::framework::master_test_suite();

	using namespace system_utilities::tests_::common;

	master_test_suite.add( BOOST_TEST_CASE( &multikey_tree_node_smoke_test ) );
	master_test_suite.add( BOOST_TEST_CASE( &multikey_tree_with_map_comparator_tests ) );
	
#ifdef RUN_PERFORMANCE_TESTS
	master_test_suite.add( BOOST_TEST_CASE( &multikey_tree_node_smoke_performance_big_name_test ) );
	master_test_suite.add( BOOST_TEST_CASE( &multikey_tree_node_smoke_performance_many_signals_test ) );
	master_test_suite.add( BOOST_TEST_CASE( &multikey_tree_node_smoke_performance_many_threads_test ) );
#endif 

	return TEST_RETURN;
}

DEFINE_MAIN_FOR_DYNAMIC

