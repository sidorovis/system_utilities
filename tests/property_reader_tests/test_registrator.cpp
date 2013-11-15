#include "test_registrator.h"

#include <boost/test/unit_test.hpp>

TEST_INIT
{	
	boost::unit_test_framework::test_suite& master_test_suite = boost::unit_test::framework::master_test_suite();

	using namespace system_utilities::tests_::common;
	master_test_suite.add( BOOST_TEST_CASE( &property_reader_constructor_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &property_reader_parse_istream_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &property_reader_delete_comment_tests ) );
	//
	master_test_suite.add( BOOST_TEST_CASE( &property_reader_test_on_include_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &property_reader_include_line_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &property_reader_string_with_setting_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &property_reader_additional_string_with_setting_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &property_reader_read_sub_property_file_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &property_reader_parse_line_tests ) );
	//
	master_test_suite.add( BOOST_TEST_CASE( &property_reader_comment_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &property_reader_trim_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &property_reader_const_trim_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &property_reader_split_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &property_reader_comment_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &property_reader_small_amount_of_parameters_tests ) );
	//
	master_test_suite.add( BOOST_TEST_CASE( &property_reader_size_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &property_reader_get_set_value_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &property_reader_reset_value_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &property_reader_delete_value_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &property_reader_rename_parameter_tests ) );
	master_test_suite.add( BOOST_TEST_CASE( &property_reader_check_value_tests ) );

	return TEST_RETURN;
}

DEFINE_MAIN_FOR_DYNAMIC
