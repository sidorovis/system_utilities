#include "test_registrator.h"

#include <boost/test/unit_test.hpp>

TEST_INIT
{
	
	boost::unit_test_framework::test_suite& master_test_suite = boost::unit_test::framework::master_test_suite();

	using namespace system_utilities::tests_::common;

	master_test_suite.add( BOOST_TEST_CASE( &logger_constructor_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &logger_write_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &logger_flush_tests ) );

	master_test_suite.add( BOOST_TEST_CASE( &logger_note_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &logger_formatted_note_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &logger_warn_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &logger_formatted_warn_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &logger_error_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &logger_formatted_error_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &logger_debug_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &logger_formatted_debug_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &logger_fatal_tests ) );

#ifdef RUN_PERFORMANCE_TESTS
	master_test_suite.add( BOOST_TEST_CASE( &logger_write_performance_tests ) );
#endif 

	return TEST_RETURN;
}

DEFINE_MAIN_FOR_DYNAMIC
