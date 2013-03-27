#include "test_registrator.h"

#include <boost/test/unit_test.hpp>
using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* ts1 = BOOST_TEST_SUITE( "thread_slice_controller tests" );
	boost::unit_test::unit_test_log.set_threshold_level( boost::unit_test::log_messages );

	using namespace system_utilities::tests_::common;
	ts1->add( BOOST_TEST_CASE( &task_processor_constructor_tests ) );
	ts1->add( BOOST_TEST_CASE( &task_processor_add_task_tests ) );
	ts1->add( BOOST_TEST_CASE( &task_processor_wait_tests ) );

	if ( RUN_PERFORMANCE_TESTS )
	{
		ts1->add( BOOST_TEST_CASE( &task_processor_add_task_performace_tests ) );
		ts1->add( BOOST_TEST_CASE( &task_processor_own_allocator_performance_tests ) );
	}

	return ts1;
}
