#ifndef _BOOST_DYNAMIC_TEST_HELPER_DYNAMIC_LINK_HELPER_H_
#define _BOOST_DYNAMIC_TEST_HELPER_DYNAMIC_LINK_HELPER_H_

#ifdef BOOST_TEST_DYN_LINK
#define TEST_INIT bool init_unit_test_suite()
#else
#define TEST_INIT boost::unit_test_framework::test_suite* init_unit_test_suite( int, char*[] )
#endif

#ifdef BOOST_TEST_DYN_LINK
#define TEST_RETURN true;
#else
#define TEST_RETURN NULL;
#endif

#ifdef BOOST_TEST_DYN_LINK
#define DEFINE_MAIN_FOR_DYNAMIC \
int main( int argc, char* argv[] ) \
{ \
    return ::boost::unit_test::unit_test_main( &init_unit_test_suite, argc, argv ); \
}
#else
#define DEFINE_MAIN_FOR_DYNAMIC
#endif


#endif // _BOOST_DYNAMIC_TEST_HELPER_DYNAMIC_LINK_HELPER_H_