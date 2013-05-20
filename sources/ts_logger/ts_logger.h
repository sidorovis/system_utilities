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

		// ts_logger: thread safe logger
		// see logger class to template parameters reference

		template< typename logger_type = logger< true, true, true > >
		class ts_logger : public logger_type
		{
			friend void tests_::common::ts_logger_write_tests();

			mutable boost::mutex protect_write_;

			explicit ts_logger( const ts_logger& copy_from );
		public:
			template< typename P1 >
			explicit ts_logger( P1& p1 )
				: logger_type( p1 )
			{
			}
			template< typename P1, typename P2 >
			explicit ts_logger( P1& p1, P2& p2 )
				: logger_type( p1, p2 )
			{
			}
			virtual ~ts_logger()
			{
			}
		protected:
			void write( const details::message_level::value value, const std::string& message )
			{
				boost::mutex::scoped_lock lock( protect_write_ );
				logger_type::write( value, message );
			}
		};

	};
};

#endif // _SYSTEM_UTILITIES_COMMON_TS_LOGGER_H_

