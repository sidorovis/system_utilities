#include "system_processor.h"

#include <boost/filesystem.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <csignal>

namespace
{
}

namespace system_utilities
{
    namespace common
    {
		namespace system_processor
		{
			namespace details
			{

				void exit_handler(int)
				{
					stop( "ctrl+c pressed, system processor stopping" );
				}

				sp init_without_default_config_file( const std::string& binary_path, const std::string& config_file_path )
				{
					if ( config_file_path.empty() )
						sp_impl::instance_ = new sp_impl( binary_path );
					else
						sp_impl::instance_ = new sp_impl( binary_path, config_file_path );
					return sp( sp_impl::instance_ );
				}
				sp init_with_default_config_file( const std::string& binary_path, const std::string& config_file_path, const std::string& default_config_file )
				{
					if ( config_file_path.empty() )
						sp_impl::instance_ = new sp_impl( binary_path, default_config_file );
					else
						sp_impl::instance_ = new sp_impl( binary_path, config_file_path );
					return sp( sp_impl::instance_ );
				}
				//
				sp_impl* sp_impl::instance_ = NULL;
				boost::mutex sp_impl::instance_protector_;
				//
				sp_impl::sp_impl( const std::string& binary_path )
					: stopping_( false )
				{
					check_binary_path( binary_path );
				}
				sp_impl::sp_impl( const std::string& binary_path, const std::string& config_path )
					: stopping_( false )
				{
					check_binary_path( binary_path );
					std::string config_full_path = config_path;

					if (!boost::filesystem::exists( config_path ))
					{
						config_full_path = binary_path + config_path;
						if (!boost::filesystem::exists( config_full_path ) )
							throw std::logic_error( "config file cannot be found: " + config_path );
					}

					properties_.reset( new property_reader( config_full_path, binary_path ) );
					create_logs_directory();
					create_system_logger();
					load_predefined_logs_settings();
				}
				void sp_impl::create_logs_directory()
				{
					using namespace boost::filesystem;
					read_logs_path();
					if ( !exists( logs_path_ ) )
						create_directory( logs_path_ );
					if ( !is_directory( logs_path_ ) )
						throw std::logic_error( "logs folder setting: "+ logs_path_ + " incorrect. it is not a directory" );
					create_timestamp_folder();
				}
				void sp_impl::read_logs_path()
				{
					using namespace boost::filesystem;
					logs_path_ = properties_->get_value( "System.log.path", "logs" );
					path p( logs_path_ );
					if ( p.root_name().empty() )
						logs_path_ = binary_path_ + logs_path_;
					logs_path_ = path( logs_path_ + "/" ).parent_path().string() + "/";
				}

				void sp_impl::create_timestamp_folder()
				{
					using namespace boost::filesystem;
					static const boost::posix_time::ptime now = boost::posix_time::microsec_clock::universal_time();
					static std::locale loc( std::cout.getloc(), new boost::posix_time::time_facet("%Y%m%d_%H%M%S_%f"));
					std::stringstream log_out;
					log_out.imbue( loc );
					log_out << now << "/";
					const std::string time_stamp = log_out.str();

					logs_path_ += time_stamp;
					if ( !exists( logs_path_ ) )
						create_directory( logs_path_ );
					if ( !is_directory( logs_path_ ) )
						throw std::logic_error( "logs folder setting: "+ logs_path_ + " incorrect. it is not a directory" );
				}
				void sp_impl::create_system_logger()
				{
					std::string system_logger_name = "_engine.log";
					if ( properties_.get() )
						system_logger_name = properties_->get_value( "System.log.name", system_logger_name );
					engine_logger_.reset( new file_logger<>( logs_path_ + system_logger_name ) );
				}
				void sp_impl::load_predefined_logs_settings()
				{
					if ( !properties_ || !engine_logger_ )
						return;
					if ( properties_->get_value< bool >( "System.stop_by_ctrl_c", false ) )
					{
						engine_logger_->note( "System.stop_by_ctrl_c is set to yes" );
						add_exit_handlers();
					}
				}
				void sp_impl::add_exit_handlers()
				{
					signal( SIGINT, exit_handler );
					signal( SIGTERM, exit_handler );
				}
				//
				sp_impl::~sp_impl()
				{
					boost::mutex::scoped_lock lock( instance_protector_ );
					instance_ = NULL;
				}
				//
				void sp_impl::stop( const std::string& reason )
				{
					boost::mutex::scoped_lock lock( stop_protector_ );
					stopping_ = true;
					if ( !reason.empty() )
					{
						if ( engine_logger_.get() )
							engine_logger_->note( "Stopping system: " + reason );
						else
							std::cout << "Stopping system: " + reason << std::endl;
					}
					stop_waiter_.notify_all();
				}
				void sp_impl::wait_for_stop()
				{
					boost::mutex::scoped_lock lock( stop_protector_ );
					while (!stopping_)
						stop_waiter_.wait( lock );
				}
				void sp_impl::reset()
				{
					boost::mutex::scoped_lock lock( stop_protector_ );
					stopping_ = false;
				}
				//
				std::string sp_impl::parse_config_file_name(const int argc, char* const argv[])
				{
					using namespace boost::program_options;
					options_description desc("Options");
					desc.add_options()("config", value< std::string >(), "config file");

					variables_map vm;
					store( parse_command_line(argc, argv, desc), vm );
					notify( vm );  

					std::string config_file = "";
					if ( vm.count("config") )
						config_file = vm["config"].as< std::string >();

					return config_file;
				}

			}

			sp init( const int argc, char * const argv[], const std::string& default_config_file )
			{
				using details::sp_impl;
				boost::mutex::scoped_lock lock( sp_impl::instance_protector_ );
				sp_impl::check_that_instance_not_exists();
				sp_impl::check_argc_parameter( argc );
				const std::string binary_path = boost::filesystem::path( argv[0] ).parent_path().string() + "/";
				const std::string config_file_path = sp_impl::parse_config_file_name( argc, argv );

				if ( default_config_file.empty() )
					return details::init_without_default_config_file( binary_path, config_file_path );
				else
					return details::init_with_default_config_file( binary_path, config_file_path, default_config_file );
			}

			void stop( const std::string& reason )
			{
				boost::mutex::scoped_lock lock( details::sp_impl::instance_protector_ );
				if ( details::sp_impl::instance_ )
					details::sp_impl::instance_->stop( reason );
				else
					throw std::logic_error( "system processor was not create, call init first" );
			}
			void wait_for_stop()
			{
				details::sp_impl::instance_protector_.lock();
				if ( details::sp_impl::instance_ )
				{
					details::sp_impl::instance_protector_.unlock();
					details::sp_impl::instance_->wait_for_stop();
				}
				else
					throw std::logic_error( "system processor was not create, call init first" );
			}
			void reset()
			{
				boost::mutex::scoped_lock lock( details::sp_impl::instance_protector_ );
				if ( details::sp_impl::instance_ )
					details::sp_impl::instance_->reset();
				else
					throw std::logic_error( "system processor was not create, call init first" );
			}
			//
			const std::string binary_path()
			{
				boost::mutex::scoped_lock lock( details::sp_impl::instance_protector_ );
				if ( details::sp_impl::instance_ )
					return details::sp_impl::instance_->binary_path_;
				else
					throw std::logic_error( "system processor was not create, call init first" );
			}
			const std::string logs_path()
			{
				boost::mutex::scoped_lock lock( details::sp_impl::instance_protector_ );
				if ( details::sp_impl::instance_ )
					return details::sp_impl::instance_->logs_path_;
				else
					throw std::logic_error( "system processor was not create, call init first" );
			}

			//
			const std::string config( const std::string& name, const std::string& default_value )
			{
				return details::config_impl< std::string >( name, default_value );
			}
		}
	}
}
