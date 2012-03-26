#include "test_registrator.h"

#include <boost/test/unit_test.hpp>
using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* ts1 = BOOST_TEST_SUITE( "logger tests" );
	boost::unit_test::unit_test_log.set_threshold_level( boost::unit_test::log_messages );

	using namespace system_utilities::tests_::common;
	ts1->add( BOOST_TEST_CASE( &logger_constructor_tests ) );
	ts1->add( BOOST_TEST_CASE( &logger_write_tests ) );
	ts1->add( BOOST_TEST_CASE( &logger_flush_tests ) );
	//
	ts1->add( BOOST_TEST_CASE( &logger_note_tests ) );
	ts1->add( BOOST_TEST_CASE( &logger_warn_tests ) );
	ts1->add( BOOST_TEST_CASE( &logger_error_tests ) );
	ts1->add( BOOST_TEST_CASE( &logger_debug_tests ) );
	ts1->add( BOOST_TEST_CASE( &logger_fatal_tests ) );
	//ts1->add( BOOST_TEST_CASE( &property_reader_parse_line_tests ) );
	/////
	//ts1->add( BOOST_TEST_CASE( &property_reader_comment_tests ) );
	//ts1->add( BOOST_TEST_CASE( &property_reader_trim_tests ) );
	//ts1->add( BOOST_TEST_CASE( &property_reader_const_trim_tests ) );
	//ts1->add( BOOST_TEST_CASE( &property_reader_split_tests ) );
	//ts1->add( BOOST_TEST_CASE( &property_reader_comment_tests ) );
	//ts1->add( BOOST_TEST_CASE( &property_reader_small_amount_of_parameters_tests ) );
	/////
	//ts1->add( BOOST_TEST_CASE( &property_reader_size_tests ) );
	//ts1->add( BOOST_TEST_CASE( &property_reader_get_set_value_tests ) );
	//ts1->add( BOOST_TEST_CASE( &property_reader_check_value_tests ) );

	return ts1;
}
