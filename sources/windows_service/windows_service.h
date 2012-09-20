#ifndef _SYSTEM_UTILITIES_COMMON_WINDOWS_SERVICE_H_
#define _SYSTEM_UTILITIES_COMMON_WINDOWS_SERVICE_H_

#include <string>
#include <boost/noncopyable.hpp>
#include <Windows.h>

namespace system_utilities
{
	namespace common
	{

		// windows service class created to gave possibility easy work with windows services
		// it compiles only under windows 
		// you should inherit from this class and create start_thread() and stop_thread() methods

		// usage example:
		// class service: public system_utilities::common::windows_service
		// {
		//		boost::thread thread_;
		//		system_processor::sp sp_;
		//	public:
		//		explicit service( const std::string& service_name, const int argc, char* const argv[], system_processor::sp sp_ptr )
		//		 : windows_service(service_name, service_name, true, argc, argv)
		//		 , sp_ ( sp_ptr )
		//		{
		//		}
		//	private:
		//		virtual void start_thread() 
		//		{
		//			thread_ = boost::thread( &calc_service::thread_func, this );
		//		}
		//		virtual void stop_thread() 
		//		{
		//			sp_->stop("Stop application");
		//		}
		//		void thread_func()
		//		{
		//			sp_->wait_for_stop();
		//			system_utilities::common::service_manager::stop( service_name() );
		//		}
		// };
		//

		class windows_service : virtual public boost::noncopyable
		{
			enum 
			{
				INSTALL,
				INSTALL_EX,
				UNINSTALL,
				RUN,
				STOP,
				NO_PARAMETERS,
				INCORRECT_USING
			} type_;
			const std::string service_name_;
			const std::string dispay_name_;
			const bool auto_start_;
			static windows_service* instance_;
			SERVICE_STATUS_HANDLE status_handle_;
			SERVICE_STATUS status_;

		public:
			explicit windows_service( const std::string& service_name, const int argc, char* const argv[] );
			explicit windows_service( const std::string& service_name, const std::string& display_name, const bool auto_start, const int argc, char* const argv[] );
			void process();
			virtual ~windows_service();

		private:
			void init_( const int argc, char* const argv[], const bool install_ex );
			void start_();
			void stop_();
			void run_service_();

			static void WINAPI service_main_(DWORD dwArgc, LPWSTR *lpszArgv);
			static void WINAPI service_ctrl_handler_(DWORD dwCtrl);

		protected:
			void set_service_status(DWORD dwCurrentState);

			virtual void start_thread() = 0;
			virtual void stop_thread() = 0;

			const std::string& service_name() const;
		};
	}
}

#endif // _SYSTEM_UTILITIES_COMMON_WINDOWS_SERVICE_H_

