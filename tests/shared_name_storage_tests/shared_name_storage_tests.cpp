#include "test_registrator.h"

#include <shared_name_storage.h>

void system_utilities::common::tests_::shared_name_storage_constructor_tests()
{
	BOOST_CHECK_NO_THROW( shared_name_storage() );
}
void system_utilities::common::tests_::shared_name_storage_add_name_tests()
{
	shared_name_storage sns;
	BOOST_CHECK_THROW( sns.add_name( "" ), std::exception );
	BOOST_CHECK_EQUAL( sns.add_name( "new" ), true );
	BOOST_CHECK_EQUAL( sns.add_name( "new" ), false );
	BOOST_CHECK_EQUAL( sns.add_name( "hello" ), true );
	BOOST_CHECK_EQUAL( sns.add_name( "world" ), true );
	BOOST_CHECK_EQUAL( sns.add_name( "hello" ), false );
	BOOST_CHECK_EQUAL( sns.add_name( "new" ), false );
}
void system_utilities::common::tests_::shared_name_storage_operator_in_tests()
{
	shared_name_storage sns;
	BOOST_CHECK_THROW( ( sns << "" ), std::exception );
	sns << "hello" << "world" << "new" << "world";
	BOOST_CHECK_EQUAL( sns.add_name( "new" ), false );
	BOOST_CHECK_EQUAL( sns.add_name( "hello" ), false );
	BOOST_CHECK_EQUAL( sns.add_name( "world" ), false );
}
void system_utilities::common::tests_::shared_name_storage_get_shared_tests()
{
	shared_name_storage sns;
	sns << "hello" << "world" << "new" << "world";
	BOOST_CHECK_NO_THROW( sns.get_shared( "hello" ) );
	BOOST_CHECK_NO_THROW( sns.get_shared( "new" ) );
	BOOST_CHECK_NO_THROW( sns.get_shared( "unexisted" ) );
	BOOST_CHECK_NO_THROW( sns.get_shared( "world" ) );

	BOOST_CHECK_EQUAL( sns.get_shared( "hello" ), sns.get_shared( "hello" ) );
	BOOST_CHECK_EQUAL( *(sns.get_shared( "new" )), "new" );
	BOOST_CHECK_EQUAL( *(sns.get_shared( "unexisted" )), "" );
	BOOST_CHECK_EQUAL( *(sns.get_shared( "world" )), "world" );
}
void system_utilities::common::tests_::shared_name_storage_check_shared_tests()
{
	shared_name_storage sns;
	sns << "hello" << "world" << "new" << "world";
	shared_name_storage::shared_string ss = sns.get_shared( "hello" );
	BOOST_CHECK_EQUAL( ss.get(), sns.get_shared( "hello" ).get() );
	BOOST_CHECK_EQUAL( sns.check_shared( ss ), true );
	BOOST_CHECK_EQUAL( sns.check_shared( shared_name_storage::shared_string( new std::string( "" ) ) ), false );
	BOOST_CHECK_EQUAL( sns.check_shared( shared_name_storage::shared_string( new std::string( "hello" ) ) ), false );
}
void system_utilities::common::tests_::shared_name_storage_iterator_tests()
{
	shared_name_storage sns;
	sns << "hello" << "world" << "new" << "world";
	std::string arr[] = { "hello", "new", "world" };
	size_t ind = 0;
	for( shared_name_storage::const_iterator i = sns.begin() ; i != sns.end() ; ++i )
	{
		BOOST_CHECK_EQUAL( *(i.shared()), arr[ind] );
		BOOST_CHECK_EQUAL( *i, arr[ind] );
		++ind;
	}
}
//
void system_utilities::common::tests_::shared_name_storage_clear_tests()
{
	typedef shared_name_storage::shared_string shared_string;
	shared_name_storage sns;
	sns.clear();
	sns.add_name( "hello" );
	shared_string hello = sns.get_shared( "hello" );

	BOOST_CHECK_EQUAL( sns.check_shared( hello ), true );
	sns.clear();
	BOOST_CHECK_EQUAL( sns.check_shared( hello ), false );
}
void system_utilities::common::tests_::shared_name_storage_del_name_tests()
{
	typedef shared_name_storage::shared_string shared_string;
	shared_name_storage sns;
	sns.clear();
	sns.add_name( "hello" );
	shared_string hello = sns.get_shared( "hello" );

	BOOST_CHECK_EQUAL( sns.check_shared( hello ), true );
	sns.del_name( "hello" );
	BOOST_CHECK_EQUAL( sns.check_shared( hello ), false );

	sns.add_name( "hello" );
	hello = sns.get_shared( "hello" );
	BOOST_CHECK_EQUAL( sns.check_shared( hello ), true );
	sns.del_name( hello );
	BOOST_CHECK_EQUAL( sns.check_shared( hello ), false );
}
