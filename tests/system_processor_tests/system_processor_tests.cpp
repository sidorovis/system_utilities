#include "test_registrator.h"

#include <system_processor.h>
#include <time_tracker.h>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

using namespace system_utilities::common;

namespace system_utilities
{
	namespace tests_
	{
		namespace common
		{
			void system_processor_constructor_tests()
			{
				using namespace boost::filesystem;
				static const std::string tests_directory = SOURCE_DIR "/tests/data/system_processor/";
				current_path( tests_directory );

				BOOST_CHECK_THROW( system_processor::details::sp_impl( "" ), std::logic_error );
				BOOST_CHECK_NO_THROW( system_processor::details::sp_impl( tests_directory.c_str() ) );
				BOOST_CHECK_THROW( system_processor::details::sp_impl( "", "" ), std::logic_error );
				BOOST_CHECK_THROW( system_processor::details::sp_impl( "", "config_example_001.ini" ), std::logic_error );
				BOOST_CHECK_THROW( system_processor::details::sp_impl( tests_directory, "" ), std::logic_error );
				BOOST_CHECK_NO_THROW( system_processor::details::sp_impl( tests_directory, "config_example_001.ini" ) );
				remove_all( "logs" );
				BOOST_CHECK_THROW( system_processor::details::sp_impl( tests_directory, "config_example_002.ini" ), std::exception );
				BOOST_CHECK_NO_THROW( system_processor::details::sp_impl( tests_directory, "config_example_003.ini" ) );
				remove_all( "c:/system_utilities_logs" );
				{
					system_processor::details::sp_impl processor( tests_directory, "config_example_001.ini" );
					boost::regex log_path_regexp(	"logs/\\d{8}\\_\\d{6}\\_\\d{6}\\/" );
					const std::string logs_path = processor.logs_path_.substr( tests_directory.size() );
					BOOST_CHECK_EQUAL( boost::regex_match( logs_path, log_path_regexp ), true );
				}
				remove_all( "logs" );
			}

			void system_processor_init_tests()
			{
				using namespace boost::filesystem;
				static const std::string tests_directory = SOURCE_DIR "/tests/data/system_processor/";
				current_path( tests_directory );

				int argc = 3;
				char* const argv[] = { 
					SOURCE_DIR "/tests/data/system_processor/test.exe",
					"--config",
					"config_example_004.ini"
				};
				{
					system_processor::sp sp = system_processor::init( argc, argv, "" );
				}
				BOOST_CHECK_EQUAL( exists( "logs_004" ), true );
				remove_all( "logs_004" );
			}
			namespace details
			{
				void system_processor_stop_wait_stop_test_helper()
				{
					boost::this_thread::sleep( boost::posix_time::milliseconds( 400 ) );
					system_processor::stop( "stop by test purposes" );
				}
			}
			void system_processor_stop_wait_stop_tests()
			{
				using namespace boost::filesystem;
				static const std::string tests_directory = SOURCE_DIR "/tests/data/system_processor/";
				current_path( tests_directory );

				int argc = 1;
				char* const argv[] = { SOURCE_DIR "/tests/data/system_processor/test.exe" };
				{
					time_tracker tt;
					system_processor::sp sp = system_processor::init( argc, argv, "config_example_005.ini" );
					boost::thread th( boost::bind( &details::system_processor_stop_wait_stop_test_helper ) );
					system_processor::wait_for_stop();
					BOOST_CHECK_EQUAL( tt.milliseconds() >= 400, true );
				}
				
				remove_all( "logs_005" );
			}
			namespace details
			{
				void system_processor_exit_handler_test_helper()
				{
					boost::this_thread::sleep( boost::posix_time::milliseconds( 100 ) );
					system_processor::details::exit_handler(0);
				}
			}
			void system_processor_exit_handler_tests()
			{
				using namespace boost::filesystem;
				static const std::string tests_directory = SOURCE_DIR "/tests/data/system_processor/";
				current_path( tests_directory );

				int argc = 1;
				char* const argv[] = { SOURCE_DIR "/tests/data/system_processor/test.exe" };
				{
					time_tracker tt;
					system_processor::sp sp = system_processor::init( argc, argv, "config_example_006.ini" );
					boost::thread th( boost::bind( &details::system_processor_exit_handler_test_helper ) );
					BOOST_CHECK_NO_THROW( system_processor::wait_for_stop() );
				}
				remove_all( "logs_006" );
			}
			void system_processor_binary_path_tests()
			{
				using namespace boost::filesystem;
				static const std::string tests_directory = SOURCE_DIR "/tests/data/system_processor/";
				current_path( tests_directory );

				int argc = 1;
				char* const argv[] = { SOURCE_DIR "/tests/data/system_processor/test.exe" };

				BOOST_CHECK_THROW( system_processor::binary_path(), std::exception );
				{
					system_processor::sp sp = system_processor::init( argc, argv, "config_example_007.ini" );
					BOOST_CHECK_NO_THROW( system_processor::binary_path() );
					BOOST_CHECK_EQUAL( system_processor::binary_path(), SOURCE_DIR "/tests/data/system_processor/" );
				}
				remove_all( "logs_007" );
			}
			void system_processor_logs_path_tests()
			{
				using namespace boost::filesystem;
				static const std::string tests_directory = SOURCE_DIR "/tests/data/system_processor/";
				current_path( tests_directory );

				int argc = 1;
				char* const argv[] = { SOURCE_DIR "/tests/data/system_processor/test.exe" };

				BOOST_CHECK_THROW( system_processor::logs_path(), std::exception );
				{
					system_processor::sp sp = system_processor::init( argc, argv, "config_example_007.ini" );
					BOOST_CHECK_NO_THROW( system_processor::logs_path() );
					std::string logs_path = system_processor::logs_path();

					boost::regex log_path_regexp(	"logs_007/\\d{8}\\_\\d{6}\\_\\d{6}\\/" );
					logs_path = logs_path.substr( tests_directory.size() ); 
					BOOST_CHECK_EQUAL( boost::regex_match( logs_path, log_path_regexp ), true );
				}
				remove_all( "logs_007" );
			}
			void system_processor_config_tests()
			{
				using namespace boost::filesystem;
				static const std::string tests_directory = SOURCE_DIR "/tests/data/system_processor/";
				current_path( tests_directory );

				int argc = 1;
				char* const argv[] = { SOURCE_DIR "/tests/data/system_processor/test.exe" };

				BOOST_CHECK_THROW( system_processor::logs_path(), std::exception );
				{
					system_processor::sp sp = system_processor::init( argc, argv, "config_example_008.ini" );
					
					BOOST_CHECK_EQUAL( system_processor::config( "Parameter.config" ), "hello world" );
					BOOST_CHECK_EQUAL( system_processor::config<bool>( "System.stop_by_ctrl_c" ), true );
					BOOST_CHECK_EQUAL( system_processor::config<double>( "Parameter.double" ), 45.6 );

					BOOST_CHECK_NO_THROW( sp->properties_->check_value( "System.log.path" ) );

				}
				remove_all( "logs_008" );
			}
			void system_processor_config_values_tests()
			{
				using namespace boost::filesystem;
				static const std::string tests_directory = SOURCE_DIR "/tests/data/system_processor/";
				current_path( tests_directory );

				int argc = 1;
				char* const argv[] = { SOURCE_DIR "/tests/data/system_processor/test.exe" };

				BOOST_CHECK_THROW( system_processor::logs_path(), std::exception );
				{
					system_processor::sp sp = system_processor::init( argc, argv, "config_example_009.ini" );
					
					BOOST_CHECK_EQUAL( system_processor::config( "Parameter.config" ), "hello world" );
					BOOST_CHECK_EQUAL( system_processor::config_values( "Parameter.values" ).size(), 4 );
					BOOST_CHECK_EQUAL( system_processor::config_values( "Parameter.values" )[0], "hello" );
					BOOST_CHECK_EQUAL( system_processor::config_values( "Parameter.values" )[1], "world" );
					BOOST_CHECK_EQUAL( system_processor::config_values( "Parameter.values" )[2], "some" );
					BOOST_CHECK_EQUAL( system_processor::config_values( "Parameter.values" )[3], "parameters" );

					BOOST_CHECK_NO_THROW( sp->properties_->check_value( "System.log.path" ) );

				}
				remove_all( "logs_009" );
			}

			void system_processor_config_reset_value_tests()
			{
				using namespace boost::filesystem;
				static const std::string tests_directory = SOURCE_DIR "/tests/data/system_processor/";
				current_path( tests_directory );

				int argc = 1;
				char* const argv[] = { SOURCE_DIR "/tests/data/system_processor/test.exe" };

				BOOST_CHECK_THROW( system_processor::logs_path(), std::exception );
				{
					system_processor::sp sp = system_processor::init( argc, argv, "config_example_011.ini" );

					BOOST_CHECK_EQUAL( system_processor::config( "parameter_01" ), "value_01" );
					BOOST_CHECK_EQUAL( system_processor::config( "parameter_02" ), "value_02" );
					BOOST_CHECK_EQUAL( system_processor::config( "parameter_03", "" ), "" );

					BOOST_CHECK_NO_THROW( system_processor::config_reset_value( "parameter_01", "value_01_new" ) );
					BOOST_CHECK_NO_THROW( system_processor::config_reset_value( "parameter_02", 22.33 ) );
					BOOST_CHECK_NO_THROW( system_processor::config_reset_value( "parameter_03", 1235 ) );

					BOOST_CHECK_EQUAL( system_processor::config( "parameter_01" ), "value_01_new" );
					BOOST_CHECK_EQUAL( system_processor::config< double >( "parameter_02", 0.0 ), 22.33 );
					BOOST_CHECK_EQUAL( system_processor::config< size_t >( "parameter_03", 0 ), 1235 );
				}
				remove_all( "logs_011" );
			}
			void system_processor_config_delete_value_tests()
			{
				using namespace boost::filesystem;
				static const std::string tests_directory = SOURCE_DIR "/tests/data/system_processor/";
				current_path( tests_directory );

				int argc = 1;
				char* const argv[] = { SOURCE_DIR "/tests/data/system_processor/test.exe" };

				BOOST_CHECK_THROW( system_processor::logs_path(), std::exception );
				{
					system_processor::sp sp = system_processor::init( argc, argv, "config_example_012.ini" );

					BOOST_CHECK_EQUAL( system_processor::config( "parameter_01" ), "value_01" );
					BOOST_CHECK_EQUAL( system_processor::config( "parameter_02" ), "value_02" );
					BOOST_CHECK_EQUAL( system_processor::config( "parameter_03", "" ), "" );

					BOOST_CHECK_NO_THROW( system_processor::config_delete_value( "parameter_01" ) );
					BOOST_CHECK_NO_THROW( system_processor::config_delete_value( "parameter_03" ) );

					BOOST_CHECK_EQUAL( system_processor::config( "parameter_01" ), "" );
					BOOST_CHECK_EQUAL( system_processor::config( "parameter_02" ), "value_02" );
					BOOST_CHECK_EQUAL( system_processor::config( "parameter_03" ), "" );
				}
				remove_all( "logs_012" );
			}
			void system_processor_config_rename_parameter_tests()
			{
				{
					using namespace boost::filesystem;
					static const std::string tests_directory = SOURCE_DIR "/tests/data/system_processor/";
					current_path( tests_directory );

					int argc = 1;
					char* const argv[] = { SOURCE_DIR "/tests/data/system_processor/test.exe" };

					BOOST_CHECK_THROW( system_processor::logs_path(), std::exception );
					{
						system_processor::sp sp = system_processor::init( argc, argv, "config_example_014.1.ini" );

						BOOST_CHECK_EQUAL( system_processor::config_rename_parameter( "user_nice_name", "global.calculating.thread_count" ), true );
						BOOST_CHECK_EQUAL( system_processor::config_check_value( "user_nice_name" ), false );
						BOOST_CHECK_EQUAL( system_processor::config_check_value( "global.calculating.thread_count" ), true );
						BOOST_CHECK_EQUAL( system_processor::config< size_t >( "global.calculating.thread_count", 0 ), 4 );
					}
					remove_all( "logs_014.1" );
				}
				{
					using namespace boost::filesystem;
					static const std::string tests_directory = SOURCE_DIR "/tests/data/system_processor/";
					current_path( tests_directory );

					int argc = 1;
					char* const argv[] = { SOURCE_DIR "/tests/data/system_processor/test.exe" };

					BOOST_CHECK_THROW( system_processor::logs_path(), std::exception );
					{
						system_processor::sp sp = system_processor::init( argc, argv, "config_example_014.2.ini" );

						BOOST_CHECK_EQUAL( system_processor::config_check_value( "name_old" ), false );
						BOOST_CHECK_EQUAL( system_processor::config_check_value( "name_new" ), false );
						BOOST_CHECK_EQUAL( system_processor::config_rename_parameter( "name_old", "name_new" ), false );
						BOOST_CHECK_EQUAL( system_processor::config_check_value( "name_old" ), false );
						BOOST_CHECK_EQUAL( system_processor::config_check_value( "name_new" ), false );
					}
					remove_all( "logs_014.2" );
				}
				{
					using namespace boost::filesystem;
					static const std::string tests_directory = SOURCE_DIR "/tests/data/system_processor/";
					current_path( tests_directory );

					int argc = 1;
					char* const argv[] = { SOURCE_DIR "/tests/data/system_processor/test.exe" };

					BOOST_CHECK_THROW( system_processor::logs_path(), std::exception );
					{
						system_processor::sp sp = system_processor::init( argc, argv, "config_example_014.3.ini" );

						BOOST_CHECK_EQUAL( system_processor::config_check_value( "name_old" ), false );
						BOOST_CHECK_EQUAL( system_processor::config_check_value( "name_new" ), true );
						BOOST_CHECK_EQUAL( system_processor::config_rename_parameter( "name_old", "name_new" ), false );
						BOOST_CHECK_EQUAL( system_processor::config_check_value( "name_old" ), false );
						BOOST_CHECK_EQUAL( system_processor::config_check_value( "name_new" ), true );
						BOOST_CHECK_EQUAL( system_processor::config( "name_new" ), "xxx" );
					}
					remove_all( "logs_014.3" );
				}
			}
			void system_processor_config_check_value_tests()
			{
				using namespace boost::filesystem;
				static const std::string tests_directory = SOURCE_DIR "/tests/data/system_processor/";
				current_path( tests_directory );

				int argc = 1;
				char* const argv[] = { SOURCE_DIR "/tests/data/system_processor/test.exe" };

				BOOST_CHECK_THROW( system_processor::logs_path(), std::exception );
				{
					system_processor::sp sp = system_processor::init( argc, argv, "config_example_013.ini" );

					BOOST_CHECK_NO_THROW( system_processor::config_check_value( "parameter_01" ) );
					BOOST_CHECK_NO_THROW( system_processor::config_check_value( "no_exist_parameter" ) );

					BOOST_CHECK_EQUAL( system_processor::config_check_value( "parameter_01" ), true );
					BOOST_CHECK_EQUAL( system_processor::config_check_value( "parameter_02" ), true );
					BOOST_CHECK_EQUAL( system_processor::config_check_value( "no_exist_parameter" ), false );

					system_processor::config_reset_value( "no_exist_parameter", "data" );
					system_processor::config_delete_value( "parameter_01" );

					BOOST_CHECK_EQUAL( system_processor::config_check_value( "parameter_01" ), false );
					BOOST_CHECK_EQUAL( system_processor::config_check_value( "parameter_02" ), true );
					BOOST_CHECK_EQUAL( system_processor::config_check_value( "no_exist_parameter" ), true );
				}
				remove_all( "logs_013" );
			}
			//
			void system_processor_create_log_tests()
			{
				using namespace boost::filesystem;
				static const std::string tests_directory = SOURCE_DIR "/tests/data/system_processor/";
				current_path( tests_directory );

				int argc = 1;
				char* const argv[] = { SOURCE_DIR "/tests/data/system_processor/test.exe" };

				BOOST_CHECK_THROW( system_processor::logs_path(), std::exception );
				{
					system_processor::sp sp = system_processor::init( argc, argv, "config_example_010.ini" );
					
					boost::shared_ptr< file_logger<> > log = system_processor::create_log< logger<> >( "example" );
					log->note("YeyeYe");
					
					BOOST_CHECK_EQUAL( exists( system_processor::logs_path() + "example" ), true );
				}
				remove_all( "logs_010" );
			}
		}
	}
}
