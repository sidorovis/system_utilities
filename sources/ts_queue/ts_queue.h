#ifndef _SYSTEM_UTILITIES_COMMON_TS_QUEUE_H_
#define _SYSTEM_UTILITIES_COMMON_TS_QUEUE_H_

#include <list>

#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>

namespace system_utilities
{
	// ts_queue: thread safe queue
	// this queue emulate default queue with thread safe protection
	// could be used into nultithread application
	// on 4 PC System demonstrate 2*10^6 push-pop iterations by 10-20 threads per second
	// non virtual destructor, please inherit only if you know what are you doing

    namespace common
    {
		template< 
			class T, 
			template< typename, typename > class container = std::list, 
			template< typename > class real_allocator_type = std::allocator >
		class ts_queue
		{
			class iterator;
			friend class iterator;

			typedef T* element_ptr;

			typedef container< element_ptr, real_allocator_type< element_ptr > > queue;

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
			// restart method: stop queue from processing, clead queue (with deleting not processed elements by delete)
            void restart()
            {
                stop_processing();
                stopping_ = false;
            }
			// stop method: stop queue, notify wait() and ts_pop() methods that wait for messages or result of processing
			// this method is thread safe
			void stop()
			{
				stopping_ = true;
				boost::mutex::scoped_lock lock( queue_protector_ );
				push_.notify_all();
                wait_.notify_all();
			}
			// stop_processing method: stop processing method stop queue, notify wait() and ts_pop() methods and flush not poped messages with delete.
			// this method is thread safe
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
			// non virtual destructor
			// this method is thread safe
			~ts_queue()
			{
				stop_processing();
			}
			// wait method: wait while user call stop(), stop_processing(), ~destructor() methods OR all messages will be poped out queue 
			// if stop(), stop_processing() method was called before - returns immediatly
			// this method is thread safe
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
			// push() method: push message into queue
			// if stop(), stop_processing() method was called before - returns immediatly
			// returns true - if message was added to queue
			// returns false - if message was not added to queue, check this parameter it could be reason of memory leak
			// this method is thread safe
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
			// pop() message returns pointer to message that was in queue
			// returns pointer to message or NULL
			// if queue is empty - returns NULL
			// it does not wait for push - just return NULL if there is no messages into queue
			// this method is thread safe
			value_type pop()
			{
				if (queue_.empty())
					return NULL;
				value_type result = queue_.front();
				queue_.pop_front();
                if (queue_.empty())
				{
					boost::mutex::scoped_lock lock( queue_protector_ );
                    wait_.notify_all();
				}
				return result;
			}
			// ts_pop() message returns pointer to message that was in queue
			// returns pointer to message or NULL
			// if queue is empty - returns NULL
			// it does not wait for push - just return NULL if there is no messages into queue
			// this method is not thread safe!
			value_type ts_pop()
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
			// wait_pop() message returns pointer to message that was in queue
			// returns pointer to message or NULL
			// if queue is empty, wait until stop(), stop_processing(), push() will be called.
			// if queue is stopping - return NULL
			// this method wait for push
			// this method is not thread safe!
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
			// size() method: returns 0 if queue is going to stop
			const size_t size() const 
			{
				if (stopping_)
					return 0;
				boost::mutex::scoped_lock lock( queue_protector_ );
				return queue_.size();
			}
			// ts_size() method: returns queue size
			// thread safe method
			const size_t ts_size() const
			{
				boost::mutex::scoped_lock lock( queue_protector_ );
				return queue_.size();
			}
			// empty() method: return true if queue is going to stop
			// returns false is queue.size() > 0
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
