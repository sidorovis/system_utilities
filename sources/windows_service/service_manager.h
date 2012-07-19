#ifndef _SYSTEM_UTILITIES_COMMON_SERVICE_MANAGER_H_
#define _SYSTEM_UTILITIES_COMMON_SERVICE_MANAGER_H_

#include <string>
#include <boost/noncopyable.hpp>
#include <Windows.h>

namespace system_utilities
{
	namespace common
	{
		class service_manager : boost::noncopyable
		{
		public:
			static void install(const std::string& service_name);
			static void uninstall(const std::string& service_name);
			static void start(const std::string& service_name);
			static void stop(const std::string& service_name);
		private:
			static void cleanup_(SC_HANDLE& schSCManager, SC_HANDLE& schService);
		};
	}
}

#endif // _SYSTEM_UTILITIES_COMMON_SERVICE_MANAGER_H_
