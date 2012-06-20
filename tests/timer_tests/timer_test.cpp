#include "test_registrator.h"

#include <timer.h>

using namespace system_utilities::common;

namespace system_utilities
{
	namespace tests_
	{
		namespace common
		{
			namespace details
			{
				boost::mutex m_;
				boost::condition c_;
				size_t call_count_;

				void timer_check()
				{
					boost::mutex::scoped_lock lock( m_ );
					call_count_++;
					c_.notify_all();
				}

				struct timer_object_tests_helper
				{
					boost::mutex m_;
					boost::condition c_;
					size_t call_count_;

					timer t_;

					timer_object_tests_helper()
					{
						call_count_ = 0;
						t_ = create_timer( 100, *this, &timer_object_tests_helper::method );
						t_->begin();
						{
							boost::mutex::scoped_lock lock( m_ );
							c_.wait( lock );
						}
						BOOST_CHECK_EQUAL( details::call_count_, 1 );
					}
					void method()
					{
						boost::mutex::scoped_lock lock( m_ );
						call_count_++;
						c_.notify_all();
						t_->end();
					}
				};
			}
			void timer_constructor_tests()
			{
				BOOST_CHECK_NO_THROW( timer t = create_timer( 500, &details::timer_check ) );
			}
			void timer_begin_end_tests()
			{
				timer t = create_timer( 500, &details::timer_check );
				BOOST_CHECK_NO_THROW( t->begin() );
				BOOST_CHECK_NO_THROW( t->end() );
			}
			void timer_method_tests()
			{
				timer t = create_timer( 100, &details::timer_check );
				details::call_count_ = 0;
				t->begin();
				{
					boost::mutex::scoped_lock lock( details::m_ );
					details::c_.wait( lock );
					t->end();
				}
				BOOST_CHECK_EQUAL( details::call_count_ > 0, true );
			}
			void timer_object_method_tests()
			{
				details::timer_object_tests_helper toth;
			}
		}
	}
}
