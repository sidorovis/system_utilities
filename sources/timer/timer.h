#ifndef _SYSTEM_UTILITIES_COMMON_TIMER_H_
#define _SYSTEM_UTILITIES_COMMON_TIMER_H_

#include <task_processor.h>

#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread.hpp>

namespace system_utilities
{
	namespace common
	{

		template< class task = details::timer_task, template< class, class, class > class task_handler = task_processor< task > >
		class timer;

		namespace details
		{

			template< typename method >
			class timer_task
			{
				typedef timer< method > friend_timer;
				friend class friend_timer;
				
				method& method_;

				explicit timer_task( method& m )
					: method_( m )
				{
				}

			public:
				void operator()()
				{
					method_();
				}
				~timer_task()
				{
				}
			};
		};
		template< typename method >
		class timer : protected virtual boost::noncopyable
		{
			task_handler& task_processor_;
			const size_t sleep_interval_;
			bool started_;
		public:
			explicit timer( task_handler& th, const size_t sleep_microseconds, method& m )
				: task_processor_( th )
				, sleep_interval_( sleep_microseconds )
			{
				started_ = true;
				th.add_task( new details::timer_task< method >( m ) );
			}
		private:
			void stop()
			{
				started_ = false;
			}
		public:
			~timer()
			{
			}
		};
	}
}

//
//namespace re_utils
//{
//    namespace timers
//    {
//        class simple_timer
//        {
//            friend class timer_with_logger;
//            
//            boost::thread thread_for_run_;
//            boost::recursive_mutex should_stop_protector_;
//            boost::condition for_stop_;
//            volatile const size_t sleep_microseconds_;
//            volatile bool started_;
//            volatile bool should_stop_;
//            
//            boost::xtime sleep_interval();
//        public:
//            explicit simple_timer( size_t sleep_microseconds );
//            ~simple_timer();
//            void begin();
//        private:
//            void runner();
//		public:
//            void end();
//        protected:
//			virtual void before_run();
//            virtual void run() = 0;
//        };
//    };
//};


#endif // _SYSTEM_UTILITIES_COMMON_TIMER_H_

