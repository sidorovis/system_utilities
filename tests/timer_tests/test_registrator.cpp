#include "test_registrator.h"

#include <boost/test/unit_test.hpp>

TEST_INIT
{
	boost::unit_test_framework::test_suite& master_test_suite = boost::unit_test::framework::master_test_suite();

	using namespace system_utilities::tests_::common;

	master_test_suite.add( BOOST_TEST_CASE( &timer_constructor_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &timer_begin_end_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &timer_method_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &timer_object_method_tests ) );

	return TEST_RETURN;
}

DEFINE_MAIN_FOR_DYNAMIC
