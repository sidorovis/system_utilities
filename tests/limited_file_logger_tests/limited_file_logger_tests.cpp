#include "test_registrator.h"

#include <limited_file_logger.h>
#include <ts_logger.h>
#include <time_tracker.h>

#include <boost/regex.hpp>

using namespace system_utilities::common;

namespace system_utilities
{
	namespace tests_
	{
		namespace common
		{
			void limited_file_logger_constructor_tests()
			{
				typedef ts_logger< limited_file_logger< true, true, true > > ts_limited_file_logger;
				{
					ts_limited_file_logger lfl( "file_name" );
					lfl.note() << "test example";
				}
				BOOST_CHECK_EQUAL( boost::filesystem::exists( "file_name" ), true );
				BOOST_CHECK_NO_THROW( boost::filesystem::remove( "file_name" ) );
				{
					limited_file_logger< true, true, true, 1ul > lfl( "file_name" );
				}
				BOOST_CHECK_EQUAL( boost::filesystem::exists( "file_name" ), true );
				BOOST_CHECK_NO_THROW( boost::filesystem::remove( "file_name" ) );
				{
					limited_file_logger< true, true, true, 1ul > lfl( "file_name" );
					lfl.note() << "test example";
				}
				BOOST_CHECK_EQUAL( boost::filesystem::file_size( "file_name" ), 53ul );
				BOOST_CHECK_EQUAL( boost::filesystem::exists( "file_name" ), true );
				BOOST_CHECK_NO_THROW( boost::filesystem::remove( "file_name" ) );
				{
					limited_file_logger< true, true, false, 1ul > lfl( "file_name" );
					lfl.note() << "test example";
				}
				BOOST_CHECK_EQUAL( boost::filesystem::file_size( "file_name" ), 14ul );
				BOOST_CHECK_EQUAL( boost::filesystem::exists( "file_name" ), true );
				BOOST_CHECK_NO_THROW( boost::filesystem::remove( "file_name" ) );
				{
					limited_file_logger< true, true, true, 1ul > lfl( "file_name" );
					for ( size_t i = 0 ; i < 8 * 1128 ; ++i )
						lfl.note() << "test example012345671234567890123456789012345678901234567890123456789012345678901234567";
				}
				BOOST_CHECK_EQUAL( boost::filesystem::exists( "file_name" ), true );
				BOOST_CHECK_EQUAL( boost::filesystem::exists( "file_name.old" ), true );
				BOOST_CHECK_EQUAL( boost::filesystem::file_size( "file_name.old" ) <= 1024 * 1024, true );
				{
					limited_file_logger< true, true, true, 1ul > lfl( "file_name" );
					for ( size_t i = 0 ; i < 8 * 1128 ; ++i )
					{
						lfl.note() << "test example012345671234567890123456789012345678901234567890123456789012345678901234567";
					}
				}
				BOOST_CHECK_EQUAL( boost::filesystem::exists( "file_name" ), true );
				BOOST_CHECK_EQUAL( boost::filesystem::exists( "file_name.old" ), true );
				BOOST_CHECK_EQUAL( boost::filesystem::file_size( "file_name.old" ) <= 1024 * 1024, true );
				BOOST_CHECK_NO_THROW( boost::filesystem::remove( "file_name" ) );
				BOOST_CHECK_NO_THROW( boost::filesystem::remove( "file_name.old" ) );
			}
		}
	}
}
