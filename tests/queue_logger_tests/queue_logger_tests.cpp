#include "test_registrator.h"

#include <queue_logger.h>
#include <time_tracker.h>

#include <boost/algorithm/string.hpp>

using namespace system_utilities::common;

namespace system_utilities
{
	namespace tests_
	{
		namespace common
		{
			namespace details
			{
				typedef queue_logger< true, false, false > q_logger;

				void logger_writer( q_logger* logger, const size_t size )
				{
					for ( size_t i = 0 ; i < size ; ++i )
						logger->note() << "qwerasdfzxcvkjhgiyt7654";
				}
				void queue_logger_write_test_helper( const size_t amount_of_messages_per_thread, const size_t time )
				{
					static const size_t processors_thread_size = 4;
					static const size_t thread_per_log = 4;

					std::stringstream log_first, log_second;
					
					details::q_logger::tasker task_processor( processors_thread_size );

					typedef boost::shared_ptr< details::q_logger > logger_ptr;
					std::vector< logger_ptr > loggers;
					boost::thread_group tg;

					{
						details::q_logger* logger = new details::q_logger( log_first, task_processor );
						loggers.push_back( logger_ptr( logger ) );
					}
					{
						details::q_logger* logger = new details::q_logger( log_second, task_processor );
						loggers.push_back( logger_ptr( logger ) );
					}

					time_tracker tt;
					for ( size_t i = 0 ; i < thread_per_log ; ++i )
					{
						tg.create_thread( boost::bind( &details::logger_writer, loggers[0].get(), amount_of_messages_per_thread ) );
						tg.create_thread( boost::bind( &details::logger_writer, loggers[1].get(), amount_of_messages_per_thread ) );
					}
					tg.join_all();
					task_processor.wait();
					std::cout << tt.milliseconds() << std::endl;
					BOOST_CHECK_EQUAL( tt.milliseconds() < time , true );
					const std::string first_result = log_first.str();
					const std::string second_result = log_second.str();

					typedef std::vector< std::string > strings;
					{
						strings lines;
						boost::algorithm::split( lines, first_result, boost::algorithm::is_any_of( "\n" ) );
						BOOST_CHECK_EQUAL( lines.size(), amount_of_messages_per_thread * thread_per_log + 1 );
						for (size_t i = 0 ; i < amount_of_messages_per_thread * thread_per_log ; ++i )
							BOOST_CHECK_EQUAL( lines[ i ].c_str(), "qwerasdfzxcvkjhgiyt7654" );
					}
					{
						strings lines;
						boost::algorithm::split( lines, second_result, boost::algorithm::is_any_of( "\n" ) );
						BOOST_CHECK_EQUAL( lines.size(), amount_of_messages_per_thread * thread_per_log + 1 );
						for (size_t i = 0 ; i < amount_of_messages_per_thread * thread_per_log ; ++i )
							BOOST_CHECK_EQUAL( lines[ i ].c_str(), "qwerasdfzxcvkjhgiyt7654" );
					}

				}
			};
			void queue_logger_constructor_tests()
			{
				std::stringstream str;
				queue_logger<>::tasker task_processor( 4 );
				BOOST_CHECK_NO_THROW( queue_logger<> first_logger( str, task_processor ) );
				BOOST_CHECK_NO_THROW( queue_logger<> second_logger( str, task_processor ) );
			}
			void queue_logger_write_tests()
			{
				details::queue_logger_write_test_helper( 2500, 200 );
			}
			void queue_logger_performance_write_tests()
			{
				details::queue_logger_write_test_helper( 25000, 280 );
			}
		}
	}
}
