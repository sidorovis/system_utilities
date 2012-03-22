#include "test_registrator.h"

#include <boost/test/unit_test.hpp>
using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* ts1 = BOOST_TEST_SUITE( "ts_queue tests" );
	boost::unit_test::unit_test_log.set_threshold_level( boost::unit_test::log_messages );

	using namespace system_utilities::tests_::common;
	ts1->add( BOOST_TEST_CASE( &ts_queue_constructor_tests ) );
	ts1->add( BOOST_TEST_CASE( &ts_queue_different_threads_tests ) );
	ts1->add( BOOST_TEST_CASE( &ts_queue_wait_pop_tests ) );
	ts1->add( BOOST_TEST_CASE( &ts_queue_another_container_tests ) );

#ifdef RUN_PERFORMANCE_TESTS
	ts1->add( BOOST_TEST_CASE( &ts_queue_many_threads_tests ) );
#endif

	return ts1;
}
