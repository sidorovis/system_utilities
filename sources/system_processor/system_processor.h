#ifndef _SYSTEM_UTILITIES_COMMON_SYSTEM_PROCESSOR_H_
#define _SYSTEM_UTILITIES_COMMON_SYSTEM_PROCESSOR_H_

#include <property_reader.h>
#include <queue_logger.h>
#include <file_logger.h>

#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

namespace system_utilities
{
	namespace tests_
	{
		namespace common
		{
			void system_processor_constructor_tests();
		};
	}
    namespace common
    {
		namespace system_processor
		{
			namespace details
			{
				class sp_impl;
			}

			//
			typedef boost::shared_ptr< details::sp_impl > sp;
			//
			sp init( const int argc, char * const argv[], const std::string& default_config_file );
			//
			void stop( const std::string& reason );
			void wait_for_stop();
			void reset();
			//
			const std::string logs_path();
			//
			template< class result_type >
			result_type config( const std::string& name, const result_type& default_value = result_type() );
			//
			namespace details
			{
				void exit_handler(int);

				sp init_without_default_config_file( const std::string& binary_path, const std::string& config_file_path );
				sp init_with_default_config_file( const std::string& binary_path, const std::string& config_file_path, const std::string& default_config_file );

				template< class result_type >
				result_type config_impl( const std::string& name, const result_type& default_value = result_type() );

				class sp_impl : protected virtual boost::noncopyable
				{
					friend void tests_::common::system_processor_constructor_tests();

					//
					friend sp system_processor::init( const int argc, char* const argv[], const std::string& default_config_file );
					friend sp init_without_default_config_file( const std::string& binary_path, const std::string& config_file_path );
					friend sp init_with_default_config_file( const std::string& binary_path, const std::string& config_file_path, const std::string& default_config_file );

					template< typename result_type >
					friend result_type config_impl( const std::string& name, const result_type& default_value );

					friend void system_processor::stop( const std::string& );
					friend void system_processor::wait_for_stop();
					friend void system_processor::reset();
					friend void details::exit_handler(int);
					//
					friend const std::string system_processor::logs_path();

					//
					static boost::mutex instance_protector_;
					static sp_impl* instance_;
					boost::shared_ptr< property_reader > properties_;
					boost::shared_ptr< file_logger<> > engine_logger_;

					mutable boost::mutex stop_protector_;
					mutable boost::condition stop_waiter_;
					bool stopping_;

					std::string binary_path_;
					std::string logs_path_;

					static std::string parse_config_file_name(const int argc, char* const argv[]);

					explicit sp_impl( const std::string& binary_path );
					explicit sp_impl( const std::string& binary_path, const std::string& config_path );

					void create_logs_directory();
					void read_logs_path();
					void create_timestamp_folder();
					void create_system_logger();
					void load_predefined_logs_settings();
					void add_exit_handlers();
					//
				public:
					~sp_impl();
					void stop( const std::string& reason );
					void wait_for_stop();
					void reset();
				private:
					static inline void check_that_instance_not_exists()
					{
						if (sp_impl::instance_)
							throw std::logic_error( "init method could not be runned, system created" );
					}
					static inline void check_argc_parameter( const int argc )
					{
						if (argc == 0)
							throw std::logic_error( "please give argc, argv parameters from main() function " );
					}
					inline void check_binary_path( const std::string& binary_path )
					{
						if ( binary_path.empty() || !(boost::filesystem::exists( binary_path )) )
							throw std::logic_error( "wrong argc, argv parameters, please send here real parameters" );
						binary_path_ = binary_path;
					}
				};

				template< class result_type >
				result_type config_impl( const std::string& name, const result_type& default_value )
				{
					boost::mutex::scoped_lock lock( details::sp_impl::instance_protector_ );
					if (!details::sp_impl::instance_.get())
						throw std::logic_error( "system processor should exist (init method)." );
					if (!details::sp_impl::instance_->properties_.get())
						throw std::logic_error( "config file should be added by parameters." );
					return details::sp_impl::instance_->properties_->get_value( name, default_value );
				}

			}

		};
	};
};

#endif // _SYSTEM_UTILITIES_COMMON_SYSTEM_PROCESSOR_H_

