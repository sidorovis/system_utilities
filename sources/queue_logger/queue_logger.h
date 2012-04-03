#ifndef _SYSTEM_UTILITIES_COMMON_QUEUE_LOGGER_H_
#define _SYSTEM_UTILITIES_COMMON_QUEUE_LOGGER_H_

#include <logger.h>
#include <task_processor.h>

namespace system_utilities
{
	namespace tests_
	{
		namespace common
		{
		};
	}
    namespace common
    {
		template< bool turn_on = true, bool flush_stream = true, bool print_prefix = true >
		class queue_logger;

		namespace details
		{
			template< bool turn_on, bool flush_stream, bool print_prefix >
			class queue_logger_task
			{
				typedef queue_logger< turn_on, flush_stream, print_prefix > logger;
				friend class logger;

				logger& logger_;

				const message_level::value message_level_;
				const std::string message_;
				explicit queue_logger_task( logger& l, const message_level::value message_level, const std::string& message )
					: logger_( l )
					, message_level_( message_level )
					, message_( message )
				{
				}
			public:
				void operator()()
				{
					logger_.real_write( message_level_, message_ );
				}
				~queue_logger_task()
				{
				}
			};
		};
		template< bool turn_on, bool flush_stream, bool print_prefix >
		class queue_logger : public logger< turn_on, flush_stream, print_prefix >
		{
			typedef details::queue_logger_task< turn_on, flush_stream, print_prefix > logger_task;
			friend class logger_task;
		public:
			typedef typename task_processor< logger_task > tasker;
		private:
			mutable boost::mutex protect_write_;
			tasker& task_processor_;
		protected:
			explicit queue_logger( const queue_logger& copy_from )
				: protect_write_()
				, task_processor_( copy_from.task_processor_ )
			{
			}
		public:
			explicit queue_logger( std::ostream& out, tasker& tp )
				: logger< turn_on, flush_stream, print_prefix >( out )
				, task_processor_( tp )
			{
			}
			virtual ~queue_logger()
			{
			}
		private:
			void write( const details::message_level::value value, const std::string& message )
			{
				logger_task* task = new logger_task( *this, value, message );
				task_processor_.add_task( task );
			}
			void real_write( const details::message_level::value value, const std::string& message )
			{
				boost::mutex::scoped_lock lock( protect_write_ );
				logger< turn_on, flush_stream, print_prefix >::write( value, message );
			}
		};

	};
};

#endif // _SYSTEM_UTILITIES_COMMON_QUEUE_LOGGER_H_

