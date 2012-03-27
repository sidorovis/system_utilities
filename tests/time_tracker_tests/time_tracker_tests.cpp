#include "test_registrator.h"
#include <time_tracker.h>

#include <boost/thread.hpp>

using namespace system_utilities::common;

namespace system_utilities
{
	namespace tests_
	{
		namespace common
		{
			void time_tracker_constructor_tests()
			{
				BOOST_CHECK_NO_THROW( time_tracker tt );
			}
			void time_tracker_milliseconds_tests()
			{
				{
					time_tracker tt;
					BOOST_CHECK_EQUAL( tt.milliseconds(), static_cast< size_t >( 0 ) );
				}
				{
					static const size_t milliseconds = 1;
					time_tracker tt;
					size_t old_time = tt.milliseconds();
					for ( size_t i = 0 ; i < 5 ; ++i )
					{
						boost::this_thread::sleep( boost::posix_time::milliseconds( milliseconds ) );
						BOOST_CHECK_EQUAL( tt.milliseconds() > old_time, true );
						old_time = tt.milliseconds();
					}
				}
			}
			void time_tracker_seconds_tests()
			{
				{
					time_tracker tt;
					BOOST_CHECK_EQUAL( tt.seconds(), static_cast< size_t >( 0 ) );
				}
				{
					time_tracker tt;
					for ( size_t i = 0 ; i < 3 ; ++i )
					{
						boost::this_thread::sleep( boost::posix_time::milliseconds( 1001 ) );
						BOOST_CHECK_EQUAL( tt.seconds() >= (i + 1), true );
						BOOST_CHECK_EQUAL( tt.seconds() < (i + 1) + 2, true );
					}
				}
			}
		}
	}
}
