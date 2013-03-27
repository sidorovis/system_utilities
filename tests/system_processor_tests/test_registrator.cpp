#include "test_registrator.h"

#include <boost/test/unit_test.hpp>
using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* ts1 = BOOST_TEST_SUITE( "system_processor tests" );
	boost::unit_test::unit_test_log.set_threshold_level( boost::unit_test::log_messages );

	using namespace system_utilities::tests_::common;
	ts1->add( BOOST_TEST_CASE( &system_processor_constructor_tests ) );
	ts1->add( BOOST_TEST_CASE( &system_processor_init_tests ) );
	ts1->add( BOOST_TEST_CASE( &system_processor_stop_wait_stop_tests ) );
	ts1->add( BOOST_TEST_CASE( &system_processor_exit_handler_tests ) );
	ts1->add( BOOST_TEST_CASE( &system_processor_logs_path_tests ) );
	ts1->add( BOOST_TEST_CASE( &system_processor_config_tests ) );
	ts1->add( BOOST_TEST_CASE( &system_processor_set_config_tests ) );
	ts1->add( BOOST_TEST_CASE( &system_processor_config_values_tests ) );
	ts1->add( BOOST_TEST_CASE( &system_processor_config_reset_value_tests ) );
	ts1->add( BOOST_TEST_CASE( &system_processor_config_delete_value_tests ) );
	ts1->add( BOOST_TEST_CASE( &system_processor_config_rename_parameter_tests ) );
	ts1->add( BOOST_TEST_CASE( &system_processor_config_check_value_tests ) );
	ts1->add( BOOST_TEST_CASE( &system_processor_create_log_tests ) );

	if ( RUN_PERFORMANCE_TESTS )
	{
	}

	return ts1;
}
