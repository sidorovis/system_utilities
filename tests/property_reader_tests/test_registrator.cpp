#include "test_registrator.h"

#include <boost/test/unit_test.hpp>
using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* ts1 = BOOST_TEST_SUITE( "property_reader tests" );
	boost::unit_test::unit_test_log.set_threshold_level( boost::unit_test::log_messages );

	using namespace system_utilities::tests_::common;
	ts1->add( BOOST_TEST_CASE( &property_reader_constructor_tests ) );
	ts1->add( BOOST_TEST_CASE( &property_reader_parse_istream_tests ) );
	ts1->add( BOOST_TEST_CASE( &property_reader_delete_comment_tests ) );
	//
	ts1->add( BOOST_TEST_CASE( &property_reader_test_on_include_tests ) );
	ts1->add( BOOST_TEST_CASE( &property_reader_include_line_tests ) );
	ts1->add( BOOST_TEST_CASE( &property_reader_string_with_setting_tests ) );
	ts1->add( BOOST_TEST_CASE( &property_reader_additional_string_with_setting_tests ) );
	ts1->add( BOOST_TEST_CASE( &property_reader_read_sub_property_file_tests ) );
	ts1->add( BOOST_TEST_CASE( &property_reader_parse_line_tests ) );
	//
	ts1->add( BOOST_TEST_CASE( &property_reader_comment_tests ) );
	ts1->add( BOOST_TEST_CASE( &property_reader_trim_tests ) );
	ts1->add( BOOST_TEST_CASE( &property_reader_const_trim_tests ) );
	ts1->add( BOOST_TEST_CASE( &property_reader_split_tests ) );
	ts1->add( BOOST_TEST_CASE( &property_reader_comment_tests ) );
	ts1->add( BOOST_TEST_CASE( &property_reader_small_amount_of_parameters_tests ) );
	//
	ts1->add( BOOST_TEST_CASE( &property_reader_size_tests ) );
	ts1->add( BOOST_TEST_CASE( &property_reader_get_set_value_tests ) );
	ts1->add( BOOST_TEST_CASE( &property_reader_reset_value_tests ) );
	ts1->add( BOOST_TEST_CASE( &property_reader_delete_value_tests ) );
	ts1->add( BOOST_TEST_CASE( &property_reader_rename_parameter_tests ) );
	ts1->add( BOOST_TEST_CASE( &property_reader_check_value_tests ) );

	return ts1;
}
