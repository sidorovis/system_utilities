#include "test_registrator.h"

#include <deque>
#include <queue>

#include <ts_queue.h>
#include <time_tracker.h>


#include <boost/thread.hpp>

using namespace system_utilities::common;

namespace system_utilities
{
	namespace tests_
	{
		namespace common
		{
			namespace details
			{
				void ts_queue_constructor_test_helper()
				{
					ts_queue< size_t > mq;
					for (int i = 1 ; i < 10 ; i ++)
						mq.push( new size_t(i) );
					size_t *s = mq.pop();
					BOOST_CHECK_EQUAL( *s, (size_t)1 );
					delete s;
					for (int i = 10 ; i < 14 ; i ++)
						mq.push( new size_t(i) );
					size_t i = 2;
					while (!mq.empty())
					{
						size_t* s = mq.pop();
						BOOST_CHECK_EQUAL( *s, i++ );
						delete s;
					}
					BOOST_CHECK_EQUAL( mq.empty(), true );
				}
				typedef ts_queue< size_t > ts_queue_size_t;
				void ts_queue_different_threads_pop_thread_helper( ts_queue_size_t* mq, const size_t different_threads_test_size )
				{
					boost::this_thread::sleep( boost::posix_time::milliseconds( 20 ) );
					size_t i = 0;
					while ( i < different_threads_test_size )
					{
						size_t* s = mq->wait_pop();
						BOOST_CHECK_EQUAL( i, *s );
						delete s;
						i++;
					}
					BOOST_CHECK_EQUAL( i, different_threads_test_size );
					BOOST_CHECK_EQUAL( mq->empty(), true );
					BOOST_CHECK_EQUAL( mq->size(), (size_t)0 );
				}
				void ts_queue_wait_pop_test_helper( ts_queue_size_t* mq )
				{
					time_tracker tt;
					using namespace boost::posix_time;
					size_t *s = mq->wait_pop();
					BOOST_CHECK_EQUAL( *s, (size_t)15 );
					delete s;
					BOOST_CHECK_EQUAL( tt.seconds() >= 1, true );
					BOOST_CHECK_EQUAL( mq->empty(), true );
					BOOST_CHECK_EQUAL( mq->size(), (size_t)0 );
				}
				struct ts_queue_many_threads_test_helper
				{
					ts_queue_size_t mq_;
					boost::mutex push_protector_;
					size_t push_iterations_;

					boost::mutex pop_protector_;
					size_t pop_iterations_;
					explicit ts_queue_many_threads_test_helper()
						: push_iterations_( 0 )
						, pop_iterations_( 0 )
					{
						const size_t push_threads = 15;
						const size_t pop_threads = 13;
						BOOST_CHECK_EQUAL( mq_.size(), (size_t)0 );
						BOOST_CHECK_EQUAL( mq_.empty(), true );
						boost::thread_group tg_push, tg_pop;
						for (size_t i = 0 ; i < push_threads ; ++i)
							tg_push.create_thread( boost::bind( &ts_queue_many_threads_test_helper::pusher, this ) );
						for (size_t i = 0 ; i < pop_threads ; ++i)
							tg_pop.create_thread( boost::bind( &ts_queue_many_threads_test_helper::poper, this ) );
						for (size_t i = 0 ; i < 12 ; ++i)
						{
							boost::this_thread::sleep( boost::posix_time::seconds( 2 ) );
							boost::mutex::scoped_lock push_lock( push_protector_ );
							boost::mutex::scoped_lock pop_lock( pop_protector_ );
							BOOST_CHECK_EQUAL( push_iterations_ - pop_iterations_ > 0, true );
						}
						mq_.stop_processing();
						tg_pop.join_all();
						tg_push.join_all();
					}
					void pusher()
					{
						while (true)
						{
							size_t * s = new size_t( rand() % 100000 );
							bool res = mq_.push( s );
							if (!res)
							{
								delete s;
								break;
							}
							{
								boost::mutex::scoped_lock lock( push_protector_ );
								push_iterations_ ++;
							}
						}
					}
					void poper()
					{
						while (true)
						{
							size_t* s = mq_.wait_pop();
							if (!s)
								break;
							delete s;
							{
								boost::mutex::scoped_lock lock( pop_protector_ );
								pop_iterations_ ++;
							}
						}
					}
				};
				void ts_queue_wait_test_helper(details::ts_queue_size_t* mq_, size_t* pop_iterations_)
				{
					while (true)
					{
						size_t* s = mq_->wait_pop();
						if (!s)
							break;
						delete s;
						(*pop_iterations_)++;
					}
				}
			}
			void ts_queue_constructor_tests()
			{
				ts_queue< int > queue;
				details::ts_queue_constructor_test_helper();
			}
			void ts_queue_different_threads_tests()
			{
				details::ts_queue_size_t mq;
				const size_t different_threads_test_size = 1000000;
				boost::thread pop = boost::thread( boost::bind( &details::ts_queue_different_threads_pop_thread_helper, &mq, different_threads_test_size ) );
				using namespace boost::posix_time;
				for (size_t i = 0 ; i < different_threads_test_size ; i++)
				{
					mq.push( new size_t(i) );
				}
				pop.join();
				BOOST_CHECK_EQUAL( mq.empty(), true );
			}
			void ts_queue_wait_pop_tests()
			{
				details::ts_queue_size_t mq;
				BOOST_CHECK_EQUAL( mq.empty(), true );
				BOOST_CHECK_EQUAL( mq.size(), (size_t)0 );
				boost::thread pop = boost::thread( boost::bind( &details::ts_queue_wait_pop_test_helper, &mq ) );
				boost::this_thread::sleep( boost::posix_time::milliseconds( 1200 ) );
				mq.push( new size_t(15) );
				pop.join();
				BOOST_CHECK_EQUAL( mq.empty(), true );
				BOOST_CHECK_EQUAL( mq.size(), (size_t)0 );
			}
			void ts_queue_many_threads_tests()
			{
				details::ts_queue_many_threads_test_helper helper;
			}
			void ts_queue_wait_tests()
			{
				details::ts_queue_size_t mq;
				mq.restart();
				BOOST_CHECK_EQUAL( mq.size(), (size_t)0 );
				BOOST_CHECK_EQUAL( mq.empty(), true );
				const size_t size = 1000000;
				for(size_t i = 0 ; i < size ; ++i )
				{
					size_t* s = new size_t(i);
					mq.push( s );
				}
				BOOST_CHECK_EQUAL( mq.size(), (size_t)size );
				size_t pop_iterations_ = 0;
				boost::thread poper( boost::bind( &details::ts_queue_wait_test_helper, &mq, &pop_iterations_ ) );
				mq.wait();
				mq.stop();
				poper.join();
				BOOST_CHECK_EQUAL(pop_iterations_, size);
			}

			void ts_queue_another_container_tests()
			{
				ts_queue< int, std::deque > deque_ts;
				deque_ts.push( new int(5) );
				int* a = deque_ts.pop();
				BOOST_CHECK_EQUAL( *a, 5 );
				delete a;
			}
		}
	}
}
