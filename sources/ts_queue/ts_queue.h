#ifndef _SYSTEM_UTILITIES_COMMON_TS_QUEUE_H_
#define _SYSTEM_UTILITIES_COMMON_TS_QUEUE_H_

#include <list>

#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>

namespace system_utilities
{
    namespace common
    {
		template< 
			class T, 
			template< typename, typename > class container = std::list, 
			template< typename > class allocator_type = std::allocator >
		class ts_queue
		{
			class iterator;
			friend class iterator;

			typedef typename T* element_ptr;

			typedef container< element_ptr, allocator_type< element_ptr > > queue;

			explicit ts_queue(const ts_queue& );
			ts_queue& operator=(const ts_queue&);
		public:
			typedef typename queue::allocator_type allocator_type;
			typedef typename queue::value_type value_type;
			typedef typename queue::size_type size_type;
			typedef typename queue::reference reference;
			typedef typename queue::const_reference const_reference;

		private:
			queue queue_;
			mutable boost::mutex queue_protector_;
			boost::condition push_;
			boost::condition wait_;

			volatile bool stopping_;

		public:
			explicit ts_queue()
				: queue_( )
				, stopping_( false )
			{
			}
            void restart()
            {
                stop_processing();
                stopping_ = false;
            }
			void stop()
			{
				stopping_ = true;
				boost::mutex::scoped_lock lock( queue_protector_ );
				push_.notify_all();
                wait_.notify_all();
			}
			void stop_processing()
			{
				stopping_ = true;
				boost::mutex::scoped_lock lock( queue_protector_ );
				while (!queue_.empty())
				{
					delete queue_.front();
					queue_.pop_front();
				}
				push_.notify_all();
                wait_.notify_all();
			}
			~ts_queue()
			{
				stop_processing();
			}
            void wait()
            {
				if (stopping_)
                    return;
				boost::mutex::scoped_lock lock( queue_protector_ );
				if (stopping_)
                    return;
                while (!queue_.empty() && !stopping_)
                    wait_.wait( lock );
            }
			bool push(value_type val)
			{
				if (stopping_)
					return false;
				boost::mutex::scoped_lock lock( queue_protector_ );
				if (stopping_)
					return false;
				queue_.push_back( val );
				push_.notify_one();
				return true;
			}
			value_type ts_pop()
			{
				if (queue_.empty())
					return NULL;
				value_type result = queue_.front();
				queue_.pop_front();
                if (queue_.empty())
                    wait_.notify_all();
				return result;
			}
			value_type pop()
			{
				if (stopping_)
					return NULL;
				boost::mutex::scoped_lock lock( queue_protector_ );
				if (stopping_)
					return NULL;
				if (queue_.empty())
					return NULL;
				value_type result = queue_.front();
				queue_.pop_front();
                if (queue_.empty())
                    wait_.notify_all();
				return result;
			}
			value_type wait_pop()
			{
				if (stopping_)
					return NULL;
				boost::mutex::scoped_lock lock( queue_protector_ );
				if (stopping_)
					return NULL;
				while (queue_.empty())
				{
					push_.wait( lock );
					if (stopping_)
						return NULL;
				}
				value_type result = queue_.front();
				queue_.pop_front();
                if (queue_.empty())
                    wait_.notify_all();
				return result;
			}
			const size_t size() const 
			{
				if (stopping_)
					return 0;
				boost::mutex::scoped_lock lock( queue_protector_ );
				return queue_.size();
			}
			const size_t ts_size() const
			{
				boost::mutex::scoped_lock lock( queue_protector_ );
				return queue_.size();
			}
			const bool empty() const
			{
				if (stopping_)
					return true;
				boost::mutex::scoped_lock lock( queue_protector_ );
				return queue_.empty();
			}
		};
	}
}


#endif // _SYSTEM_UTILITIES_COMMON_TS_QUEUE_H_
