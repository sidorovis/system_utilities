
#include "simple_timer_test.h"

typedef boost::mutex::scoped_lock lock;

simple_timer_test::simple_timer_test( const size_t micro_seconds )
: simple_timer( micro_seconds )
, run_started_counter_( 0 )
{
}
simple_timer_test::~simple_timer_test()
{
}
void simple_timer_test::run()
{
    lock run_waiter( run_protector_ );
    run_started_counter_ ++;
    run_waiter_.notify_one();
}
void simple_timer_test::wait_run()
{
    lock run_finished( run_protector_ );
    while (run_started_counter_ < 2)
        run_waiter_.wait( run_finished );
}
void simple_timer_test::test_full_timer_circle()
{
    begin();
    wait_run();
    end();
}
//
end_in_run_test::end_in_run_test()
 : simple_timer( 5 )
 , run_started_counter_( 0 )
{
}
end_in_run_test::~end_in_run_test()
{
	end();
}
void end_in_run_test::run()
{
	end();
    lock run_waiter( run_protector_ );
    run_started_counter_ ++;
    run_waiter_.notify_one();
}
void end_in_run_test::wait_run()
{
    lock run_finished( run_protector_ );
    while (run_started_counter_ < 1)
        run_waiter_.wait( run_finished );
}
void end_in_run_test::test_end_in_run()
{
	begin();
	wait_run();
}
//
using boost::unit_test_framework::test_case;

simple_timer_test_suite::simple_timer_test_suite()
: test_suite( "Simple Timer Test Suite" )
{
    boost::shared_ptr< simple_timer_test > instance( new simple_timer_test(500) );
    test_case* timer_test_case = BOOST_CLASS_TEST_CASE( &simple_timer_test::test_full_timer_circle, instance );
    BOOST_TEST_MESSAGE( "Registrate Simple Timer Test Case for 500 microsec" );
    add( timer_test_case );

    boost::shared_ptr< simple_timer_test > instance2( new simple_timer_test( 5000 ) );
    test_case* timer_test_case2 = BOOST_CLASS_TEST_CASE( &simple_timer_test::test_full_timer_circle, instance2 );
    BOOST_TEST_MESSAGE( "Registrate Simple Timer Test Case for 5 microsec" );
    add( timer_test_case2 );

    boost::shared_ptr< end_in_run_test > end_in_run( new end_in_run_test() );
    test_case* timer_test_case3 = BOOST_CLASS_TEST_CASE( &end_in_run_test::test_end_in_run, end_in_run );
    BOOST_TEST_MESSAGE( "Registrate end in run test case for 5 microsec" );
    add( timer_test_case3 );
}

using boost::unit_test_framework::test_suite;

test_suite* init_unit_test_suite(int, char**const)
{
    test_suite* top_test_suite = BOOST_TEST_SUITE("Simple Timer Testing");
    top_test_suite->add( new simple_timer_test_suite() );
    return top_test_suite;
}

