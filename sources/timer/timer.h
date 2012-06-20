#ifndef _SYSTEM_UTILITIES_COMMON_TIMER_H_
#define _SYSTEM_UTILITIES_COMMON_TIMER_H_

#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread.hpp>

namespace system_utilities
{
	namespace common
	{

		class abstract_timer
		{
		public:
			virtual void begin() = 0;
			virtual void end() = 0;
		};
		typedef boost::shared_ptr< abstract_timer > timer;

		template< typename method >
		class timer_prototype : protected virtual boost::noncopyable, public abstract_timer
		{
            boost::thread thread_for_run_;
            boost::recursive_mutex should_stop_protector_;
            boost::condition for_stop_;

            volatile const size_t sleep_microseconds_;
            volatile bool started_;
            volatile bool should_stop_;

			method& method_;
		public:
			explicit timer_prototype( const size_t sleep_microseconds, method& m )
				: sleep_microseconds_( sleep_microseconds )
				, started_ ( false )
				, should_stop_ ( false )
				, method_( m )
			{
			}
			~timer_prototype()
			{
				end();
				thread_for_run_.join();
			}
			//
			void begin()
			{
				started_ = true;
				should_stop_ = false;
				thread_for_run_ = boost::thread( boost::bind( &timer_prototype::processing_, this ) );
			}
			void end()
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
		private:
			boost::xtime sleep_interval_()
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

			void processing_()
			{
				try
				{
					for(;;)
					{
						if (should_stop_)
							break;
						boost::recursive_mutex::scoped_lock run_func( should_stop_protector_ );
						
						method_();

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
		};

		
		template< typename object >
		class timer_prototype_with_object : protected virtual boost::noncopyable, public abstract_timer
		{
            boost::thread thread_for_run_;
            boost::recursive_mutex should_stop_protector_;
            boost::condition for_stop_;

            volatile const size_t sleep_microseconds_;
            volatile bool started_;
            volatile bool should_stop_;

			object& object_;
			typedef void (object::*object_method)();
			object_method method_;
		public:
			explicit timer_prototype_with_object( const size_t sleep_microseconds, object& obj, object_method m )
				: sleep_microseconds_( sleep_microseconds )
				, started_ ( false )
				, should_stop_ ( false )
				, object_( obj )
				, method_( m )
			{
			}
			~timer_prototype_with_object()
			{
				end();
				thread_for_run_.join();
			}
			//
			void begin()
			{
				started_ = true;
				should_stop_ = false;
				thread_for_run_ = boost::thread( boost::bind( &timer_prototype_with_object::processing_, this ) );
			}
			void end()
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
		private:
			boost::xtime sleep_interval_()
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

			void processing_();
		};

		template< typename object > 
		void timer_prototype_with_object< object >::processing_()
		{
			try
			{
				for(;;)
				{
					if (should_stop_)
						break;
					boost::recursive_mutex::scoped_lock run_func( should_stop_protector_ );
					
					(object_.*method_)();

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
		
		template< typename method >
		timer create_timer( const size_t sleep_microseconds, method m )
		{
			return timer( new timer_prototype< method >( sleep_microseconds, m ) );
		}
		template< typename object >
		timer create_timer( const size_t sleep_microseconds, object& obj, void (object::*object_method)() )
		{
			return timer( new timer_prototype_with_object< object >( sleep_microseconds, obj, object_method ) );
		}
	}
}



#endif // _SYSTEM_UTILITIES_COMMON_TIMER_H_

