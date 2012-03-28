#include "test_registrator.h"

#include <file_logger.h>
#include <time_tracker.h>
#include <queue_logger.h>

#include <boost/regex.hpp>

using namespace system_utilities::common;

namespace system_utilities
{
	namespace tests_
	{
		namespace common
		{
			void file_logger_constructor_tests()
			{
				static const std::string tests_directory = SOURCE_DIR "/tests/data/file_logger/";
				using namespace boost::filesystem;
				current_path( tests_directory );
				{
					file_logger<> simple_logger( "log_test_1.out" );
					BOOST_CHECK_EQUAL( exists( "log_test_1.out" ), true );
				}
				remove( "log_test_1.out" );
				{
					queue_logger<>::tasker tasker( 1 );
					file_logger< queue_logger<> > simple_logger( "log_test_2.out", tasker );
					BOOST_CHECK_EQUAL( exists( "log_test_2.out" ), true );
				}
				remove( "log_test_2.out" );
			}

			void file_logger_note_tests()
			{
				using namespace boost::filesystem;
				{
					file_logger<> l( "log_test_note.out" );
					l.note( "note test" );
					l.note() << "test 2";
				}
				BOOST_CHECK_EQUAL( exists( "log_test_note.out" ), true );
				remove( "log_test_note.out" );
			}
			void file_logger_warn_tests()
			{
				using namespace boost::filesystem;
				{
					file_logger<> l( "log_test_warn.out" );
					l.warn( "note test" );
					l.warn() << "test 2";
				}
				BOOST_CHECK_EQUAL( exists( "log_test_warn.out" ), true );
				remove( "log_test_warn.out" );
			}
			void file_logger_error_tests()
			{
				using namespace boost::filesystem;
				{
					file_logger<> l( "log_test_error.out" );
					l.error( "error test" );
					l.error() << "test 2";
				}
				BOOST_CHECK_EQUAL( exists( "log_test_error.out" ), true );
				remove( "log_test_error.out" );
			}
			void file_logger_debug_tests()
			{
				using namespace boost::filesystem;
				{
					file_logger<> l( "log_test_debug.out" );
					l.debug( "debug test" );
					l.debug() << "test 2";
				}
				BOOST_CHECK_EQUAL( exists( "log_test_debug.out" ), true );
				remove( "log_test_debug.out" );
			}
			void file_logger_fatal_tests()
			{
				using namespace boost::filesystem;
				{
					file_logger<> l( "log_test_fatal.out" );
					l.fatal( "fatal test" );
					l.fatal() << "test 2";
				}
				BOOST_CHECK_EQUAL( exists( "log_test_fatal.out" ), true );
				remove( "log_test_fatal.out" );
			}
		
			namespace details
			{
				template< class file_logger >
				void file_logger_performance_logger_helper_thread( file_logger* logger, size_t test_size )
				{
					for (size_t i = 0 ; i < test_size ; ++i )
					{
						logger->note( "1234567890123456789012345678901" );
					}
				}
			}
			
			void file_logger_performance_tests()
			{
				static const size_t log_threads_size = 4;
				static const size_t writers = 7;
				static const size_t tests_size = 20000;
				using namespace boost::filesystem;

				typedef queue_logger< true, false, true > queue_logger_type;
				typedef file_logger< queue_logger_type > file_logger_type;
				{
					queue_logger_type::tasker tasker( log_threads_size );
					file_logger_type logger( "log_test_3.out", tasker );
					boost::thread_group tg;
					time_tracker tt;
					for( size_t i = 0 ; i < writers ; ++i )
						tg.create_thread( boost::bind( &details::file_logger_performance_logger_helper_thread< file_logger_type > , &logger, tests_size ) );
					std::cout << tt.milliseconds() << std::endl;
					tg.join_all();
					std::cout << tt.milliseconds() << std::endl;
					tasker.wait();
					BOOST_CHECK_EQUAL( tt.milliseconds() < 1200, true );
					std::cout << tt.milliseconds() << std::endl;					
				}
				BOOST_CHECK_EQUAL( exists( "log_test_3.out" ), true );
				{
					char buffer[256];
					std::ifstream str( "log_test_3.out", std::ios::in );
					size_t line_count = 0;
					boost::regex string_regexp( "\\[\\d{4}\\-\\w{3}\\-\\d{2} \\d{2}\\:\\d{2}\\:\\d{2}\\.\\d{6}\\:[A-Z ]{7}\\]\\: 1234567890123456789012345678901" );
					while ( !str.eof() )
					{
						str.getline( buffer, 256 );
						const std::string line( buffer );
						if ( line.empty() )
							continue;
						line_count++;
						BOOST_CHECK_EQUAL( boost::regex_match( line, string_regexp ), true );
					}
					BOOST_CHECK_EQUAL( line_count, tests_size * writers );
				}
				remove( "log_test_3.out" );
			}
		}
	}
}
