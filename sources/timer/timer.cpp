#include "timer.h"

namespace system_utilities
{
	namespace common
	{

		timer_prototype::timer_prototype( const size_t sleep_microseconds, timer_prototype::method m )
			: sleep_microseconds_( sleep_microseconds )
			, started_ ( false )
			, should_stop_ ( false )
			, method_( m )
		{
		}
		timer_prototype::~timer_prototype()
		{
			end();
			thread_for_run_.join();
		}
		void timer_prototype::begin()
		{
			started_ = true;
			should_stop_ = false;
			thread_for_run_ = boost::thread( boost::bind( &timer_prototype::processing_, this ) );
		}
		void timer_prototype::end()
		{
			if (!started_ || should_stop_)
				return;
			{
				should_stop_ = true;
				boost::recursive_mutex::scoped_lock stopper( should_stop_protector_ );
				for_stop_.notify_one();
			}
			started_ = false;
		}

		boost::xtime timer_prototype::sleep_interval_()
		{
			boost::xtime time;
			boost::xtime_get( &time, boost::TIME_UTC );
			if (sleep_microseconds_ >= 1000)
			{
				size_t sec_inc = sleep_microseconds_ / 1000;
				time.sec += sec_inc;
				time.nsec += (boost::xtime::xtime_nsec_t)((sleep_microseconds_ - sec_inc * 1000) * 1000000);
			}
			else
				time.nsec += (boost::xtime::xtime_nsec_t)((sleep_microseconds_ * 1000000));
			return time;
		}
		void timer_prototype::processing_()
		{
			try
			{
				for(;;)
				{
					if (should_stop_)
						break;
					boost::recursive_mutex::scoped_lock run_func( should_stop_protector_ );
					(*method_)();
					if (should_stop_)
						break;
					for_stop_.timed_wait( run_func, sleep_interval_() );
					if (should_stop_)
						break;
				}
			}
			catch (...)
			{
			}
		}

		timer create_timer( const size_t sleep_microseconds, void (*method)() )
		{
			return timer( new timer_prototype( sleep_microseconds, method ) );
		}
		//
		//
		namespace testing_
		{
			void testing_method(){}

			class testing_class
			{
			public:
				explicit testing_class()
				{
					timer t = create_timer( 600, *this, &testing_class::class_method );
				}
				void class_method()
				{
				}
			};

			void class_instantiation_check()
			{
				timer t = create_timer( 600, &testing_method );
				testing_class tc;
			}
		}
	}
}

