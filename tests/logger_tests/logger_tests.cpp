#include "test_registrator.h"

#include <logger.h>

#include <boost/regex.hpp>

using namespace system_utilities::common;

namespace system_utilities
{
	namespace tests_
	{
		namespace common
		{
			void logger_constructor_tests()
			{
				std::stringstream str;
				logger< false > l_false( str );
				logger< true > l_true( str );
				logger< true, false > l_true_false( str );
				logger< true, false, false > l_true_false_false( str );
				logger< true, true, false > l_true_true_false( str );
				logger< true, true, true, std::stringstream > l_stringstream( str );
			}
			void logger_write_tests()
			{
				{
					std::stringstream stream;
					logger< false > no_logger( stream );
					no_logger.write( "LEVEL", "message" );
					const std::string stream_content = stream.str();
					BOOST_CHECK_EQUAL( "", stream_content );
				}
				{
					std::stringstream stream;
					logger<> log( stream );
					log.write( ":LEVEL  ]: ", "message" );
					const std::string stream_content = stream.str();
					boost::regex message_regex( "\\[\\d{4}\\-\\w{3}\\-\\d{2} \\d{2}\\:\\d{2}\\:\\d{2}\\.\\d{6}\\:[A-Z ]{7}\\]\\: message\n" );
					BOOST_CHECK_EQUAL( boost::regex_match( stream_content, message_regex ), true );
				}
				{
					std::stringstream stream;
					logger< true, false > log( stream );
					log.write( ":LEVEL  ]: ", "message" );
					const std::string stream_content = stream.str();
					boost::regex message_regex( "\\[\\d{4}\\-\\w{3}\\-\\d{2} \\d{2}\\:\\d{2}\\:\\d{2}\\.\\d{6}\\:[A-Z ]{7}\\]\\: message\n" );
					BOOST_CHECK_EQUAL( boost::regex_match( stream_content, message_regex ), true );
				}
				{
					std::stringstream stream;
					logger< true, false, false > log( stream );
					log.write( ":LEVEL  ]: ", "message" );
					const std::string stream_content = stream.str();
					boost::regex message_regex( "message\n" );
					BOOST_CHECK_EQUAL( boost::regex_match( stream_content, message_regex ), true );
				}
			}
			void logger_flush_tests()
			{
				std::stringstream it_is_just_a_test_on_existing_flush_method;
				logger<> l( it_is_just_a_test_on_existing_flush_method );
				l.flush();
			}
			void logger_note_tests()
			{
				std::stringstream stream;
				logger<> l( stream );
				l.note( "note message" );
				const std::string stream_content = stream.str();
				boost::regex message_regex( "\\[\\d{4}\\-\\w{3}\\-\\d{2} \\d{2}\\:\\d{2}\\:\\d{2}\\.\\d{6}\\:NOTE   \\]\\: note message\n" );
				BOOST_CHECK_EQUAL( boost::regex_match( stream_content, message_regex ), true );
			}
			void logger_warn_tests()
			{
				std::stringstream stream;
				logger<> l( stream );
				l.warn( "not important message" );
				const std::string stream_content = stream.str();
				boost::regex message_regex( "\\[\\d{4}\\-\\w{3}\\-\\d{2} \\d{2}\\:\\d{2}\\:\\d{2}\\.\\d{6}\\:WARNING\\]\\: not important message\n" );
				BOOST_CHECK_EQUAL( boost::regex_match( stream_content, message_regex ), true );
			}
			void logger_error_tests()
			{
				std::stringstream stream;
				logger<> l( stream );
				l.error( "error message" );
				const std::string stream_content = stream.str();
				boost::regex message_regex( "\\[\\d{4}\\-\\w{3}\\-\\d{2} \\d{2}\\:\\d{2}\\:\\d{2}\\.\\d{6}\\:ERROR  \\]\\: error message\n" );
				BOOST_CHECK_EQUAL( boost::regex_match( stream_content, message_regex ), true );
			}
			void logger_debug_tests()
			{
				std::stringstream stream;
				logger<> l( stream );
				l.debug( "debug message" );
				const std::string stream_content = stream.str();
				boost::regex message_regex( "\\[\\d{4}\\-\\w{3}\\-\\d{2} \\d{2}\\:\\d{2}\\:\\d{2}\\.\\d{6}\\:DEBUG  \\]\\: debug message\n" );
				BOOST_CHECK_EQUAL( boost::regex_match( stream_content, message_regex ), true );
			}
			void logger_fatal_tests()
			{
				std::stringstream stream;
				logger<> l( stream );
				l.fatal( "fatal message" );
				const std::string stream_content = stream.str();
				boost::regex message_regex( "\\[\\d{4}\\-\\w{3}\\-\\d{2} \\d{2}\\:\\d{2}\\:\\d{2}\\.\\d{6}\\:FATAL  \\]\\: fatal message\n" );
				BOOST_CHECK_EQUAL( boost::regex_match( stream_content, message_regex ), true );
			}
		}
	}
}

/*
#include "logger_test.h"

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

#include <iostream>

namespace 
{
    std::string delete_pattern_from_string( std::string& content_ )
    {
        std::string content = content_;
        const size_t pattern_end = content.find( "]: " );
        content.erase( 0, pattern_end + 3);
        content.erase( content.size() - 1, 1 );
        return content;
    }
};

simple_logger_test::simple_logger_test()
{
    log_file_example_lines.push_back( "this is example message of error" );
    log_file_example_lines.push_back( "this is example message of note" );
    log_file_example_lines.push_back( "this is example message of debug" );
    log_file_example_lines.push_back( "this is example message of warn" );
    log_file_example_lines.push_back( "this is example message of fatal" );
    
}
simple_logger_test::~simple_logger_test()
{
}
void simple_logger_test::create_test()
{
    std::stringstream stream;
    {
        simple_logger logger( &stream );
    }
    BOOST_CHECK_EQUAL( stream.str(), "" );
    
}
void simple_logger_test::logger_all_levels_test()
{
    fatal_level_test();
    error_level_test();
    warning_level_test();
    note_level_test();
    debug_level_test();
}
void simple_logger_test::fatal_level_test()
{
    stream_content_ = "test fatal message";
    logger_level_test( &simple_logger::fatal );
}
void simple_logger_test::error_level_test()
{
    stream_content_ = "test error message";
    logger_level_test( &simple_logger::error );
}
void simple_logger_test::warning_level_test()
{
    stream_content_ = "warning test message";
    logger_level_test( &simple_logger::warn );
}
void simple_logger_test::note_level_test()
{
    stream_content_ = "test for note message";
    logger_level_test( &simple_logger::note );    
}
void simple_logger_test::debug_level_test()
{
    stream_content_ = "debug message test";
    logger_level_test( &simple_logger::debug );
}

void simple_logger_test::logger_level_test(simple_logger_function member_function)
{
    std::stringstream stream;
    simple_logger logger( &stream );
    (logger.*member_function)( stream_content_ );
    test_right_content(stream);
}
void simple_logger_test::test_right_content(std::stringstream& stream)
{
    std::string logger_stream_content = stream.str();
    BOOST_CHECK_EQUAL( delete_pattern_from_string( logger_stream_content ) , stream_content_ );
}
void simple_logger_test::create_log_file_example()
{
    std::stringstream stream;
    generate_stream( stream );
    std::string content = stream.str();
    BOOST_CHECK_EQUAL( content[ content.size() - 1 ], '\n' );
    stream_lines lines;
    boost::split( lines, content, boost::is_any_of("\n") );
    check_line_count( lines );
    check_first_and_last_line( lines );
    check_message_lines( lines );
}
void simple_logger_test::generate_stream(std::stringstream& stream)
{
    simple_logger logger( &stream );
    logger.error( log_file_example_lines[ 0 ] );
    logger.note( log_file_example_lines[ 1 ] );
    logger.debug( log_file_example_lines[ 2 ] );
    logger.warn( log_file_example_lines[ 3 ] );
    logger.fatal( log_file_example_lines[ 4 ] );
}
void simple_logger_test::check_line_count(stream_lines& lines)
{
    BOOST_CHECK_EQUAL( lines.size(), log_file_example_lines.size() + 1 );
}
void simple_logger_test::check_first_and_last_line(stream_lines& lines)
{
    BOOST_CHECK_EQUAL( lines[ lines.size() - 1 ], "" );    
}
void simple_logger_test::check_message_lines( stream_lines& lines )
{
    for( size_t i = 0 ; i < log_file_example_lines.size() ; i++ )
    {
		boost::regex message_regex( "\\[(\\d){4}\\-(\\w){3}\\-(\\d){2} (\\d){2}\\:(\\d){2}\\:(\\d){2}\\.(\\d){6}\\:([A-Z ]){7}\\]\\: " + log_file_example_lines[i] );
        BOOST_CHECK_EQUAL( boost::regex_match( lines[i], message_regex ), true );
    }
}

void simple_logger_test::test_write_function()
{
    class simple_logger_write_tester : public simple_logger
    {

    public:
        simple_logger_write_tester( std::stringstream& stream )
        : simple_logger( &stream )
        {
            write( "Level", "message" );
            std::string content = stream.str();
            stream_lines lines;
            boost::split( lines, content, boost::is_any_of("\n") );
            boost::regex message_regex( "\\[(\\d){4}\\-(\\w){3}\\-(\\d){2} (\\d){2}\\:(\\d){2}\\:(\\d){2}\\.(\\d){6}Levelmessage" );
            const bool line_should_match_regexp = boost::regex_match( lines[0], message_regex );
            BOOST_CHECK_EQUAL( line_should_match_regexp, true );
        }
    };
    std::stringstream stream;
    simple_logger_write_tester tester( stream );
}


using boost::unit_test_framework::test_case;

simple_logger_test_suite::simple_logger_test_suite()
: test_suite( "Simple Logger Test Suite" )
{
    boost::shared_ptr< simple_logger_test > instance( new simple_logger_test );
    add( BOOST_CLASS_TEST_CASE( &simple_logger_test::create_test, instance ) );
    add( BOOST_CLASS_TEST_CASE( &simple_logger_test::logger_all_levels_test, instance ) );
    add( BOOST_CLASS_TEST_CASE( &simple_logger_test::create_log_file_example, instance ) );
    add( BOOST_CLASS_TEST_CASE( &simple_logger_test::test_write_function, instance ) );
}

using boost::unit_test_framework::test_suite;

test_suite* init_unit_test_suite(int, char**const)
{
    test_suite* top_test_suite = BOOST_TEST_SUITE("Simple Logger Testing");
    top_test_suite->add( new simple_logger_test_suite() );
    return top_test_suite;
}

*/