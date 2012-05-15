#ifndef _SYSTEM_UTILITIES_COMMON_TS_LOGGER_H_
#define _SYSTEM_UTILITIES_COMMON_TS_LOGGER_H_

#include <logger.h>

#include <boost/thread.hpp>

namespace system_utilities
{
 	namespace tests_
	{
		namespace common
		{
			void ts_logger_write_tests();
		};
	}
   namespace common
    {

		template< bool turn_on = true, bool flush_stream = true, bool print_prefix = true >
		class ts_logger : public logger< turn_on, flush_stream, print_prefix >
		{
			friend void tests_::common::ts_logger_write_tests();


			mutable boost::mutex protect_write_;

			explicit ts_logger( const ts_logger& copy_from );
		public:
			explicit ts_logger( std::ostream& out )
				: logger< turn_on, flush_stream, print_prefix >( out )
			{
			}
			virtual ~ts_logger()
			{
			}
		protected:
			void write( const details::message_level::value value, const std::string& message )
			{
				boost::mutex::scoped_lock lock( protect_write_ );
				logger< turn_on, flush_stream, print_prefix >::write( value, message );
			}
		};

	};
};

#endif // _SYSTEM_UTILITIES_COMMON_TS_LOGGER_H_

