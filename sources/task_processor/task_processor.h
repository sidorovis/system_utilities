#ifndef _SYSTEM_UTILITIES_COMMON_TASK_PROCESSOR_H_
#define _SYSTEM_UTILITIES_COMMON_TASK_PROCESSOR_H_

#include <ts_queue.h>

namespace system_utilities
{
	namespace common
	{
		template< 
			class task, 
			class task_queue = ts_queue< task >, 
			class allocator = std::allocator< task > >
		class task_processor : protected virtual boost::noncopyable
		{
			allocator& allocator_;
			boost::thread_group threads_;
			task_queue task_queue_;
			bool stopping_;
			bool process_on_stop_;

			explicit task_processor();
			explicit task_processor( const task_processor& );
			task_processor& operator=( const task_processor& );
		public:
			explicit task_processor( const size_t thread_amount, bool process_on_stop = false, allocator& allocator_object = allocator() )
				: stopping_( false )
				, process_on_stop_( process_on_stop )
				, allocator_( allocator_object )
			{
				for( size_t i = 0 ; i < thread_amount ; ++i )
					threads_.create_thread( boost::bind( &task_processor::processing, this ) );
			}
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
			const size_t size() const
			{
				return task_queue_.size();
			}
			void wait()
			{
				task_queue_.wait();
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
					(*t)();
					allocator_.destroy( t );
					allocator_.deallocate( t, 1 );
				}
			}
		};
	}
}

#endif // _SYSTEM_UTILITIES_COMMON_TASK_PROCESSOR_H_
