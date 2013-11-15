#include "test_registrator.h"

#include <boost/test/unit_test.hpp>

TEST_INIT
{
	boost::unit_test_framework::test_suite& master_test_suite = boost::unit_test::framework::master_test_suite();

	using namespace system_utilities::tests_::common;

	master_test_suite.add( BOOST_TEST_CASE( &windows_service_ctor_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &windows_service_install_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &windows_service_start_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &windows_service_stop_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &windows_service_uninstall_tests ) );

	return TEST_RETURN;
}

DEFINE_MAIN_FOR_DYNAMIC
