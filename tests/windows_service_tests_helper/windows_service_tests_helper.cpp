#include <windows_service.h>
#include <fstream>
#include <boost/thread.hpp>

using namespace system_utilities::common;

namespace system_utilities
{
	namespace tests_
	{
		namespace common
		{
			class windows_service_helper : public windows_service
			{
				boost::thread thread_;
				std::string pth, pth2;
			public:
				windows_service_helper(const std::string& sn, const int argc, char* const argv[])
					: windows_service( sn, argc, argv )
				{
				}
			protected:
				virtual void start_thread() 
				{
					thread_ = boost::thread(&windows_service_helper::thread_func_, this);
				}

				virtual void stop_thread() 
				{
					thread_.join();
				}
			private:
				void thread_func_()
				{
					char szPath[MAX_PATH];
					GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath));
					std::ofstream out((std::string(szPath) + ".txt").c_str());
					out << "Running" << std::endl;
					out.close();
				}
			};
		}
	}
}

int main(const int argc, char* const argv[])
{
	system_utilities::tests_::common::windows_service_helper wsh( "win_srv_tests_helper", argc, argv );
	wsh.process();
	return 0;
}