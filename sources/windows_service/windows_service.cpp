#include "windows_service.h"
#include "service_manager.h"
#include <Windows.h>
#include <boost/bind.hpp>
#include <boost/scoped_array.hpp>

namespace system_utilities
{
	namespace common
	{
		windows_service* windows_service::instance_ = NULL;

		windows_service::windows_service( const std::string& service_name, const int argc, char* const argv[] )
			: service_name_( service_name )
		{
			status_handle_ = NULL;
			status_.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
			status_.dwCurrentState = SERVICE_START_PENDING;
			status_.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
			status_.dwWin32ExitCode = NO_ERROR;
			status_.dwServiceSpecificExitCode = 0;
			status_.dwCheckPoint = 0;
			status_.dwWaitHint = 0;

			if (argc == 1)
			{
				type_ = NO_PARAMETERS;
				return;
			}
			if (argc > 2)
			{
				type_ = INCORRECT_USING;
				return;
			}
			const std::string parameter(argv[1]);
			if (parameter == "install")
				type_ = INSTALL;
			else if (parameter == "uninstall")
				type_ = UNINSTALL;
			else if (parameter == "run" || parameter == "start")
				type_ = RUN;
			else if (parameter == "stop")
				type_ = STOP;
			else
				type_ = INCORRECT_USING;
		}

		void windows_service::process()
		{
			switch (type_)
			{
			case INCORRECT_USING:
				printf("Using <program name> install | uninstall | start | stop");
				return;
			case INSTALL:
				service_manager::install( service_name_ );
				break;
			case UNINSTALL:
				service_manager::uninstall( service_name_ );
				break;
			case RUN:
				service_manager::start( service_name_ );
				break;
			case STOP:
				service_manager::stop( service_name_ );
			case NO_PARAMETERS:
				run_service_();
				break;
			}
		}

		void windows_service::start_()
		{
			set_service_status(SERVICE_START_PENDING);

			start_thread();

			set_service_status(SERVICE_RUNNING);
		}
		void windows_service::stop_()
		{
			set_service_status(SERVICE_STOP_PENDING);

			stop_thread();

			set_service_status(SERVICE_STOPPED);
		}

		void windows_service::run_service_()
		{
			instance_ = this;
			boost::scoped_array< char > service_name_ptr(new char[service_name_.length() + 1]);
			strcpy_s(service_name_ptr.get(), service_name_.length() + 1, service_name_.c_str());
			SERVICE_TABLE_ENTRY serviceTable[] = 
			{
				{ service_name_ptr.get(), (LPSERVICE_MAIN_FUNCTION)service_main_ },
				{ NULL, NULL }
			};
			StartServiceCtrlDispatcher(serviceTable);
		}

		void WINAPI windows_service::service_main_(DWORD dwArgc, LPWSTR *lpszArgv)
		{
			assert(instance_ != NULL);

			instance_->status_handle_ = RegisterServiceCtrlHandler((char*)(instance_->service_name_.c_str()), service_ctrl_handler_ );

			instance_->start_();
		}

		void WINAPI windows_service::service_ctrl_handler_(DWORD dwCtrl)
		{
			assert(instance_ != NULL);
			switch (dwCtrl)
			{
			case SERVICE_CONTROL_STOP:
			case SERVICE_CONTROL_SHUTDOWN:
				instance_->stop_();
				break;
			}
		}

		void windows_service::set_service_status(DWORD dwCurrentState)
		{
			status_.dwCurrentState = dwCurrentState;
			SetServiceStatus(status_handle_, &status_);
		}

		const std::string& windows_service::service_name() const
		{
			return service_name_;
		}
	}
}


