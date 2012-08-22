#include "service_manager.h"

#include <Windows.h>
#include <strsafe.h>

#include <boost/scoped_array.hpp>

#include <cstdio>

namespace system_utilities
{
	namespace common
	{
		void service_manager::install(const std::string& service_name)
		{
			install(service_name, service_name, DEMAND_START);
		}

		void service_manager::install(const std::string& service_name, const std::string& display_name, const service_start_type sst)
		{
			boost::scoped_array< char > service_name_ptr(new char[service_name.length() + 1]);
			strcpy_s(service_name_ptr.get(), service_name.length() + 1, service_name.c_str());
			boost::scoped_array< char > display_name_ptr(new char[display_name.length() + 1]);
			strcpy_s(display_name_ptr.get(), display_name.length() + 1, display_name.c_str());
			char szPath[MAX_PATH];
			SC_HANDLE schSCManager = NULL;
			SC_HANDLE schService = NULL;

			if (sst != AUTO_START && sst != DEMAND_START)
			{
				printf("Incorrect call of install function. Check the parameters.");
				return;
			}

			if (GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath)) == 0)
			{
				print_error_("GetModuleFileName");
				cleanup_(schSCManager, schService);
				return;
			}

			// Open the local default service control manager database
			schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT | 
				SC_MANAGER_CREATE_SERVICE);
			if (schSCManager == NULL)
			{
				print_error_("OpenSCManager");
				cleanup_(schSCManager, schService);
				return;
			}

			// Install the service into SCM by calling CreateService
			schService = CreateService(
				schSCManager,                   // SCManager database
				service_name_ptr.get(),			// Name of service
				display_name_ptr.get(),			// Name to display
				SERVICE_ALL_ACCESS,				// Desired access
				SERVICE_WIN32_OWN_PROCESS,		// Service type
				sst,							// Service start type
				SERVICE_ERROR_NORMAL,           // Error control type
				szPath,                         // Service's binary
				NULL,                           // No load ordering group
				NULL,                           // No tag identifier
				NULL,							// Dependencies
				NULL,							// Service running account
				NULL							// Password of the account
				);
			if (schService == NULL)
			{
				print_error_("CreateService");
				cleanup_(schSCManager, schService);
				return;
			}

			printf("%s is installed.\n", service_name_ptr.get());

			cleanup_(schSCManager, schService);
		}

		void service_manager::uninstall(const std::string& service_name)
		{
			boost::scoped_array< char > service_name_ptr(new char[service_name.length() + 1]);
			strcpy_s(service_name_ptr.get(), service_name.length() + 1, service_name.c_str());
			SC_HANDLE schSCManager = NULL;
			SC_HANDLE schService = NULL;
			SERVICE_STATUS ssSvcStatus = {};

			// Open the local default service control manager database
			schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
			if (schSCManager == NULL)
			{
				print_error_("OpenSCManager");
				cleanup_(schSCManager, schService);
				return;
			}

			// Open the service with delete, stop, and query status permissions
			schService = OpenService(schSCManager, service_name_ptr.get(), SERVICE_STOP | SERVICE_QUERY_STATUS | DELETE);
			if (schService == NULL)
			{
				print_error_("OpenService");
				cleanup_(schSCManager, schService);
				return;
			}

			// Try to stop the service
			if (ControlService(schService, SERVICE_CONTROL_STOP, &ssSvcStatus))
			{
				printf("Stopping %s.", service_name_ptr.get());
				Sleep(1000);

				while (QueryServiceStatus(schService, &ssSvcStatus))
				{
					if (ssSvcStatus.dwCurrentState == SERVICE_STOP_PENDING)
					{
						printf(".");
						Sleep(1000);
					}
					else break;
				}

				if (ssSvcStatus.dwCurrentState == SERVICE_STOPPED)
				{
					printf("\n%s is stopped.\n", service_name_ptr.get());
				}
				else
				{
					printf("\n%s failed to stop.\n", service_name_ptr.get());
				}
			}

			// Now remove the service by calling DeleteService.
			if (!DeleteService(schService))
			{
				print_error_("DeleteService");
				cleanup_(schSCManager, schService);
				return;
			}

			printf("%s is removed.\n", service_name_ptr.get());

			cleanup_(schSCManager, schService);
		}

		void service_manager::start(const std::string& service_name)
		{
			boost::scoped_array< char > service_name_ptr(new char[service_name.length() + 1]);
			strcpy_s(service_name_ptr.get(), service_name.length() + 1, service_name.c_str());
			SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
			SC_HANDLE hService = OpenService(hSCManager, service_name_ptr.get(), SERVICE_START);
			if ( !StartService(hService, 0, NULL) )
			{
				print_error_("StartService");
			}

			cleanup_(hSCManager, hService);
		}
		void service_manager::stop(const std::string& service_name)
		{
			boost::scoped_array< char > service_name_ptr(new char[service_name.length() + 1]);
			strcpy_s(service_name_ptr.get(), service_name.length() + 1, service_name.c_str());
			SC_HANDLE schSCManager = NULL;
			SC_HANDLE schService = NULL;
			SERVICE_STATUS ssSvcStatus = {};

			// Open the local default service control manager database
			schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
			if (schSCManager == NULL)
			{
				print_error_("OpenSCManager");
				cleanup_(schSCManager, schService);
				return;
			}

			// Open the service with delete, stop, and query status permissions
			schService = OpenService(schSCManager, service_name_ptr.get(), SERVICE_STOP | 
				SERVICE_QUERY_STATUS );
			if (schService == NULL)
			{
				print_error_("OpenService");
				cleanup_(schSCManager, schService);
				return;
			}

			// Try to stop the service
			if (ControlService(schService, SERVICE_CONTROL_STOP, &ssSvcStatus))
			{
				printf("Stopping %s.", service_name_ptr.get());
				Sleep(1000);

				while (QueryServiceStatus(schService, &ssSvcStatus))
				{
					if (ssSvcStatus.dwCurrentState == SERVICE_STOP_PENDING)
					{
						printf(".");
						Sleep(1000);
					}
					else break;
				}

				if (ssSvcStatus.dwCurrentState == SERVICE_STOPPED)
				{
					printf("\n%s is stopped.\n", service_name_ptr.get());
				}
				else
				{
					printf("\n%s failed to stop.\n", service_name_ptr.get());
				}
			}

			cleanup_(schSCManager, schService);
		}

		void service_manager::cleanup_(SC_HANDLE& schSCManager, SC_HANDLE& schService)
		{
			if (schService)
			{
				CloseServiceHandle(schService);
				schService = NULL;
			}
			if (schSCManager)
			{
				CloseServiceHandle(schSCManager);
				schSCManager = NULL;
			}
		}

		void service_manager::print_error_( LPTSTR lpszFuncName )
		{
			LPVOID lpMsgBuf;
			const DWORD dw = GetLastError();

			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				dw,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR) &lpMsgBuf,	0, NULL );

			printf("%s failed with error %d: %s\n", lpszFuncName, dw, lpMsgBuf);

			LocalFree(lpMsgBuf);
		}
	}
}
