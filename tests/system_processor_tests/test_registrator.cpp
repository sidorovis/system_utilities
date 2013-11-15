#include "test_registrator.h"

#include <boost/test/unit_test.hpp>

TEST_INIT
{
	boost::unit_test_framework::test_suite& master_test_suite = boost::unit_test::framework::master_test_suite();

	using namespace system_utilities::tests_::common;
	
	master_test_suite.add( BOOST_TEST_CASE( &system_processor_constructor_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &system_processor_init_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &system_processor_stop_wait_stop_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &system_processor_exit_handler_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &system_processor_logs_path_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &system_processor_config_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &system_processor_set_config_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &system_processor_config_values_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &system_processor_config_reset_value_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &system_processor_config_delete_value_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &system_processor_config_rename_parameter_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &system_processor_config_check_value_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &system_processor_create_log_tests ) );

#ifdef RUN_PERFORMANCE_TESTS
#endif 

	return TEST_RETURN;
}

DEFINE_MAIN_FOR_DYNAMIC
