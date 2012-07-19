#include "test_registrator.h"

#include <windows_service.h>

using namespace system_utilities::common;

namespace system_utilities
{
	namespace tests_
	{
		namespace common
		{
			namespace details_
			{
				class windows_service_impl : public windows_service
				{
				public:
					windows_service_impl(const std::string& sn, const int argc, char* const argv[])
						: windows_service( sn, argc, argv )
					{

					}
				protected:
					virtual void start_thread() 
					{
						
					}

					virtual void stop_thread() 
					{
						
					}
				};
			}
			void windows_service_ctor_tests()
			{
				int argc = 0;
				char* const argv[] = {"",""};
				BOOST_CHECK_NO_THROW( details_::windows_service_impl( "TestCtor", argc, argv ) );
			}
		}
	}
}
