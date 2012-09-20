#ifndef _SYSTEM_UTILITIES_COMMON_SERVICE_MANAGER_H_
#define _SYSTEM_UTILITIES_COMMON_SERVICE_MANAGER_H_

#include <string>
#include <boost/noncopyable.hpp>
#include <Windows.h>

namespace system_utilities
{
	namespace common
	{
		// service manager class created to gave possibility easy work with windows services
		// it compiles only under windows 

		class service_manager : boost::noncopyable
		{
		public:
			enum service_start_type
			{
				AUTO_START = SERVICE_AUTO_START,
				DEMAND_START = SERVICE_DEMAND_START
			};
			static void install(const std::string& service_name);
			static void install(const std::string& service_name, const std::string& display_name, const service_start_type sst);
			static void uninstall(const std::string& service_name);
			static void start(const std::string& service_name);
			static void stop(const std::string& service_name);
		private:
			static void cleanup_(SC_HANDLE& schSCManager, SC_HANDLE& schService);
			static void print_error_( LPTSTR function_name );
		};
	}
}

#endif // _SYSTEM_UTILITIES_COMMON_SERVICE_MANAGER_H_
