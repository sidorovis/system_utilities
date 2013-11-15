#include "test_registrator.h"

#include <boost/test/unit_test.hpp>

TEST_INIT
{
	boost::unit_test_framework::test_suite& master_test_suite = boost::unit_test::framework::master_test_suite();

	using namespace system_utilities::tests_::common;

	master_test_suite.add( BOOST_TEST_CASE( &shared_name_storage_constructor_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &shared_name_storage_add_name_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &shared_name_storage_operator_in_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &shared_name_storage_get_shared_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &shared_name_storage_check_shared_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &shared_name_storage_iterator_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &shared_name_storage_clear_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &shared_name_storage_del_name_tests ) );

	return TEST_RETURN;
}

DEFINE_MAIN_FOR_DYNAMIC
