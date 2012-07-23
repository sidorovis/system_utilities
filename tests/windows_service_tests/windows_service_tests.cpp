#include "test_registrator.h"

#include <windows_service.h>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>

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

				std::string get_test_helper_path()
				{
					char szPath[MAX_PATH];
					GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath));
					boost::filesystem::path p(szPath);
					return p.parent_path().string() + "/windows_service_tests_helper.exe";
				}
				int check_install()
				{
					int res = 0;
					SC_HANDLE sch_manager = NULL;
					SC_HANDLE sch_service = NULL;

					sch_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
					if (sch_manager)
					{
						sch_service = OpenService(sch_manager, "win_srv_tests_helper", SERVICE_ALL_ACCESS);
						if (sch_service)
						{
							res = 1;
							CloseServiceHandle(sch_service);
						}
						CloseServiceHandle(sch_manager);
					}
					else
					{
						res = -1;
						std::cout << "Error in open SCManager" << std::endl;
					}
					return res;
				}
				int check_run()
				{
					int install_res = check_install();
					if (install_res != 1)
						return install_res;

					int res = 0;
					SC_HANDLE sch_manager = NULL;
					SC_HANDLE sch_service = NULL;

					sch_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
					if (sch_manager)
					{
						sch_service = OpenService(sch_manager, "win_srv_tests_helper", SERVICE_ALL_ACCESS);
						if (sch_service)
						{
							SERVICE_STATUS_PROCESS ssp_status;
							DWORD dw_temp;
							if (QueryServiceStatusEx(sch_service, SC_STATUS_PROCESS_INFO, 
								reinterpret_cast<LPBYTE> (&ssp_status), sizeof(SERVICE_STATUS_PROCESS),	&dw_temp ) == TRUE )
							{
								if (ssp_status.dwCurrentState == SERVICE_RUNNING)
									res = 1;
							}
							CloseServiceHandle(sch_service);
						}
						CloseServiceHandle(sch_manager);
					}
					else
					{
						res = -1;
						std::cout << "Error in open SCManager" << std::endl;
					}
					return res;
				 }
			}
			void windows_service_ctor_tests()
			{
				int argc = 0;
				char* const argv[] = {"",""};
				BOOST_CHECK_NO_THROW( details_::windows_service_impl( "TestCtor", argc, argv ) );
			}
			void windows_service_install_tests()
			{
				BOOST_CHECK_EQUAL( details_::check_install(), 0 );
				std::string cmd = details_::get_test_helper_path() + " install";
				int err = system( cmd.c_str() );
				BOOST_CHECK_EQUAL( err, 0 );
				BOOST_CHECK_EQUAL( details_::check_install(), 1 );

			}
			void windows_service_start_tests()
			{
				BOOST_CHECK_EQUAL( details_::check_run(), 0 );
				std::string cmd = details_::get_test_helper_path() + " start";
				int err = system( cmd.c_str() );
				BOOST_CHECK_EQUAL( err, 0 );
				boost::this_thread::sleep(boost::posix_time::milliseconds(500));
				BOOST_CHECK_EQUAL( boost::filesystem::exists(details_::get_test_helper_path() + ".txt"), true );
				BOOST_CHECK_EQUAL( details_::check_run(), 1 );
			}
			void windows_service_stop_tests()
			{
				BOOST_CHECK_EQUAL( details_::check_run(), 1 );
				std::string cmd = details_::get_test_helper_path() + " stop";
				int err = system( cmd.c_str() );
				BOOST_CHECK_EQUAL( err, 0 );
				boost::filesystem::remove(details_::get_test_helper_path() + ".txt");
				BOOST_CHECK_EQUAL( details_::check_run(), 0 );
			}
			void windows_service_uninstall_tests()
			{
				BOOST_CHECK_EQUAL( details_::check_install(), 1 );
				std::string cmd = details_::get_test_helper_path() + " uninstall";
				int err = system( cmd.c_str() );
				BOOST_CHECK_EQUAL( err, 0 );
				BOOST_CHECK_EQUAL( details_::check_install(), 0 );
			}
		}
	}
}
