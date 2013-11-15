#ifndef _SYSTEM_UTILITIES_COMMON_TIME_TRACKER_H_
#define _SYSTEM_UTILITIES_COMMON_TIME_TRACKER_H_

#include <boost/noncopyable.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/mutex.hpp>

namespace system_utilities
{
	namespace common
	{
		// time_tracker class count time until it was created or reseted.
		// could be used for performance tests, for processing time calculating
		// not a virtual destructor class

		class time_tracker
		{
			mutable boost::mutex protect_start_;
			boost::posix_time::ptime start_;
		public:
			explicit time_tracker();
			time_tracker( const time_tracker& other );
			~time_tracker();
			//
			void reset();
			size_t milliseconds() const;
			size_t seconds() const;
		};
	}
}

#endif // _SYSTEM_UTILITIES_COMMON_TIME_TRACKER_H_
