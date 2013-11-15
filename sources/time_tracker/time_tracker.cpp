#include "time_tracker.h"

namespace system_utilities
{
	namespace common
	{
		time_tracker::time_tracker()
			: start_( boost::posix_time::microsec_clock::universal_time() )
		{
		}
		time_tracker::time_tracker(const time_tracker& other)
			: start_( other.start_ )
		{
		}
		time_tracker::~time_tracker()
		{
		}
		//
		void time_tracker::reset()
		{
			boost::mutex::scoped_lock lock( protect_start_ );
			start_ = boost::posix_time::microsec_clock::universal_time();
		}
		size_t time_tracker::milliseconds() const
		{
			boost::mutex::scoped_lock lock( protect_start_ );
			const size_t result = static_cast< size_t >( (boost::posix_time::microsec_clock::universal_time() - start_).total_milliseconds() );
			return result;
		}
		size_t time_tracker::seconds() const
		{
			boost::mutex::scoped_lock lock( protect_start_ );
			return (boost::posix_time::microsec_clock::universal_time() - start_).total_seconds();
		}
	}
}