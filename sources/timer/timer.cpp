#include "timer.h"
//
//#include <boost/bind.hpp>
//
//typedef boost::recursive_mutex::scoped_lock lock;
//
//using namespace re_utils::timers;
//
//simple_timer::simple_timer( const size_t sleep_microseconds = 3000 )
//: sleep_microseconds_ ( sleep_microseconds )
//, started_ ( false )
//, should_stop_ ( false )
//{
//    
//}
//simple_timer::~simple_timer()
//{
//    end();
//    thread_for_run_.join();
//}
//boost::xtime simple_timer::sleep_interval()
//{
//    boost::xtime time;
//    boost::xtime_get( &time, boost::TIME_UTC );
//	if (sleep_microseconds_ >= 1000)
//	{
//		size_t sec_inc = sleep_microseconds_ / 1000;
//		time.sec += sec_inc;
//		time.nsec += (boost::xtime::xtime_nsec_t)((sleep_microseconds_ - sec_inc * 1000) * 1000000);
//	}
//	else
//	    time.nsec += (boost::xtime::xtime_nsec_t)((sleep_microseconds_ * 1000000));
//    return time;
//}
//
//void simple_timer::begin()
//{
//    started_ = true;
//    should_stop_ = false;
//    thread_for_run_ = boost::thread( boost::bind( &simple_timer::runner, this ) );
//}
//void simple_timer::runner()
//{
//    try
//    {
//        for(;;)
//        {
//			if (should_stop_)
//                break;
//            lock run_func( should_stop_protector_ );
//            before_run();
//            run();
//			if (should_stop_)
//                break;
//            for_stop_.timed_wait( run_func, sleep_interval() );
//            if (should_stop_)
//                break;
//        }
//    }
//    catch (...)
//    {
//    }
//}
//void simple_timer::end()
//{
//    if (!started_ || should_stop_)
//        return;
//    {
//        should_stop_ = true;
//        lock stopper( should_stop_protector_ );
//        for_stop_.notify_one();
//    }
//    started_ = false;
//}
//
//
//void simple_timer::before_run()
//{
//}
