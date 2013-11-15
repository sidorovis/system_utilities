#ifndef _SYSTEM_UTILITIES_COMMON_TASK_PROCESSOR_H_
#define _SYSTEM_UTILITIES_COMMON_TASK_PROCESSOR_H_

#include <ts_queue.h>

namespace system_utilities
{
	namespace common
	{

		// task processor class was created in "fast-to-release" implementation.
		// please be carefull with next: task_processor does not have virtual destructor - please be sure that you don't delete child of task_processor class by pointer.
		// please create you task with next assumption: operator() of task class should not produce any exception. Exception that going out of task will terminate the application.

		// task class example: 
		// class task_example
		// {
		//		public:
		//		void operator()()
		//		{
		//			// task actions;
		//		}
		// };
		// you can find other method usage examples at task_processor.cpp

		template< 
			class task, 
			class task_queue = ts_queue< task >, 
			class allocator = std::allocator< task > >
		class task_processor : protected virtual boost::noncopyable
		{
			allocator allocator_;
			boost::thread_group threads_;
			task_queue task_queue_;
			bool stopping_;
			bool process_on_stop_;

			boost::condition wait_condition_;
			mutable boost::mutex wait_;
			volatile size_t working_threads_;

			explicit task_processor();
			explicit task_processor( const task_processor& );
			task_processor& operator=( const task_processor& );
		public:
			// constructor
			// process_on_stop bool parameter: wait until all tasks will be processed by processing threads

			explicit task_processor( const size_t thread_amount, bool process_on_stop = false, allocator allocator_object = allocator() )
				: allocator_( allocator_object )
                , stopping_( false )
				, process_on_stop_( process_on_stop )
				, working_threads_( 0 )
			{
				for( size_t i = 0 ; i < thread_amount ; ++i )
					threads_.create_thread( boost::bind( &task_processor::processing, this ) );
			}
			// !not a virtual destructor
			~task_processor()
			{
				stop();
				threads_.join_all();
			}
			bool add_task( task* const t )
			{
				if (stopping_)
					return false;
				return task_queue_.push( t );
			}
			size_t size() const
			{
				return task_queue_.size();
			}
			void wait()
			{
				task_queue_.wait();
				boost::mutex::scoped_lock lock( wait_ );
				while ( working_threads_ != 0 )
					wait_condition_.wait( lock );
			}
			void stop()
			{
				if ( process_on_stop_ )
				{
					stopping_ = true;
					task_queue_.wait();
				}
				task_queue_.stop();
			}
		private:
			void processing()
			{
				for (;;)
				{
					task* const t = task_queue_.wait_pop();
					if ( !t )
						return;
					{
						boost::mutex::scoped_lock lock( wait_ );
						++working_threads_;
					}
					(*t)();
					allocator_.destroy( t );
					allocator_.deallocate( t, 1 );
					{
						boost::mutex::scoped_lock lock( wait_ );
						--working_threads_;
						if ( working_threads_ == 0 )
							wait_condition_.notify_all();
					}
				}
			}
		};
	}
}

#endif // _SYSTEM_UTILITIES_COMMON_TASK_PROCESSOR_H_
