#include "test_registrator.h"

#include <boost/test/unit_test.hpp>

using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* ts1 = BOOST_TEST_SUITE( "shared name storage tests" );
	boost::unit_test::unit_test_log.set_threshold_level( boost::unit_test::log_messages );

	using namespace system_utilities::tests_::common;

	ts1->add( BOOST_TEST_CASE( &shared_name_storage_constructor_tests ) );
	ts1->add( BOOST_TEST_CASE( &shared_name_storage_add_name_tests ) );
	ts1->add( BOOST_TEST_CASE( &shared_name_storage_operator_in_tests ) );
	ts1->add( BOOST_TEST_CASE( &shared_name_storage_get_shared_tests ) );
	ts1->add( BOOST_TEST_CASE( &shared_name_storage_check_shared_tests ) );
	ts1->add( BOOST_TEST_CASE( &shared_name_storage_iterator_tests ) );
	ts1->add( BOOST_TEST_CASE( &shared_name_storage_clear_tests ) );
	ts1->add( BOOST_TEST_CASE( &shared_name_storage_del_name_tests ) );

	return ts1;
}
