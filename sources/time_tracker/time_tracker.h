#ifndef _SYSTEM_UTILITIES_COMMON_TIME_TRACKER_H_
#define _SYSTEM_UTILITIES_COMMON_TIME_TRACKER_H_

#include <boost/noncopyable.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace system_utilities
{
	namespace common
	{
		class time_tracker : protected virtual boost::noncopyable
		{
			boost::posix_time::ptime start_;
		public:
			explicit time_tracker()
				: start_( boost::posix_time::microsec_clock::universal_time() )
			{
			}
			const size_t milliseconds() const
			{
				const size_t result = (boost::posix_time::microsec_clock::universal_time() - start_).total_milliseconds();
				return result;
			}
			const size_t seconds() const
			{
				return (boost::posix_time::microsec_clock::universal_time() - start_).total_seconds();
			}
			~time_tracker()
			{
			}
		};
	}
}

#endif // _SYSTEM_UTILITIES_COMMON_TIME_TRACKER_H_
